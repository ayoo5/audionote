# Backend Deployment Guide (GUI-Based GCP Workflow)

This section documents how to deploy the Audionote backend infrastructure primarily using the Google Cloud Platform graphical interface (GUI), with minimal command-line usage.

This workflow is especially useful for:

* students,
* first-time cloud developers,
* embedded systems developers,
* and users unfamiliar with DevOps tooling.

The backend system is responsible for:

* generating secure upload URLs,
* receiving uploaded lecture recordings,
* triggering automatic transcription,
* organizing transcripts,
* and integrating with Google Shared Drive.

---

# Backend Architecture Overview

```text
ESP32 Device
        ↓
Signed URL Backend
        ↓
Google Cloud Storage
        ↓
Auto-Transcription Backend
        ↓
Google Speech-to-Text
        ↓
Google Shared Drive
```

---

# Before You Begin

Ensure the following stages are already completed:

* Firmware setup
* Hardware assembly
* Audio recording validation
* Upload system validation
* Power system integration

Continue from:

→ [Power System Guide](../02-hardware/power-system.md)

---

# Recommended Accounts & Requirements

| Requirement                                  | Purpose               |
| -------------------------------------------- | --------------------- |
| Google Account                               | GCP authentication    |
| Debit/Card Payment Method                    | Billing verification  |
| Internet Connection                          | Backend deployment    |
| Node.js (Optional)                           | Local backend testing |
| GitHub Repository / Downloaded Backend Files | Backend source code   |

---

# Stage 1 — Create a Google Cloud Account

Open:

* [Google Cloud Console](https://console.cloud.google.com/?utm_source=chatgpt.com)

Sign in using your Google account.

---

# Google Cloud Free Credits

New Google Cloud accounts usually receive promotional credits for development and testing.

Learn more:

* [Google Cloud Free Tier & Credits](https://cloud.google.com/free?utm_source=chatgpt.com)

Important:

* Billing must still be enabled
* Usage quotas still apply

To avoid unexpected charges:

* use short recordings during testing,
* monitor storage usage,
* and configure billing alerts.

Billing alerts guide:

* [Google Cloud Budget Alerts Documentation](https://cloud.google.com/billing/docs/how-to/budgets?utm_source=chatgpt.com)

---

# Stage 2 — Create a Google Cloud Project

Inside the Google Cloud Console:

1. Click the project selector at the top navigation bar
2. Click:

   ```text
   NEW PROJECT
   ```
3. Enter a project name

Example:

```text
audionote-backend
```

4. Click:

   ```text
   CREATE
   ```

Wait for the project to initialize.

---

# Stage 3 — Enable Billing

Cloud Run and Speech-to-Text require billing to be enabled.

Open:

* [Google Cloud Billing Console](https://console.cloud.google.com/billing?utm_source=chatgpt.com)

Then:

1. Link a billing account
2. Add payment method
3. Attach billing account to your project

---

# Stage 4 — Enable Required APIs

Open:

* [Google Cloud API Library](https://console.cloud.google.com/apis/library?utm_source=chatgpt.com)

Enable the following APIs:

| API                | Purpose                    |
| ------------------ | -------------------------- |
| Cloud Run API      | Backend hosting            |
| Cloud Storage API  | File storage               |
| Speech-to-Text API | Audio transcription        |
| Google Drive API   | Shared Drive integration   |
| IAM API            | Permissions management     |
| Eventarc API       | Storage trigger automation |

---

# How to Enable an API

For each API:

1. Search the API name
2. Open the API page
3. Click:

   ```text
   ENABLE
   ```

Repeat for all required APIs.

---

# Stage 5 — Create the Primary Storage Bucket

The primary storage bucket stores uploaded WAV lecture recordings.

Open:

* [Google Cloud Storage Console](https://console.cloud.google.com/storage/browser?utm_source=chatgpt.com)

---

# Create Bucket

1. Click:

   ```text
   CREATE
   ```
2. Enter bucket name

Example:

```text
audionote-recordings
```

Bucket names must be globally unique.

---

# Recommended Bucket Settings

| Setting        | Recommendation |
| -------------- | -------------- |
| Access Control | Uniform        |
| Storage Class  | Standard       |
| Location Type  | Region         |
| Public Access  | Disabled       |

---

# Recommended Folder Structure

```text
lectures/
    └── level/
          └── course/
                └── session/
```

---

# Stage 6 — Create the Transcript Bucket (Recommended)

This bucket stores:

* generated transcripts,
* processed outputs,
* and exported text files.

---

# Create Second Bucket

Repeat the bucket creation process.

Example:

```text
audionote-transcripts
```

---

# Recommended Transcript Structure

```text
transcripts/
    └── level/
          └── course/
                └── session/
                      ├── transcript.txt
                      ├── metadata.json
                      └── summary.txt
```

---

# Stage 7 — Obtain the Backend Source Code

You can either:

* clone the repository,
* or manually upload the backend folders.

---

# Option 1 — Clone Repository

```bash
git clone https://github.com/yourusername/lecture-transcription-iot.git
```

---

# Option 2 — Manual File Upload

Required folders:

```text
backend/
├── signedurl/
└── autotranscribe/
```

Each folder should contain:

```text
index.js
package.json
```

---

# Stage 8 — Open Google Cloud Shell Editor

Open:

* [Google Cloud Shell](https://shell.cloud.google.com/?utm_source=chatgpt.com)

Then open the Cloud Shell Editor.

Cloud Shell Editor documentation:

* [Google Cloud Shell Editor Overview](https://cloud.google.com/shell/docs/editor-overview?utm_source=chatgpt.com)

---

# Stage 9 — Upload Backend Files

Inside Cloud Shell Editor:

1. Create folders:

```text
backend/
├── signedurl/
└── autotranscribe/
```

2. Upload:

* index.js
* package.json
* additional config files

using drag-and-drop or upload menu.

---

# Stage 10 — Install Backend Dependencies

Open the Cloud Shell terminal.

Navigate into the signed URL service:

```bash
cd backend/signedurl
```

Install dependencies:

```bash
npm install
```

Repeat for the transcription backend:

```bash
cd ../autotranscribe
npm install
```

---

# Stage 11 — Deploy the Signed URL Backend

Navigate into:

```bash
backend/signedurl
```

Then deploy using Cloud Run:

```bash
gcloud run deploy signedurl-service \
  --source . \
  --region us-central1 \
  --allow-unauthenticated
```

---

# Deployment Configuration

During deployment:

| Prompt                | Recommendation |
| --------------------- | -------------- |
| Region                | us-central1    |
| Allow unauthenticated | Yes            |
| Build source          | Yes            |

---

# Expected Result

After deployment:

```text
Service URL:
https://signedurl-service-xxxxx.run.app
```

Save this URL.

---

# Stage 12 — Configure ESP32 Backend Endpoint

Update firmware:

```cpp
#define SIGNED_URL_ENDPOINT "https://your-cloud-run-url.run.app"
```

Then re-upload firmware to the ESP32.

---

# Stage 13 — Deploy the Auto-Transcription Backend

Navigate into:

```bash
backend/autotranscribe
```

Deploy:

```bash
gcloud run deploy autotranscribe-service \
  --source . \
  --region us-central1 \
  --allow-unauthenticated
```

---

# Stage 14 — Configure Storage Trigger Automation

The transcription service should trigger automatically when WAV files are uploaded.

---

# Open Eventarc

Open:

* [Google Eventarc Console](https://console.cloud.google.com/eventarc?utm_source=chatgpt.com)

---

# Create Trigger

1. Click:

   ```text
   CREATE TRIGGER
   ```

2. Configure:

| Setting        | Recommendation         |
| -------------- | ---------------------- |
| Event Provider | Cloud Storage          |
| Event Type     | Object Finalized       |
| Bucket         | audionote-recordings   |
| Destination    | Cloud Run              |
| Service        | autotranscribe-service |

---

# Trigger Workflow

```text
ESP32 Upload
        ↓
Storage Bucket
        ↓
Eventarc Trigger
        ↓
Auto-Transcription Backend
        ↓
Transcript Generation
```

---

# Stage 15 — Configure IAM Permissions

Open:

* [Google Cloud IAM Console](https://console.cloud.google.com/iam-admin/iam?utm_source=chatgpt.com)

---

# Recommended Roles

| Role                 | Purpose              |
| -------------------- | -------------------- |
| Storage Admin        | Bucket access        |
| Cloud Run Admin      | Backend management   |
| Speech-to-Text User  | Transcription access |
| Service Account User | Service execution    |

---

# Stage 16 — Configure Speech-to-Text Processing

Audionote currently uses:

* post-recording batch transcription.

The backend:

1. receives uploaded WAV files,
2. processes transcription asynchronously,
3. stores transcript outputs.

Speech-to-Text documentation:

* [Google Speech-to-Text Documentation](https://cloud.google.com/speech-to-text/docs?utm_source=chatgpt.com)

---

# Recommended Audio Configuration

| Parameter    | Recommendation               |
| ------------ | ---------------------------- |
| Encoding     | LINEAR16                     |
| Audio Format | WAV                          |
| Channels     | Mono                         |
| Sample Rate  | Match firmware configuration |

---

# Stage 17 — Validate the Full Backend Pipeline

Perform a short recording test.

Expected workflow:

```text
ESP32 Recording
        ↓
Upload to Bucket
        ↓
Signed URL Validation
        ↓
Storage Trigger Fires
        ↓
Transcription Backend Runs
        ↓
Transcript Generated
```

---

# Recommended Validation Checklist

| Validation Item      | Expected Result |
| -------------------- | --------------- |
| Upload succeeds      | Successful      |
| WAV stored in bucket | Successful      |
| Trigger activates    | Successful      |
| Transcript generated | Successful      |
| No backend crashes   | Stable          |

---

# Common Backend Issues

| Problem                  | Possible Cause           | Suggested Fix                |
| ------------------------ | ------------------------ | ---------------------------- |
| Upload rejected          | Invalid signed URL       | Verify endpoint              |
| Trigger not firing       | Incorrect Eventarc setup | Reconfigure trigger          |
| STT failure              | API disabled             | Enable Speech API            |
| Permission denied        | IAM issue                | Verify service account roles |
| Backend deployment fails | Billing disabled         | Enable billing               |
| Cloud Run timeout        | Large file processing    | Optimize processing pipeline |

---

# Recommended Next Step

Once the backend deployment and automation pipeline are validated successfully, proceed to Shared Drive integration.

Continue to:

→ [Shared Drive Setup Guide](shared-drive-integration.md)

Then:

→ [Final System Integration](../05-system-integration/full-system-integration.md)
