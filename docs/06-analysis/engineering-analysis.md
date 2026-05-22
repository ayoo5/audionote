# Engineering Analysis

This section documents the engineering evaluation, testing methodology, system performance observations, and analytical workflow used during the development of the Audionote system.

The goal of this analysis is to:

* evaluate real-world system behavior,
* measure performance characteristics,
* identify system limitations,
* validate architecture decisions,
* and provide reproducible engineering data for future contributors and researchers.

This section also documents the workflow used to:

* export backend-generated metadata,
* organize evaluation datasets,
* compute latency and throughput metrics,
* and analyze transcription performance using Google Sheets.

---

# Analysis Objectives

The engineering analysis focused on evaluating:

| Evaluation Area              | Purpose                       |
| ---------------------------- | ----------------------------- |
| Audio Storage Efficiency     | Storage optimization          |
| Upload Performance           | Cloud transfer reliability    |
| Transcription Latency        | Backend responsiveness        |
| Throughput Performance       | End-to-end efficiency         |
| File Integrity               | Reliability                   |
| Folder Organization Accuracy | Backend automation validation |
| System Stability             | Long-duration operation       |

---

# Analysis Workflow Overview

```text id="if72ta"
Lecture Recording
        ↓
WAV Upload
        ↓
Cloud Processing
        ↓
Transcript Generation
        ↓
Metadata Export
        ↓
Google Sheets Analysis
        ↓
Performance Evaluation
```

---

# Reference Transcript Dataset

The engineering analysis utilized a reference transcript dataset to compare:

* generated transcript outputs,
* timing behavior,
* and system consistency.

---

# Reference Transcript Resource

→ [Reference Transcript Dataset](../06-analysis/Reference Transcript.md)

This dataset was used for:

* validation,
* comparison,
* and performance consistency checks.

---

# Engineering Metrics Collected

The following metrics were tracked during testing:

| Metric                       | Purpose                           |
| ---------------------------- | --------------------------------- |
| Recording ID                 | Session tracking                  |
| Audio Size (MB)              | Storage analysis                  |
| Audio Duration (Minutes)     | Runtime analysis                  |
| Storage Efficiency (MB/min)  | Compression efficiency evaluation |
| Corrupted Files              | Reliability evaluation            |
| Audio Size (B)               | Byte-level precision              |
| Transcript Size (B)          | Transcript scaling analysis       |
| Audio Duration (Seconds)     | Timing analysis                   |
| GCloud Created Datetime      | Upload timestamp                  |
| Transcript Created Datetime  | Processing timestamp              |
| Latency (Seconds)            | Backend processing delay          |
| Throughput (Seconds)         | End-to-end system performance     |
| File Organization Inaccuracy | Automation validation             |

---

# Recommended Google Sheets Structure

Example columns:

```text id="8zcg8m"
Recording ID
Audio Size (MB)
Audio Duration (Minutes)
Storage Efficiency (MB/min)
Corrupted Files
Audio Size (B)
Transcript Size (B)
Audio Duration (Seconds)
GCloud Created Datetime
Transcript Created Datetime
Latency (Seconds)
Throughput (Seconds)
File Organization Inaccuracy
```

---

# Data Collection Workflow

The analysis workflow involved:

```text id="gcf39u"
Cloud Storage Metadata
        ↓
Transcript Metadata
        ↓
Export to Google Sheets
        ↓
Manual Validation
        ↓
Formula-Based Analysis
        ↓
Performance Evaluation
```

---

# Exporting Metadata to Google Sheets

The Audionote backend can be extended to:

* export metadata automatically,
* generate structured logs,
* and organize performance metrics inside Google Sheets.

---

# Recommended Workflow

```text id="66xk5u"
Backend Logs
        ↓
Metadata Extraction Script
        ↓
Google Sheets Export
        ↓
Gemini-Assisted Analysis
```

---

# Suggested Metadata Fields

| Field                | Description                |
| -------------------- | -------------------------- |
| Recording Name       | Uploaded WAV identifier    |
| Upload Timestamp     | Cloud upload time          |
| Transcript Timestamp | Transcript completion time |
| File Size            | WAV file size              |
| Transcript Size      | Transcript output size     |
| Upload Status        | Success/failure            |
| Folder Accuracy      | Organizational validation  |

---

# Exporting Storage Bucket Data from Google Cloud

This section explains how to export recording metadata from Google Cloud Storage for engineering analysis inside Google Sheets.

The exported metadata can be used to analyze:
- upload latency,
- throughput,
- storage efficiency,
- transcript generation behavior,
- and overall backend performance.

---

# Recommended Export Workflow

```text
Google Cloud Storage
        ↓
Bucket Object Metadata
        ↓
CSV Export
        ↓
Google Sheets Import
        ↓
Engineering Analysis
```

---

# Option 1 — Export Using the Google Cloud Console (Beginner-Friendly)

Open:

- https://console.cloud.google.com/storage/browser

---

# Step 1 — Open the Storage Bucket

Navigate to:

```text
audionote-recordings
```

or your configured recording bucket.

---

# Step 2 — Open the Object List

Inside the bucket, you should see uploaded recordings such as:

```text
lectures/
    └── LEC145.wav
    └── LEC146.wav
```

Metadata visible includes:
- file size,
- upload timestamp,
- object name,
- storage class,
- and creation time.

---

# Step 3 — Copy Metadata into Google Sheets

Currently, Google Cloud Storage does not directly export bucket metadata into Sheets automatically from the GUI.

Recommended workflow:

1. Copy:
   - object names,
   - upload timestamps,
   - file sizes,
   - and recording identifiers

2. Paste them into:
   - Google Sheets

This is sufficient for small-to-medium engineering datasets.

---

# Option 2 — Export Bucket Metadata Using gsutil (Recommended)

This approach is more scalable and reproducible.

---

# Install Google Cloud SDK

- https://cloud.google.com/sdk/docs/install

---

# List Bucket Objects

```bash
gsutil ls -l gs://audionote-recordings/**
```

This outputs:
- file sizes,
- timestamps,
- and object paths.

---

# Example Output

```text
1468000  2026-01-29T06:34:17Z  gs://audionote-recordings/LEC145.wav
2516000  2026-01-29T06:36:14Z  gs://audionote-recordings/LEC146.wav
```

---

# Export Bucket Metadata to CSV

Example command:

```bash
gsutil ls -l gs://audionote-recordings/** > recordings_metadata.txt
```

Then manually:
1. clean the output,
2. convert to CSV,
3. and import into Google Sheets.

---

# Recommended Metadata Columns

| Field | Purpose |
|---|---|
| Object Name | Recording identifier |
| File Size | Storage analysis |
| Upload Timestamp | Latency analysis |
| Folder Path | Organization validation |

---

# Import into Google Sheets

Open:

- https://docs.google.com/spreadsheets/

Then:

```text
File
    ↓
Import
    ↓
Upload CSV
```

---

# Recommended Spreadsheet Workflow

```text
Bucket Metadata
        ↓
Google Sheets
        ↓
Derived Metrics
        ↓
Charts & Analysis
```

---

# Optional Python-Based Export Workflow

Advanced users may automate exports using:
- Python,
- pandas,
- and the Google Cloud Storage SDK.

Useful for:
- large datasets,
- repeated testing,
- and automated engineering evaluation pipelines.

---

# Google Cloud Storage SDK Documentation

- https://cloud.google.com/python/docs/reference/storage/latest

---

# Important Engineering Note

The analysis workflow intentionally combines:
- cloud metadata,
- backend timing behavior,
- transcript outputs,
- and manual engineering validation.

This hybrid workflow reflects real-world engineering analysis practices where:
- automation,
- scripting,
- and manual validation

are often used together.

---

# Google Sheets Setup

Open:

* [Google Sheets](https://docs.google.com/spreadsheets/?utm_source=chatgpt.com)

Create a spreadsheet for:

* performance metrics,
* upload logs,
* and transcription analysis.

---

# Recommended Sheet Organization

| Sheet               | Purpose                |
| ------------------- | ---------------------- |
| Upload Metrics      | Upload timing analysis |
| Transcript Metrics  | STT analysis           |
| Error Logs          | Failure tracking       |
| Long-Duration Tests | Reliability analysis   |

---

# Using Gemini Inside Google Sheets

Google Sheets includes Gemini AI integration in supported Workspace environments.

Gemini can help:

* generate formulas,
* analyze datasets,
* compute derived columns,
* and assist with engineering observations.

---

# Gemini in Sheets Documentation

* [Gemini in Google Workspace Documentation](https://support.google.com/docs/answer/13594961?utm_source=chatgpt.com)

---

# Useful Gemini Workflow

The Gemini side panel can assist with:

* generating formulas,
* summarizing trends,
* calculating metrics,
* and identifying anomalies.

---

# Recommended Manual Validation Workflow

Even when using Gemini assistance:

* manually validate generated formulas,
* verify timestamp formatting,
* and inspect unusual outliers carefully.

AI assistance should support analysis — not replace engineering validation.

---

# Recommended Spreadsheet Formulas

---

# 1. Storage Efficiency (MB/min)

Formula:

```excel id="dcdz4h"
=Audio_Size_MB / Audio_Duration_Minutes
```

Example:

```excel id="m6ayfi"
=B2/C2
```

---

# Purpose

This measures:

* how efficiently recordings utilize storage.

Lower values indicate:

* more storage-efficient recordings.

---

# 2. Audio Duration (Minutes)

Formula:

```excel id="n9b3z2"
=Audio_Duration_Seconds / 60
```

Example:

```excel id="mjlwm6"
=H2/60
```

---

# 3. Latency (Seconds)

Latency measures:

* the delay between upload completion
* and transcript generation.

Formula:

```excel id="6j90ka"
=(Transcript_Created_Datetime - GCloud_Created_Datetime) * 86400
```

Example:

```excel id="x8ht0e"
=(J2-I2)*86400
```

---

# Why Multiply by 86400?

Google Sheets stores datetime internally as days.

```text id="zvbk77"
86400 seconds = 1 day
```

Multiplication converts:

* days → seconds.

---

# 4. Throughput (Seconds)

Throughput measures:

* overall end-to-end processing time.

Example formula:

```excel id="x6z8zw"
=Latency_Seconds / Audio_Duration_Seconds
```

Example:

```excel id="aogkij"
=K2/H2
```

---

# Throughput Interpretation

| Value | Interpretation        |
| ----- | --------------------- |
| < 1   | Faster than real-time |
| ≈ 1   | Real-time equivalent  |
| > 1   | Slower than real-time |

---

# 5. File Organization Inaccuracy

Recommended workflow:

* manually validate folder correctness,
* then assign binary values.

Example:

| Value | Meaning                |
| ----- | ---------------------- |
| 0     | Correct organization   |
| 1     | Incorrect organization |

---

# Example Formula

```excel id="ygxwbh"
=IF(ExpectedFolder=ActualFolder,0,1)
```

---

# 6. Corrupted Files Rate

Example formula:

```excel id="0izdgl"
=COUNTIF(D2:D100,"YES")/COUNTA(D2:D100)
```

---

# Recommended Analysis Visualizations

Useful charts include:

| Visualization          | Purpose               |
| ---------------------- | --------------------- |
| Latency vs Duration    | Backend scaling       |
| Audio Size vs Duration | Storage efficiency    |
| Throughput Trends      | Processing efficiency |
| Error Frequency        | Reliability analysis  |

---

# Google Sheets Chart Documentation

* [Google Sheets Chart Documentation](https://support.google.com/docs/answer/63824?utm_source=chatgpt.com)

---

# Recommended Experimental Workflow

```text id="jlwm5w"
Short Recordings
        ↓
Medium Recordings
        ↓
Long Recordings
        ↓
Stress Testing
        ↓
Backend Analysis
```

---

# Long-Duration Testing

Extended-duration testing helps evaluate:

* SD card stability,
* memory behavior,
* backend consistency,
* upload recovery,
* and transcription reliability.

Recommended durations:

* 15 minutes,
* 30 minutes,

---

# Additional Resources

## Google Sheets Documentation

* [Google Sheets Documentation](https://support.google.com/docs?utm_source=chatgpt.com)

## Gemini in Workspace Documentation

* [Gemini in Google Workspace Documentation](https://support.google.com/docs/answer/13594961?utm_source=chatgpt.com)
