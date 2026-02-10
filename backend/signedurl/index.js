const express = require('express');
const { Storage } = require('@google-cloud/storage');
const crypto = require('crypto');

const app = express();
app.use(express.json());

const storage = new Storage();
const BUCKET = "YOUR_BUCKET_NAME";

// ------------------ Helpers ------------------

function sanitize(value, fallback) {
  if (!value) return fallback;
  return String(value).replace(/[^a-zA-Z0-9_-]/g, '');
}

function makeRecordingId(deviceId) {
  const now = new Date();
  const ts = now.toISOString().replace(/[:-]/g, '').replace(/\..+/, '');
  const rand = crypto.randomBytes(3).toString('hex');
  return `LEC-${ts}-${deviceId}-${rand}`;
}

// ------------------ Health ------------------
app.get('/', (req, res) =>
  res.json({ status: 'ok', service: 'getsignedurl' })
);

// ------------------ Main Endpoint ------------------
app.post('/getsignedurl', async (req, res) => {
  try {
    /* ---------- Required metadata ---------- */
    const filename = req.body.filename || 'audio.wav';
    const deviceId = sanitize(req.body.deviceId, 'esp32');
    const courseCode = sanitize(req.body.course_code, 'GENERAL');
    const level = sanitize(req.body.level, '000');
    const resumable = req.body.resumable === true;

    /* ---------- Recording identity ---------- */
    const recordingId =
      req.body.recordingId || makeRecordingId(deviceId);

    /* ---------- Object path ---------- */
    const objectPath =
      `lectures/${level}/${courseCode}/${recordingId}/${filename}`;

    const file = storage.bucket(BUCKET).file(objectPath);

    /* ---------- Resumable upload ---------- */
    if (resumable) {
      const [resumableUrl] = await file.createResumableUpload({
        origin: '*',
        metadata: {
          contentType: 'audio/wav',
          metadata: {
            course_code: courseCode,
            level: String(level),
            device_id: deviceId,
            recording_id: recordingId
          }
        }
      });

      return res.json({
        recordingId,
        objectPath,
        resumableUrl
      });
    }

    /* ---------- Simple signed URL ---------- */
    const expiresMs = Date.now() + 24 * 60 * 60 * 1000;
    const [uploadUrl] = await file.getSignedUrl({
      version: 'v4',
      action: 'write',
      expires: expiresMs,
      contentType: 'audio/wav'
    });

    return res.json({
      recordingId,
      objectPath,
      uploadUrl
    });

  } catch (err) {
    console.error('getSignedUrl error', err);
    return res.status(500).json({
      error: 'Could not generate signed URL',
      detail: err.message
    });
  }
});

// ------------------ Server ------------------
const PORT = process.env.PORT || 8080;
app.listen(PORT, () =>
  console.log(`Listening on ${PORT}`)
);
