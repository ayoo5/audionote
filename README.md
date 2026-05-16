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

# Quick Start Roadmap

This project is organized as a guided end-to-end engineering workflow.

The recommended implementation path is:

```text
Firmware Environment Setup
        ↓
ESP32 Firmware Upload
        ↓
Hardware Component Acquisition
        ↓
Incremental Hardware Assembly & Testing
        ↓
Power System Integration
        ↓
Backend Deployment (Google Cloud)
        ↓
Shared Drive & Domain Setup
        ↓
Final System Integration
        ↓
Enclosure & Portability Upgrades
        ↓
Testing, Optimization & Contributions
```

The sections below guide you through each stage of the build process in the recommended engineering order.

## Stage 1 — Firmware Development Environment

The firmware is responsible for:

* audio acquisition,
* WAV formatting,
* SD card storage,
* chunked upload handling,
* Wi-Fi connectivity,
* and upload session management.

## Firmware Pipeline

<img src="docs/08-assets/diagrams/Firmware Pipleine.png" alt="Firmware Pipeline"/>

Before assembling the hardware, first prepare the ESP32 firmware environment and confirm that firmware can be successfully uploaded to the board.

This stage includes:

- Installing Arduino IDE
- Installing the ESP32 board package
- Installing required firmware libraries
- Configuring the firmware project
- Verifying serial communication
- Uploading firmware to the ESP32

Continue to:

→ [Firmware Setup](docs/03-firmware/esp32-setup.md)


## Stage 2 — Hardware Components & BOM

After validating the firmware environment, acquire the required hardware components and development materials.

This section includes:

- Main embedded components
- Power system components
- Wiring materials
- Development accessories
- Optional enclosure materials

Continue to:

→ [Bill of Materials](docs/02-hardware/bill-of-materials.md)


## Stage 3 — Hardware Assembly & Incremental Testing

The hardware integration process is intentionally broken into incremental validation stages to simplify debugging and subsystem verification.

### Hardware Block Diagram

<img src="docs/08-assets/diagrams/Hardware Block Diagram.png" alt="Hardware Block Diagram"/>

This stage includes:

- ESP32 basic validation
- INMP441 microphone integration
- SD card module integration
- Audio recording verification
- Chunk upload validation
- Hardware troubleshooting checkpoints

Continue to:

→ [Hardware Assembly Guide](docs/02-hardware/assembly-guide.md)


## Stage 4 — Power System Integration

After validating the core audio and upload system, integrate the portable power subsystem.

This stage includes:

- Battery integration
- TP4056 charging module setup
- MT3608 voltage regulation
- Power stability testing
- Brownout prevention
- Safe portable deployment

Continue to:

→ [Power System Documentation](docs/02-hardware/power-system.md)


## Stage 5 — Backend Infrastructure Setup

Deploy the Google Cloud backend services required for upload orchestration and transcription.

### Backend Pipeline

<img src="docs/08-assets/diagrams/Backend Pipeline.png" alt="Backend Pipeline"/>

This stage includes:

- Google Cloud project setup
- Cloud Run deployment
- Signed URL service setup
- Auto-transcription service setup
- Storage bucket configuration
- IAM permissions configuration

Continue to:

→ [Backend Deployment Guide](docs/04-backend/deployment-guide.md)


## Stage 6 — Shared Drive & Domain Configuration

Configure the shared archival environment for automated transcript organization.

This stage includes:

- Google Workspace setup
- Shared Drive creation
- Domain configuration
- Drive API integration
- Folder structure automation

Continue to:

→ [Shared Drive Setup Guide](docs/04-backend/shared-drive-integration.md)


## Stage 7 — Final Integration & Enclosure Assembly

Combine all validated subsystems into the final portable deployment configuration.

### Final Prototype

<img src="docs/08-assets/demo-photos/Top View of Audionote.jpeg" alt="Final Prototype" width="300" height="500"/>

This stage includes:

- Internal component mounting
- Cable management
- Enclosure assembly
- Thermal considerations
- Wearable mounting system integration
- Final operational testing

Continue to:

→ [Final System Integration](docs/05-system-integration/full-system-integration.md)


## Stage 8 — Optional Upgrades & Contributions

Audionote is designed as a modular and extensible embedded systems platform.

Optional enhancements include:

- 3D printed enclosure upgrades
- Alternative speech-to-text models
- OTA firmware updates
- Noise suppression pipelines
- Custom PCB development
- Mobile configuration systems

Contributors and builders are encouraged to:

- share their builds,
- submit pull requests,
- experiment with backend architectures,
- improve subsystem reliability,
- and extend the platform into new educational or embedded use cases.

Continue to:

→ [Contributing Guide](CONTRIBUTING.md)


## Stage 9 — Engineering Analysis & System Evaluation

This repository includes measured engineering analysis and implementation tradeoff documentation intended to support:

- reproducibility,
- system evaluation,
- embedded optimization,
- and academic engineering analysis.

Included analysis areas:

- memory usage measurements
- upload latency analysis
- storage utilization analysis
- chunk upload optimization
- backend processing latency
- power consumption measurements
- reliability observations
- embedded systems tradeoffs

These documents are especially useful for:

- embedded systems learners,
- engineering students,
- firmware developers,
- and contributors seeking to optimize or extend the system.

Continue to:

→ [Engineering Analysis](docs/06-analysis/build-analysis.md)


## Stage 10 — Troubleshooting & Debugging

Due to the multi-domain nature of the project (embedded systems, cloud infrastructure, storage systems, networking, and power electronics), troubleshooting documentation is provided throughout the repository.

Common troubleshooting areas include:

- ESP32 flashing failures
- SD card mounting issues
- I2S microphone configuration problems
- distorted or clipped audio
- Wi-Fi upload instability
- Cloud authentication failures
- Speech-to-text API errors
- power instability and brownouts
- backend deployment failures

The troubleshooting documentation also includes:

- incremental validation checkpoints,
- expected subsystem outputs,
- debugging methodology,
- and common failure recovery procedures.

Continue to:

→ [General Troubleshooting Guide](docs/07-testing/troubleshooting.md)

