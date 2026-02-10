import { Storage } from '@google-cloud/storage';
import { v2 as speechV2 } from '@google-cloud/speech';
import { google } from 'googleapis';
import { Document, Packer, Paragraph, TextRun } from 'docx';
import path from 'path';
import { Readable } from 'stream';

/* -------------------------------------------------- */
/* ENV / PROJECT */
/* -------------------------------------------------- */

const PROJECT_ID = process.env.PROJECT_ID;
const DRIVE_ROOT_FOLDER_ID = process.env.DRIVE_ROOT_FOLDER_ID;

if (!PROJECT_ID || !DRIVE_ROOT_FOLDER_ID) {
  console.error('❌ Missing env config');
  throw new Error('Missing env config');
}

console.log('🧠 Project ID:', PROJECT_ID);
console.log('📂 Drive root folder ID:', DRIVE_ROOT_FOLDER_ID);

/* -------------------------------------------------- */
/* CLIENTS */
/* -------------------------------------------------- */

const storage = new Storage();
const speech = new speechV2.SpeechClient({ projectId: PROJECT_ID });

const auth = new google.auth.GoogleAuth({
  scopes: ['https://www.googleapis.com/auth/drive'],
});

const drive = google.drive({ version: 'v3', auth });

/* -------------------------------------------------- */
/* CONSTANTS */
/* -------------------------------------------------- */

const BUCKET_NAME = 'futa-lecture-recordings';

/* -------------------------------------------------- */
/* HELPERS */
/* -------------------------------------------------- */

const isWavFile = (name) => name.toLowerCase().endsWith('.wav');
const isTranscript = (name) =>
  name.toLowerCase().endsWith('.txt') || name.toLowerCase().endsWith('.docx');

const getFolderPath = (objectName) => {
  const idx = objectName.lastIndexOf('/');
  return idx === -1 ? '' : objectName.substring(0, idx);
};

const getBaseFilename = (objectName) =>
  path.basename(objectName, path.extname(objectName));

/**
 * Create or reuse folders in Shared Drive
 */
async function getOrCreateDriveFolderPath(folderPath) {
  let parentId = DRIVE_ROOT_FOLDER_ID;
  if (!folderPath) return parentId;

  const parts = folderPath.split('/').filter(Boolean);

  for (const part of parts) {
    const res = await drive.files.list({
      q: `'${parentId}' in parents and name='${part}' and mimeType='application/vnd.google-apps.folder' and trashed=false`,
      fields: 'files(id)',
      supportsAllDrives: true,
      includeItemsFromAllDrives: true,
    });

    if (res.data.files.length > 0) {
      parentId = res.data.files[0].id;
    } else {
      const folder = await drive.files.create({
        supportsAllDrives: true,
        requestBody: {
          name: part,
          mimeType: 'application/vnd.google-apps.folder',
          parents: [parentId],
        },
        fields: 'id',
      });
      parentId = folder.data.id;
    }
  }

  return parentId;
}

/**
 * Upload buffer to Google Drive (Shared Drive safe)
 */
async function uploadBufferToDrive({ buffer, name, mimeType, parentId }) {
  const stream = buffer instanceof Buffer
    ? Readable.from(buffer)
    : buffer;

  await drive.files.create({
    supportsAllDrives: true,
    requestBody: {
      name,
      parents: [parentId],
    },
    media: {
      mimeType,
      body: stream,
    },
  });
}

/* -------------------------------------------------- */
/* MAIN FUNCTION */
/* -------------------------------------------------- */

export const autoTranscribeOnUpload = async (event) => {
  const { bucket, name: objectName } = event;

  if (!bucket || !objectName) return;
  if (isTranscript(objectName) || !isWavFile(objectName)) return;

  console.log(`🎧 New audio uploaded: gs://${bucket}/${objectName}`);

  const gcsUri = `gs://${bucket}/${objectName}`;
  const folderPath = getFolderPath(objectName);
  const baseName = getBaseFilename(objectName);

  /* ------------------ Speech-to-Text ------------------ */

  const request = {
    recognizer: `projects/${PROJECT_ID}/locations/global/recognizers/_`,
    config: {
      languageCodes: ['en-US'],
      model: 'long',
      autoDecodingConfig: {},
      features: { enableAutomaticPunctuation: true },
    },
    files: [{ uri: gcsUri }],
    recognitionOutputConfig: { inlineResponseConfig: {} },
  };

  let transcriptText = '';

  try {
    console.log('📝 Starting transcription...');
    const [operation] = await speech.batchRecognize(request);
    const [response] = await operation.promise();

    for (const file of Object.values(response.results)) {
      for (const r of file.transcript?.results || []) {
        transcriptText += r.alternatives?.[0]?.transcript + '\n';
      }
    }
  } catch (err) {
    console.error('❌ Speech failed:', err);
    return;
  }

  if (!transcriptText.trim()) {
    console.log('⚠️ Empty transcription');
    return;
  }

  /* ------------------ Save transcripts to GCS ------------------ */

  const txtPath = `${folderPath}/${baseName}.txt`;
  await storage.bucket(BUCKET_NAME).file(txtPath).save(transcriptText);

  const doc = new Document({
    sections: [{
      children: transcriptText
        .split('\n')
        .filter(Boolean)
        .map(line => new Paragraph({ children: [new TextRun(line)] })),
    }],
  });

  const docxBuffer = await Packer.toBuffer(doc);
  console.log('✅ Files saved to GCS');

  /* ------------------ Upload to Drive ------------------ */

  try {
    const driveFolderId = await getOrCreateDriveFolderPath(folderPath);

    // 🔊 DOWNLOAD AUDIO FROM GCS
    const [audioBuffer] = await storage
      .bucket(bucket)
      .file(objectName)
      .download();

    // 🔊 UPLOAD AUDIO TO DRIVE
    await uploadBufferToDrive({
      buffer: audioBuffer,
      name: path.basename(objectName),
      mimeType: 'audio/wav',
      parentId: driveFolderId,
    });

    // 📝 TXT
    await uploadBufferToDrive({
      buffer: Buffer.from(transcriptText, 'utf-8'),
      name: `${baseName}.txt`,
      mimeType: 'text/plain',
      parentId: driveFolderId,
    });

    // 📄 DOCX
    await uploadBufferToDrive({
      buffer: docxBuffer,
      name: `${baseName}.docx`,
      mimeType:
        'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
      parentId: driveFolderId,
    });

    console.log('📁 Uploaded AUDIO + TRANSCRIPTS to Drive (mirrored structure)');
  } catch (err) {
    console.error('❌ Drive upload failed:', err);
  }
};
