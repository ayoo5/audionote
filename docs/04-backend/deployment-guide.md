# Backend Deployment Guide (CLI-heavy)

This section documents the deployment and configuration workflow for the Audionote cloud backend infrastructure.

The backend system is responsible for:

* generating secure upload URLs,
* receiving uploaded lecture recordings,
* triggering automatic transcription,
* organizing recordings,
* and integrating with Google Shared Drive for archival.

Audionote primarily uses:

* Google Cloud Run,
* Google Cloud Storage,
* Google Speech-to-Text,
* Google Drive API,
* and Google Workspace infrastructure.

At the end of this section, you should have:

* a deployed cloud backend,
* working upload endpoints,
* automated transcription processing,
* and a validated storage pipeline ready for Shared Drive integration.

---

# Backend Architecture Overview

```text id="g8ny8p"
ESP32 Device
        ↓
Signed URL Backend
        ↓
Google Cloud Storage
        ↓
Auto-Transcription Service
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

# Backend Components

| Service            | Purpose                  |
| ------------------ | ------------------------ |
| Cloud Run          | Backend service hosting  |
| Cloud Storage      | Audio file storage       |
| Speech-to-Text API | Audio transcription      |
| Google Drive API   | Shared Drive integration |
| IAM                | Access management        |
| Cloud Logging      | Monitoring and debugging |

---

# Recommended Knowledge

Basic familiarity with:

* JavaScript/Node.js,
* terminal usage,
* and Google Cloud Platform

is recommended. I got by with none so don't worry. 😃

---

# Google Cloud Free Credits for New Accounts

Google Cloud typically provides free credits for new accounts which can be used during development and testing.

At the time of writing, new users may receive promotional credits after:
- creating a Google Cloud account,
- enabling billing,
- and verifying payment information.

This is usually sufficient for:
- Cloud Run deployment,
- Storage usage,
- Speech-to-Text testing,
- and backend experimentation during development.

Learn more:

- :contentReference[oaicite:0]{index=0}

---

# Important Note About Billing

Even when using free credits:

- billing must still be enabled,
- APIs may still require activation,
- and usage limits still apply.

To avoid unexpected charges:
- monitor usage regularly,
- configure billing alerts,
- and test with short recordings initially.

---

# Stage 1 — Create a Google Cloud Project

Create a new Google Cloud project.

Open:

* [Google Cloud Console](https://console.cloud.google.com/?utm_source=chatgpt.com)

---

# Create a New Project

1. Open the Google Cloud Console
2. Click project selector
3. Create a new project
4. Give the project a name

Example:

```text id="0llc5p"
audionote-backend
```

---

# Enable Billing

Cloud Run and Speech-to-Text require billing to be enabled.

Billing setup:

* [Google Cloud Billing Setup Guide](https://cloud.google.com/billing/docs/how-to/modify-project?utm_source=chatgpt.com)

---

# Stage 2 — Install Google Cloud CLI

Install the Google Cloud SDK locally.

Official installation guide:

* [Google Cloud CLI Installation Guide](https://cloud.google.com/sdk/docs/install?utm_source=chatgpt.com)

---

# Verify Installation

Run:

```bash id="nq3d2l"
gcloud --version
```

Expected output:

* installed SDK version information

---

# Authenticate Locally

Run:

```bash id="drm2c4"
gcloud auth login
```

Then:

```bash id="djlwm4"
gcloud config set project YOUR_PROJECT_ID
```

Replace the place holder with your project ID. You can find this in your project selector or even i the CLI.

---

# Stage 3 — Enable Required APIs

Enable the following APIs:

| API                | Purpose                 |
| ------------------ | ----------------------- |
| Cloud Run API      | Backend hosting         |
| Cloud Storage API  | File storage            |
| Speech-to-Text API | Audio transcription     |
| Drive API          | Shared Drive automation |
| IAM API            | Permission management   |

---

# Enable APIs Using CLI

Run:

```bash id="4n2cn0"
gcloud services enable run.googleapis.com
```

```bash id="t1d7l4"
gcloud services enable speech.googleapis.com
```

```bash id="7g0btx"
gcloud services enable storage.googleapis.com
```

```bash id="v8r9m4"
gcloud services enable drive.googleapis.com
```

```bash id="3k6kbo"
gcloud services enable iam.googleapis.com
```

---

# Additional API Setup Reference

* [Google Cloud API Enablement Guide](https://cloud.google.com/apis/docs/getting-started?utm_source=chatgpt.com)

---

# Stage 4 — Create Cloud Storage Bucket

The storage bucket stores uploaded WAV recordings before transcription processing.

---

# Create Bucket

Open:

* [Google Cloud Storage Console](https://console.cloud.google.com/storage?utm_source=chatgpt.com)

Create a new bucket.

Example naming:

```text id="3xjzj9"
audionote-recordings
```

---

# Recommended Bucket Settings

| Setting        | Recommendation |
| -------------- | -------------- |
| Access Control | Uniform        |
| Location Type  | Region         |
| Storage Class  | Standard       |

---

# Bucket Structure Recommendation

```text id="2e5u1m"
lectures/
    └── level/
          └── course/
                └── recording-session/
```

---

# Stage 4.5 — Obtain the Backend Source Code

Before deployment, ensure the Audionote backend source code is available locally.

You can either:

- clone the full repository,
- or download the repository ZIP or specific files/folders from GitHub.

---

# Clone the Repository

```bash
git clone https://github.com/yourusername/lecture-transcription-iot.git
```

Then navigate into the repository:

```bash
cd lecture-transcription-iot
```

---

# Alternative: Manual Backend File Upload

If you do not want to clone the full repository, you can manually download the backend folders and upload them into your Google Cloud environment.

Required backend folders:

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

and any additional configuration files used in the project.

---

# Recommended Upload Methods

## Option 1 — Upload Using Google Cloud Shell

Open:

* [Google Cloud Shell](https://console.cloud.google.com/cloudshell?utm_source=chatgpt.com)

Then:

1. Create backend directories:

```bash
mkdir -p backend/signedurl
mkdir -p backend/autotranscribe
```

2. Upload files manually using the Cloud Shell upload button

3. Verify uploaded files:

```bash
ls backend/signedurl
ls backend/autotranscribe
```

---

## Option 2 — Upload Using Cloud Shell Editor

Open:

* [Google Cloud Shell](https://console.cloud.google.com/shell?utm_source=chatgpt.com)

Then:

1. Open the Cloud Shell Editor
2. Create backend folders manually
3. Drag and drop the backend files into the editor workspace
4. Open terminal and install dependencies

---

# Recommended Validation

Before deployment, verify:

| Validation Item | Expected Result |
|---|---|
| index.js exists | Yes |
| package.json exists | Yes |
| npm install completes | Successful |
| No missing dependencies | Successful |

---

# Backend Directory Structure

```text
backend/
├── signedurl/
│     ├── index.js
│     └── package.json
│
└── autotranscribe/
      ├── index.js
      └── package.json
```

---

# Backend Services Overview

| Service | Purpose |
|---|---|
| signedurl | Generates secure upload URLs for ESP32 uploads |
| autotranscribe | Processes uploaded WAV files and generates transcripts |

---

# Recommended Validation

Before deployment:

1. Verify all backend files exist
2. Verify Node.js is installed
3. Verify npm is installed

Check versions:

```bash
node --version
npm --version
```

---

# Install Node.js (If Necessary)

Official download page:

- :contentReference[oaicite:2]{index=2}

Recommended:
- Use the latest LTS release

---

# Stage 5 — Deploy Signed URL Backend

This backend service:

* authenticates upload requests,
* generates signed upload URLs,
* and initializes upload sessions.

---

# Navigate to Backend Directory

```bash id="zqrf92"
cd backend/signedurl
```

---

# Install Dependencies

```bash id="7fgrho"
npm install
```

---

# Recommended Environment Variables

Create:

```text id="9z7z8m"
.env
```

Example:

```env id="qewn8r"
BUCKET_NAME=audionote-recordings
GOOGLE_CLOUD_PROJECT=your-project-id
```

---

# Deploy to Cloud Run

Example deployment command:

```bash id="m1j4hj"
gcloud run deploy signedurl-service \
  --source . \
  --region us-central1 \ 
  --allow-unauthenticated
```

For builders within Nigeria, the nearest region server is in "europe-west1". Replace the "us-central1" with this.

---

# Cloud Run Documentation

* [Cloud Run Deployment Guide](https://cloud.google.com/run/docs/deploying-source-code?utm_source=chatgpt.com)

---

# Validation Checkpoint

Expected result:

* Cloud Run deployment URL generated

Example:

```text id="0s0icn"
https://signedurl-service-xxxxx.run.app
```

Test endpoint accessibility in browser or Postman.

---

# Stage 6 — Configure ESP32 Upload Endpoint

Update firmware configuration:

```cpp id="8jm1j0"
const char *backendUrl = "https://your-cloud-run-url.run.app";  // Your GCF endpoint
```

Re-upload firmware after modification.

---

# Stage 7 — Deploy Auto-Transcription Backend

The auto-transcription backend:

* detects uploaded audio,
* calls Speech-to-Text,
* processes transcripts,
* and prepares archival outputs.

---

# Navigate to Backend Directory

```bash id="6e5g2s"
cd backend/autotranscribe
```

---

# Install Dependencies

```bash id="k9xwob"
npm install
```

---

# Recommended Environment Variables

Example:

```env id="5snfow"
BUCKET_NAME=audionote-recordings
GOOGLE_CLOUD_PROJECT=your-project-id
```

---

# Deploy Service

```bash id="wyxjlwm"
gcloud run deploy autotranscribe-service \
  --source . \
  --region europe-west1 \
  --allow-unauthenticated
```

---

# Stage 8 — Configure Cloud Storage Trigger

The transcription backend should trigger automatically when a WAV file is uploaded.

Recommended trigger workflow:

```text id="iyvdrx"
Cloud Storage Upload
        ↓
Trigger Event
        ↓
Auto-Transcription Backend
```

---

# Eventarc / Trigger Documentation

* [Google Eventarc Documentation](https://cloud.google.com/eventarc/docs?utm_source=chatgpt.com)

---

# Stage 9 — Configure IAM Permissions

Proper IAM permissions are required for:

* Cloud Run,
* Storage access,
* Speech-to-Text access,
* and Drive integration.

---

# Recommended Service Account Permissions

| Role                 | Purpose            |
| -------------------- | ------------------ |
| Storage Admin        | Bucket access      |
| Cloud Run Admin      | Service management |
| Speech Client        | STT access         |
| Service Account User | Service execution  |

---

# IAM Documentation

* [Google Cloud IAM Documentation](https://cloud.google.com/iam/docs?utm_source=chatgpt.com)

---

# Stage 10 — Configure Speech-to-Text Processing

Audionote currently uses:

* post-recording batch transcription.

This means:

* the ESP32 uploads WAV recordings,
* then backend services process them asynchronously.

---

# Speech-to-Text Documentation

* [Google Speech-to-Text Documentation](https://cloud.google.com/speech-to-text/docs?utm_source=chatgpt.com)

---

# Recommended Audio Settings

| Parameter   | Recommendation               |
| ----------- | ---------------------------- |
| Encoding    | LINEAR16                     |
| Format      | WAV                          |
| Channels    | Mono                         |
| Sample Rate | Match firmware configuration |

---

# Validation Checkpoint

Expected results:

| Validation Item      | Expected Result |
| -------------------- | --------------- |
| Upload succeeds      | Successful      |
| WAV stored in bucket | Successful      |
| Transcription starts | Successful      |
| Transcript generated | Successful      |

---

# Stage 11 — Cloud Logging & Monitoring

Cloud logging is extremely useful during debugging.

Open:

* [Google Cloud Logging Console](https://console.cloud.google.com/logs?utm_source=chatgpt.com)

Useful for:

* upload debugging,
* backend failures,
* trigger issues,
* authentication failures,
* and STT errors.

---

# Common Backend Issues

| Problem                    | Possible Cause           | Suggested Fix                |
| -------------------------- | ------------------------ | ---------------------------- |
| Upload rejected            | Invalid signed URL       | Verify backend endpoint      |
| Cloud Run deployment fails | Billing disabled         | Enable billing               |
| STT fails                  | API not enabled          | Enable Speech API            |
| Trigger not firing         | Incorrect Eventarc setup | Verify trigger configuration |
| Permission denied          | IAM misconfiguration     | Verify service account roles |
| Upload timeout             | Large file/chunk size    | Reduce upload chunk size     |

---

# Backend Validation Checklist

| Validation Item         | Expected Result |
| ----------------------- | --------------- |
| Cloud Run deployed      | Successful      |
| Signed URLs generated   | Successful      |
| Upload reaches bucket   | Successful      |
| WAV files stored        | Successful      |
| Transcription generated | Successful      |
| Logs visible            | Successful      |

---

# Recommended Next Step

Once backend deployment and transcription processing are validated successfully, proceed to Shared Drive integration and automated archival setup.

Continue to:

→ [Shared Drive Setup Guide](shared-drive-integration.md)

Then:

→ [Final System Integration](../05-system-integration/full-system-integration.md)

---

# Additional Resources

## Cloud Run Documentation

* [Google Cloud Run Documentation](https://cloud.google.com/run/docs?utm_source=chatgpt.com)

## Cloud Storage Documentation

* [Google Cloud Storage Documentation](https://cloud.google.com/storage/docs?utm_source=chatgpt.com)

## Speech-to-Text Documentation

* [Google Speech-to-Text Documentation](https://cloud.google.com/speech-to-text/docs?utm_source=chatgpt.com)

## Google Drive API Documentation

* [Google Drive API Documentation](https://developers.google.com/drive?utm_source=chatgpt.com)

## Eventarc Documentation

* [Google Eventarc Documentation](https://cloud.google.com/eventarc/docs?utm_source=chatgpt.com)
