# Shared Drive Setup Guide

This section documents how to configure Google Shared Drive integration for the Audionote system.

The Shared Drive integration enables:

* centralized lecture archival,
* collaborative access,
* automated transcript organization,
* and long-term cloud accessibility.

At the end of this section, you should have:

* a working Google Workspace environment,
* a configured Shared Drive,
* proper folder structures,
* backend integration access,
* and automated transcript archival readiness.

---

# Why Shared Drive Integration Exists

Audionote is designed not only to:

* record lectures,
* and generate transcripts,

but also to:

* organize educational content automatically,
* simplify lecture accessibility,
* and improve collaboration between users.

Google Shared Drive was selected because it:

* supports collaborative file ownership,
* integrates well with Google APIs,
* supports large-scale organization,
* and is widely accessible in academic environments.

---

# Shared Drive Architecture Overview

```text id="qbpjlwm"
ESP32 Device
        ↓
Cloud Upload
        ↓
Transcription Backend
        ↓
Google Shared Drive
        ↓
Automated Folder Organization
```

---

# Before You Begin

Ensure the following stages are already completed:

* Firmware setup
* Hardware assembly
* Backend deployment
* Cloud Storage setup
* Transcription backend validation

Continue from:

→ [Backend Deployment Guide](deployment-guide.md)

or

→ [GUI Backend Deployment Guide](deployment-gui-based.md)

---

# Recommended Requirements

| Requirement              | Purpose              |
| ------------------------ | -------------------- |
| Google Workspace Account | Shared Drive support |
| Custom Domain Name       | Workspace setup      |
| Google Cloud Project     | API integration      |
| Google Drive API Enabled | Backend integration  |

---

# Important Note About Google Shared Drives

Google Shared Drives are generally not available on standard personal Google accounts.

A Google Workspace account is typically required.

Learn more:

* [Google Workspace Overview](https://workspace.google.com/?utm_source=chatgpt.com)

---

# Stage 1 — Purchase a Domain Name

Google Workspace setup requires a custom domain name.

Example:

```text id="lb6ijv"
yourorganization.com
```

---

# Recommended Domain Providers

| Provider                             | Notes                        |
| ------------------------------------ | ---------------------------- |
| Namecheap                            | Beginner-friendly            |
| Cloudflare                           | Strong DNS management        |
| Google Domains (availability varies) | Google ecosystem integration |

---

# Domain Provider Links

* [Namecheap](https://www.namecheap.com/?utm_source=chatgpt.com)
* [Cloudflare Domains](https://www.cloudflare.com/products/registrar/?utm_source=chatgpt.com)

---

# Domain Selection Recommendations

Choose:

* short,
* readable,
* memorable,
* and professional domain names.

Example:

```text id="s1m1mg"
audionote-project.com
```

---

# Stage 2 — Create a Google Workspace Account

Open:

* [Google Workspace Signup](https://workspace.google.com/signup/businessstarter/welcome?utm_source=chatgpt.com)

---

# Workspace Setup Process

During setup:

1. Enter organization details
2. Enter purchased domain name
3. Create administrator account
4. Configure billing
5. Verify domain ownership

---

# Stage 3 — Verify Your Domain

Google Workspace requires DNS verification.

This proves ownership of the purchased domain.

---

# Typical Verification Process

Google provides:

* TXT records,
* or CNAME records

to add to your domain provider DNS settings.

---

# DNS Configuration Reference

* [Google Workspace Domain Verification Guide](https://support.google.com/a/answer/60216?utm_source=chatgpt.com)

---

# Important Note

DNS propagation may take:

* several minutes,
* or several hours.

This is normal.

---

# Stage 4 — Create a Shared Drive

Open Google Drive:

* [Google Drive](https://drive.google.com/?utm_source=chatgpt.com)

---

# Create Shared Drive

1. Open left sidebar
2. Click:

   ```text
   Shared drives
   ```
3. Click:

   ```text
   New
   ```

---

# Example Shared Drive Name

```text id="k1b7ob"
Audionote Archive
```

---

# Stage 5 — Enable Google Drive API

Open:

* [Google Drive API Console](https://console.cloud.google.com/apis/library/drive.googleapis.com?utm_source=chatgpt.com)

Click:

```text
ENABLE
```

---

# Stage 6 — Create Service Account

The backend requires a service account to interact with Google Drive programmatically.

Open:

* [Google Cloud Service Accounts Console](https://console.cloud.google.com/iam-admin/serviceaccounts?utm_source=chatgpt.com)

---

# Create Service Account

1. Click:

   ```text
   CREATE SERVICE ACCOUNT
   ```
2. Enter service account name

Example:

```text id="e1f6dg"
audionote-drive-service
```

3. Continue

---

# Recommended Permissions

| Role              | Purpose             |
| ----------------- | ------------------- |
| Drive API Access  | Drive operations    |
| Storage Admin     | Bucket access       |
| Cloud Run Invoker | Backend integration |

---

# Stage 7 — Generate Service Account Credentials

Inside the service account:

1. Open:

   ```text
   KEYS
   ```
2. Click:

   ```text
   ADD KEY
   ```
3. Select:

   ```text
   JSON
   ```

Download the credentials file securely.

---

# Important Security Warning

Do NOT:

* upload service account keys publicly,
* commit them to GitHub,
* or expose them in firmware.

Add credential files to:

```text id="2o6jqa"
.gitignore
```

Example:

```text id="v0aj5z"
*.json
.env
```

---

# Stage 8 — Share the Shared Drive With the Service Account

The backend service account must be granted access to the Shared Drive.

---

# Find Service Account Email

Example:

```text id="9glht9"
audionote-drive-service@your-project.iam.gserviceaccount.com
```

---

# Add to Shared Drive

Inside Google Drive:

1. Open Shared Drive
2. Click:

   ```text
   Manage members
   ```
3. Add service account email
4. Assign:

   ```text
   Content Manager
   ```

or appropriate permissions.

---

# Why This Step Is Important

Without Shared Drive access:

* backend uploads will fail,
* transcript organization will fail,
* and Drive automation will not function properly.

---

# Stage 9 — Retrieve Shared Drive ID

The backend may require the Shared Drive ID.

---

# How to Find Drive ID

Open the Shared Drive in browser.

Example URL:

```text id="4nmef7"
https://drive.google.com/drive/folders/SHARED_DRIVE_ID
```

Copy:

```text
SHARED_DRIVE_ID
```

---

# Example Environment Variable

```env id="drggfq"
SHARED_DRIVE_ID=xxxxxxxxxxxxxxxx
```

---

# Stage 10 — Configure Backend Environment Variables

Recommended variables:

```env id="guzm9y"
DRIVE_FOLDER_ID=xxxxxxxx
SHARED_DRIVE_ID=xxxxxxxx
GOOGLE_APPLICATION_CREDENTIALS=service-account.json
```

---

# Stage 11 — Backend Integration Workflow

The backend can now:

* upload transcripts,
* create folders,
* organize recordings,
* and archive metadata automatically.

---

# Example Workflow

```text id="4v7dgm"
Lecture Uploaded
        ↓
Transcription Generated
        ↓
Folder Automatically Created
        ↓
Transcript Uploaded to Shared Drive
```

---

# Recommended Folder Automation Structure

```text id="y2spqh"
Department
    └── Course
          └── Session
                ├── lecture.wav
                ├── transcript.txt
                └── metadata.json
```

---

# Stage 12 — Validate Integration

Perform a short recording upload test.

Expected result:

| Validation Item             | Expected Result |
| --------------------------- | --------------- |
| WAV upload succeeds         | Successful      |
| Transcript generated        | Successful      |
| Shared Drive folder created | Successful      |
| Transcript uploaded         | Successful      |
| File organization works     | Successful      |

---

# Common Shared Drive Issues

| Problem                           | Possible Cause                 | Suggested Fix          |
| --------------------------------- | ------------------------------ | ---------------------- |
| Permission denied                 | Service account missing access | Share drive properly   |
| Folder creation fails             | Incorrect Drive ID             | Verify IDs             |
| API errors                        | Drive API disabled             | Enable API             |
| Upload succeeds but no Drive file | Backend integration issue      | Check logs             |
| Credentials rejected              | Invalid JSON key               | Regenerate credentials |

---

# Reading Backend Logs

Useful debugging command:

```bash
gcloud functions logs read autoTranscribeOnUpload --gen2 --region=europe-west1
```

Useful for diagnosing:

* Drive integration failures,
* permission issues,
* and backend upload errors.

---

# Cloud Logging Console

* [Google Cloud Logging Console](https://console.cloud.google.com/logs?utm_source=chatgpt.com)

---

# Important Engineering Note

Shared Drive integration is one of the most powerful aspects of the Audionote system because it transforms:

* isolated lecture recordings

into:

* searchable,
* organized,
* collaborative academic archives.

This stage combines:

* cloud infrastructure,
* authentication systems,
* API integration,
* and backend automation.

Expect:

* debugging,
* iteration,
* and gradual refinement during development.

Patience and incremental testing are important.

---

# Recommended Best Practices

| Best Practice                    | Why It Helps         |
| -------------------------------- | -------------------- |
| Use test folders first           | Safer debugging      |
| Validate permissions carefully   | Prevent API failures |
| Keep credentials secure          | Security             |
| Use incremental uploads          | Easier debugging     |
| Test with short recordings first | Faster iteration     |

---

# Recommended Next Step

Once Shared Drive integration is validated successfully, proceed to full system integration and enclosure assembly.

Continue to:

→ [Full System Integration Guide](../05-system-integration/full-system-integration.md)

Then:

→ [Enclosure & Mechanical Integration](../06-enclosure/enclosure-guide.md)

---

# Additional Resources

## Google Workspace Documentation

* [Google Workspace Documentation](https://support.google.com/a?utm_source=chatgpt.com)

## Google Drive API Documentation

* [Google Drive API Documentation](https://developers.google.com/drive?utm_source=chatgpt.com)

## Google Cloud IAM Documentation

* [Google Cloud IAM Documentation](https://cloud.google.com/iam/docs?utm_source=chatgpt.com)

## Google Service Accounts Documentation

* [Google Service Accounts Documentation](https://cloud.google.com/iam/docs/service-accounts?utm_source=chatgpt.com)
