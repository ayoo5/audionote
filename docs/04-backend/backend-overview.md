# Backend Overview

This document explains the architecture, design philosophy, and operational flow of the Audionote backend infrastructure.

The backend system is responsible for:

* securely receiving uploaded lecture recordings,
* processing transcription requests,
* organizing generated outputs,
* and integrating with Google Shared Drive for archival and accessibility.

Unlike traditional IoT systems that process data locally, Audionote utilizes a cloud-assisted backend architecture to overcome the computational and memory limitations of embedded hardware platforms such as the ESP32.

---

# Why the Backend Exists

The ESP32 is excellent for:

* audio acquisition,
* local storage,
* wireless communication,
* and embedded control.

However, advanced speech recognition workloads require:

* high compute capability,
* scalable processing,
* cloud APIs,
* and persistent storage infrastructure.

The backend bridges this gap by enabling:

* scalable transcription,
* centralized organization,
* remote accessibility,
* and automated processing workflows.

---

# Backend Responsibilities

The Audionote backend performs several major functions:

| Function                 | Purpose                     |
| ------------------------ | --------------------------- |
| Signed URL Generation    | Secure upload authorization |
| Cloud Storage Management | Lecture audio storage       |
| Automatic Transcription  | Speech-to-text processing   |
| File Organization        | Structured archival         |
| Shared Drive Integration | Multi-user accessibility    |
| Trigger Automation       | Event-driven processing     |
| Logging & Monitoring     | Debugging and observability |

---

# High-Level Backend Architecture

```text id="x2mhyt"
ESP32 Device
        ↓
Signed URL Backend
        ↓
Google Cloud Storage
        ↓
Storage Trigger
        ↓
Auto-Transcription Backend
        ↓
Google Speech-to-Text
        ↓
Transcript Processing
        ↓
Google Shared Drive
```

---

# Why Signed URLs Were Used

Audionote uses signed URLs instead of directly exposing storage buckets to the ESP32.

This approach improves:

* security,
* scalability,
* access control,
* and backend flexibility.

---

# Signed URL Workflow

```text id="tzqv7u"
ESP32 Requests Upload Permission
                ↓
Backend Generates Temporary Signed URL
                ↓
ESP32 Uploads WAV File Securely
                ↓
Upload Permission Expires Automatically
```

---

# Advantages of Signed URLs

| Advantage                 | Benefit                   |
| ------------------------- | ------------------------- |
| Temporary Access          | Improved security         |
| No Direct Bucket Exposure | Reduced attack surface    |
| Controlled Uploads        | Better backend management |
| Scalable Architecture     | Easier future expansion   |

---

# Backend Services

The backend currently consists of two primary services.

---

# 1. Signed URL Service

Directory:

```text id="9m2m9g"
backend/signedurl
```

This service:

* receives upload initialization requests,
* authenticates requests,
* generates temporary upload URLs,
* and prepares cloud storage upload sessions.

---

# Responsibilities

| Responsibility           | Purpose              |
| ------------------------ | -------------------- |
| Generate signed URLs     | Secure uploads       |
| Validate requests        | Upload control       |
| Configure upload targets | Storage organization |

---

# Typical Flow

```text id="0r5m73"
ESP32 Stops Recording
        ↓
ESP32 Detects Wi-Fi
        ↓
ESP32 Requests Upload URL
        ↓
Backend Returns Signed URL
        ↓
ESP32 Uploads Recording
```

---

# 2. Auto-Transcription Service

Directory:

```text id="gtx69j"
backend/autotranscribe
```

This service:

* monitors uploaded recordings,
* triggers transcription processing,
* processes transcript outputs,
* and organizes generated files.

---

# Responsibilities

| Responsibility             | Purpose               |
| -------------------------- | --------------------- |
| Detect uploaded files      | Event automation      |
| Invoke Speech-to-Text      | Transcript generation |
| Process transcript outputs | Organization          |
| Integrate with Drive       | Accessibility         |

---

# Transcription Workflow

```text id="udx6j0"
WAV File Uploaded
        ↓
Storage Trigger Activates
        ↓
Auto-Transcription Backend Executes
        ↓
Google Speech-to-Text Processes Audio
        ↓
Transcript Generated
        ↓
Transcript Organized & Stored
```

---

# Why Post-Recording Batch Transcription Was Chosen

Audionote currently uses:

* post-recording batch transcription

instead of:

* live streaming transcription.

---

# Engineering Reasons

| Reason                   | Benefit                  |
| ------------------------ | ------------------------ |
| Reduced ESP32 Complexity | Simpler firmware         |
| Lower Power Consumption  | Better portability       |
| Improved Reliability     | Fewer streaming failures |
| Better Upload Recovery   | Retry capability         |
| Reduced Memory Pressure  | Better ESP32 stability   |

---

# Current Recording Flow

```text id="tlyc9r"
Lecture Recording
        ↓
Local WAV Storage
        ↓
Recording Stops
        ↓
Wi-Fi Detected
        ↓
Chunked Upload Begins
        ↓
Backend Processing
```

---

# Why Chunked Uploading Was Used

The system uploads recordings in chunks after recording stops and Wi-Fi becomes available.

This improves:

* upload stability,
* fault tolerance,
* large file handling,
* and embedded reliability.

---

# Benefits of Chunked Uploading

| Benefit            | Explanation                |
| ------------------ | -------------------------- |
| Lower RAM usage    | Important for ESP32        |
| Better recovery    | Failed chunks can retry    |
| Improved stability | Safer uploads              |
| Easier scalability | Supports larger recordings |

---

# Cloud Storage Architecture

Audionote uses Google Cloud Storage for:

* lecture audio storage,
* transcription pipeline integration,
* and backend automation.

---

# Recommended Bucket Structure

```text id="49d5pq"
audionote-recordings/
        └── lectures/
              └── level/
                    └── course/
                          └── session/
```

---

# Event-Driven Backend Processing

Audionote uses event-driven backend execution.

This means:

* services execute automatically in response to upload events.

---

# Event Workflow

```text id="x0wotc"
File Uploaded to Bucket
        ↓
Cloud Storage Event Generated
        ↓
Trigger Activates
        ↓
Backend Executes Automatically
```

---

# Advantages of Event-Driven Architecture

| Advantage                 | Benefit            |
| ------------------------- | ------------------ |
| Reduced idle compute      | Lower costs        |
| Automatic scaling         | Better reliability |
| Efficient processing      | Faster workflows   |
| Cloud-native architecture | Easier maintenance |

---

# Google Shared Drive Integration

Generated outputs can be automatically organized into:

* shared folders,
* course folders,
* lecture directories,
* and archival structures.

This improves:

* collaboration,
* accessibility,
* and academic organization.

---

# Shared Drive Example Structure

```text id="x0bdj4"
Shared Drive
    └── Department
          └── Course
                └── Lecture Session
                      ├── Audio Recording
                      ├── Transcript
                      └── Metadata
```

---

# Why Google Shared Drive Was Chosen

| Reason                | Benefit                   |
| --------------------- | ------------------------- |
| Multi-user access     | Shared academic workflows |
| Large storage support | Lecture archival          |
| Familiar ecosystem    | Easy adoption             |
| Cloud accessibility   | Remote access             |

---

# Backend Technologies Used

| Technology            | Purpose                  |
| --------------------- | ------------------------ |
| Google Cloud Run      | Backend hosting          |
| Google Cloud Storage  | File storage             |
| Google Speech-to-Text | Transcription            |
| Eventarc              | Trigger automation       |
| Google Drive API      | Shared Drive integration |
| Node.js               | Backend runtime          |

---

# Why Cloud Run Was Chosen

Cloud Run was selected because it:

* scales automatically,
* supports containerized workloads,
* simplifies deployment,
* and integrates well with event-driven architectures.

---

# Cloud Run Advantages

| Advantage              | Benefit                       |
| ---------------------- | ----------------------------- |
| Automatic scaling      | Efficient resource usage      |
| Managed infrastructure | Reduced DevOps overhead       |
| Easy deployment        | Faster iteration              |
| Event integration      | Simplified backend automation |

---

# Important Engineering Note

The backend integration stage is typically the most technically demanding part of the Audionote system.

This stage combines:

* embedded firmware,
* cloud infrastructure,
* event automation,
* asynchronous uploads,
* authentication systems,
* and distributed backend services.

Because of this, debugging is an expected part of development.

---

# Recommended Development Approach

```text id="rk1k1m"
Validate Firmware
        ↓
Validate Upload
        ↓
Validate Storage
        ↓
Validate Trigger
        ↓
Validate Transcription
        ↓
Validate Shared Drive Integration
```

Incremental validation significantly simplifies debugging.

---

# Backend Logging & Monitoring

Cloud logs are extremely important for backend debugging.

Useful command:

```bash id="0iy8v7"
gcloud functions logs read autoTranscribeOnUpload --gen2 --region=europe-west1
```

This helps diagnose:

* trigger failures,
* backend crashes,
* permission problems,
* and deployment issues.

---

# Cloud Logging Console

* [Google Cloud Logging Console](https://console.cloud.google.com/logs?utm_source=chatgpt.com)

Useful for:

* real-time monitoring,
* debugging deployment issues,
* and backend observability.

---

# Using AI Assistance in Cloud Shell Editor

Google Cloud Shell Editor includes built-in AI assistance.

Shortcut:

```text id="7y2n5x"
Alt + G
```

This can help:

* interpret deployment errors,
* explain logs,
* troubleshoot Node.js issues,
* and debug Cloud Run deployments.

---

# Recommended Future Backend Improvements

Potential future upgrades include:

| Upgrade                 | Purpose                   |
| ----------------------- | ------------------------- |
| Improved transcription  | Integrating more advamced STT models      |
| OTA firmware updates    | Remote device maintenance |
| Local inference models  | Reduced cloud dependence  |
| Speaker diarization     | Multi-speaker separation  |
| Summarization pipelines | Lecture condensation      |
| Search indexing         | Transcript searchability  |

---

# Backend Engineering Takeaways

The Audionote backend demonstrates practical integration between:

* embedded systems,
* cloud infrastructure,
* asynchronous processing,
* and event-driven architectures.

This project intentionally balances:

* embedded resource limitations,
* cloud scalability,
* system reliability,
* and engineering accessibility.

---
