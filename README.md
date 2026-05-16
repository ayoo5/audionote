<div align="center">
  <img src="docs/08-assets/diagrams/audionote-logo.png" alt="Audionote Logo" width="250" />
  <br>
  <h3 style="margin-bottom: 0; line-height: 1;"><strong>Audionote</strong></h3>
  
  ESP32-powered asynchronous lecture transcription and archival system with automated cloud upload, cloud-based speech-to-text processing, and structured Google Shared Drive organization.
</div>

<div align="center">

| Initial Build Setup | Final Enclosed Build <br> (not the 3D printed enclosure) |
| :--------: | :--------: |
| <img src="docs/08-assets/demo-photos/Audionote Stage 2 Development.jpg" width="200" height="250">    | <img src="docs/08-assets/demo-photos/Enclosed Audionote Device.jpeg" width="200" height="250">   |

</div>

# Overview

Audionote is a low-cost embedded lecture transcription and archival platform designed for educational environments with unreliable connectivity and limited infrastructure.

The system captures lecture audio using an ESP32-based embedded device, stores recordings locally on an SD card, uploads audio recordings to a cloud backend in chunks once Wi-Fi becomes available, automatically transcribes the recordings using cloud speech-to-text services, and archives both the audio and transcript files in an organized Google Shared Drive structure.

The project was developed as part of the design and implementation of an asynchronous lecture transcription and archive device system for tertiary educational environments.

# Why Audionote Exists

Traditional note-taking methods are often incomplete, inconsistent, and inaccessible to many students.

In many developing educational environments, real-time transcription systems are impractical due to:

* unstable internet connectivity,
* high cloud streaming costs,
* limited embedded processing power,
* and infrastructure limitations.

Audionote addresses these constraints through an asynchronous edge-to-cloud architecture that prioritizes:

* reliable lecture capture,
* cloud-assisted transcription,
* structured archival,
* and educational accessibility.

The system is particularly useful for:

* students reviewing lecture content,
* accessibility support,
* institutional lecture archiving,
* asynchronous learning,
* and educational content preservation.


# Core Features

## Embedded Systems Features

* ESP32-based audio acquisition pipeline
* INMP441 I2S MEMS microphone integration
* WAV audio recording and formatting
* Local SD card audio buffering and storage
* Push-button lecture recording control
* Chunked upload architecture for large audio recordings
* Portable low-power embedded system design
* Recoverable upload workflow
* Hardware modularity for future PCB migration

## Cloud Features

* Secure signed upload URL generation
* Chunked cloud upload pipeline
* Google Cloud Run backend services
* Automated speech-to-text transcription
* Automatic Google Shared Drive organization
* Cloud-based lecture archival system
* Structured folder organization by course and level
* Cloud storage redundancy

## Systems Engineering Features

* Distributed edge-to-cloud architecture
* Asynchronous post-recording transcription
* Local-first fault-tolerant workflow
* Modular firmware and backend separation
* Recoverable upload session handling
* Automated backend processing pipeline
* Persistent local recording backup
* Scalable backend infrastructure


# System Architecture

<img src="docs/08-assets/diagrams/Full System Architecture.png" alt="System architecture"/>

## End-to-End Operational Workflow

```text
1. Lecturer powers the device
2. ESP32 initializes peripherals
3. Audio is captured from the INMP441 microphone
4. Audio is encoded into WAV format
5. Recording is stored locally on SD card
6. Recording session stops
7. Wi-Fi connection is detected
8. Audio file uploads in chunks to backend
9. Cloud backend processes uploaded recording
10. Speech-to-text transcription is generated
11. Audio and transcript are archived automatically
12. Files become accessible through Shared Drive
```

Additional architecture documentation:

* [System Architecture](docs/01-overview/system-architecture.md)
* [End-to-End Dataflow](docs/05-system-integration/end-to-end-dataflow.md)
* [Cloud Run Architecture](docs/04-backend/cloud-run-architecture.md)

# Repository Structure

```text
lecture-transcription-iot/
│
├── firmware/          # ESP32 firmware source code
├── backend/           # Cloud backend services
├── hardware/          # Schematics, wiring, enclosure files
├── docs/              # Full technical documentation
├── media/             # Demo assets and presentation materials
└── README.md          # You are here 🙂
```


## Hardware Block Diagram

<img src="docs/08-assets/diagrams/Hardware Block Diagram.png" alt="Hardware Block Diagram"/>


# Firmware Overview

The firmware is responsible for:

* audio acquisition,
* WAV formatting,
* SD card storage,
* chunked upload handling,
* Wi-Fi connectivity,
* and upload session management.

## Firmware Pipeline

<img src="docs/08-assets/diagrams/Firmware Pipleine.png" alt="Firmware Pipeline"/>


## Backend Pipeline

<img src="docs/08-assets/diagrams/Backend Pipeline.png" alt="Backend Pipeline"/>



## Additional Resources: (Might remove if not needed again)

* Put somehing here?
