<div align="center">
  <img src="docs/08-assets/diagrams/Audionote logo.png" alt="Audionote Logo" width="250" />
  <br>
  <h3 style="margin-bottom: 0; line-height: 1;"><strong>Audionote</strong></h3>
  
  ESP32-powered asynchronous lecture transcription and archival system with automated cloud upload, cloud-based speech-to-text processing, and structured Google Shared Drive organization.
</div>

<div align="center">

| Initial Build | Final Enclosed Build <br> (without 3D printed enclosure) |
| :--------: | :--------: |
| <img src="url" width="200" height="200">    | <img src="url" width="200" height="200">   |

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

```text
Picture goes here
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
└── README.md
```

# Hardware Overview

The Audionote embedded hardware platform is built around the ESP32 microcontroller and includes:

* ESP32 DevKitC
* INMP441 digital I2S microphone
* MicroSD card module
* Battery and power management system
* Push-button recording interface
* Portable enclosure system

Detailed hardware documentation:

* [Hardware Overview](docs/02-hardware/hardware-overview.md)
* [Bill of Materials](docs/02-hardware/bill-of-materials.md)
* [Wiring Guide](docs/02-hardware/wiring-guide.md)
* [Schematic Diagram](docs/02-hardware/schematic-diagram.md)
* [Power System](docs/02-hardware/power-system.md)
* [Enclosure Design](docs/02-hardware/enclosure-design.md)
* [Assembly Guide](docs/02-hardware/assembly-guide.md)

---

# Bill of Materials

| Part           | Description                 | Image | Qty | Notes                                                                              |
| -------------- | --------------------------- | ----- | --- | ---------------------------------------------------------------------------------- |
| ESP32 DevKitC  | Main embedded controller    |  <img src="url" width="200" height="100">     | 1   | ESP32-WROOM-32D was used in this project, primarily because of innate i2s support. |
| INMP441        | Digital I2S MEMS microphone |   <img src="url" width="200" height="100">    | 1   | Preferably, get the one that comes with its metal ends already soldered. This would save you so much trouble.        |
| MicroSD Module | Local audio storage         |   <img src="url" width="200" height="100">    | 1   | FAT32 recommended                                                                  |
| MicroSD Card   | Audio recording storage     |   <img src="url" width="200" height="100">    | 1   | >4GB recommended                                                                   |
| LiPo Battery   | Portable power source       |   <img src="url" width="200" height="100">    | 1   | 3.7V                                                                               |
| TP4056 Module  | Battery charging module     |    <img src="url" width="200" height="100">   | 1   | USB charging, With protection                                                      |
| Push Button    | Recording control           |    <img src="url" width="200" height="100">   | 1   |                                                                                    |
| Enclosure      | Device housing              |    <img src="url" width="200" height="100">   | 1   | 3D printable                                                                       |

Full hardware details:

* [Complete BOM](docs/02-hardware/bill-of-materials.md)


# Firmware Overview

The firmware is responsible for:

* audio acquisition,
* WAV formatting,
* SD card storage,
* chunked upload handling,
* Wi-Fi connectivity,
* and upload session management.

## Firmware Pipeline

```text
Picture goes here
```

## Firmware Features

* I2S microphone audio capture
* WAV header generation
* SD card storage management
* Chunked resumable upload system
* Wi-Fi upload management
* Upload state persistence
* Memory-aware streaming buffers
* Automatic recording session handling

## Firmware Setup

### Requirements

* Arduino IDE
* ESP32 Arduino Core
* USB cable
* ESP32 DevKitC board

### Required Libraries

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "driver/i2s.h"
```

### Uploading Firmware

1. Install Arduino IDE
2. Install ESP32 board support package
3. Clone this repository
4. Open `esp32_audio_uploader.ino`
5. Configure Wi-Fi credentials
6. Configure backend endpoint
7. Select ESP32 Dev Module
8. Upload firmware

Detailed firmware documentation:

* [Firmware Overview](docs/03-firmware/firmware-overview.md)
* [ESP32 Setup](docs/03-firmware/esp32-setup.md)
* [Firmware Architecture](docs/03-firmware/firmware-architecture.md)
* [Audio Recording Pipeline](docs/03-firmware/audio-recording-pipeline.md)
* [Chunk Upload System](docs/03-firmware/chunk-upload-system.md)
* [Memory Management](docs/03-firmware/memory-management.md)
* [Firmware Troubleshooting](docs/03-firmware/troubleshooting-firmware.md)

# Backend Overview

The backend infrastructure is hosted primarily on Google Cloud Run and manages:

* signed upload URL generation,
* cloud storage upload orchestration,
* automatic transcription,
* transcript generation,
* and Google Shared Drive archival.

## Backend Services

### signedurl Service

Responsible for:

* generating signed upload URLs,
* initializing resumable upload sessions,
* organizing upload metadata,
* and validating upload requests.

### autotranscribe Service

Responsible for:

* monitoring uploaded recordings,
* triggering speech-to-text transcription,
* generating transcript files,
* and organizing archival folders.

## Backend Pipeline

```text
Picture goes here
```

Backend documentation:

* [Backend Overview](docs/04-backend/backend-overview.md)
* [Cloud Run Architecture](docs/04-backend/cloud-run-architecture.md)
* [Signed URL Service](docs/04-backend/signed-url-service.md)
* [Auto-Transcribe Service](docs/04-backend/auto-transcribe-service.md)
* [Speech-to-Text Pipeline](docs/04-backend/speech-to-text-pipeline.md)
* [Shared Drive Integration](docs/04-backend/shared-drive-integration.md)
* [Deployment Guide](docs/04-backend/deployment-guide.md)

---

# Google Cloud Platform Setup

The backend infrastructure requires:

* Google Cloud Storage
* Google Cloud Run
* Google Speech-to-Text API
* Google Drive API
* Service Accounts and IAM configuration

Detailed setup guides:

* [GCP Project Setup](docs/04-backend/gcp-project-setup.md)
* [Authentication and Security](docs/04-backend/authentication-and-security.md)
* [Deployment Guide](docs/04-backend/deployment-guide.md)
* [Custom Domain Setup](docs/04-backend/custom-domain-setup.md)

Useful external references:

* [Google Cloud Run Documentation](https://cloud.google.com/run/docs)
* [Google Speech-to-Text Documentation](https://cloud.google.com/speech-to-text/docs)
* [Google Drive API Documentation](https://developers.google.com/drive)

# End-to-End Operational Workflow

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

Additional workflow documentation:

* Put somehing here?
