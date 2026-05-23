# Firmware Overview

This document explains the architecture, operational flow, and engineering decisions behind the Audionote firmware system.

The firmware is responsible for:

* audio acquisition,
* WAV file generation,
* SD card storage,
* wireless communication,
* upload orchestration,
* and cloud backend interaction.

The firmware acts as the embedded control layer that bridges:

* real-world audio capture,
* local embedded processing,
* and cloud-assisted transcription infrastructure.

---

# Firmware Responsibilities

The ESP32 firmware performs several major tasks:

| Responsibility       | Purpose                     |
| -------------------- | --------------------------- |
| Audio Sampling       | Capture lecture audio       |
| WAV File Generation  | Structured audio formatting |
| SD Card Storage      | Persistent local storage    |
| Push Button Handling | User interaction            |
| Wi-Fi Management     | Cloud connectivity          |
| Upload Orchestration | Cloud transfer              |
| Memory Management    | Embedded stability          |
| Status Logging       | Debugging & monitoring      |

---

# Firmware Architecture Overview

```text id="56q7qs"
Push Button Input
        ↓
Recording Controller
        ↓
INMP441 Audio Capture
        ↓
I2S Audio Processing
        ↓
WAV File Generation
        ↓
MicroSD Storage
        ↓
Wi-Fi Detection
        ↓
Chunked Upload System
        ↓
Cloud Backend
```

---

# Why the ESP32 Was Chosen

The ESP32 was selected because it provides:

* integrated Wi-Fi,
* I2S support,
* sufficient embedded performance,
* low cost,
* and strong developer ecosystem support.

---

# Advantages of the ESP32

| Feature                 | Benefit                    |
| ----------------------- | -------------------------- |
| Built-in Wi-Fi          | Cloud uploads              |
| I2S Peripheral          | Digital microphone support |
| Dual-core MCU           | Task flexibility           |
| Low Cost                | Accessibility              |
| Large Community Support | Easier development         |

---

# ESP32 Variant Used

The primary development platform used was:

```text id="6apvks"
ESP32-WROOM-32D DevKitC
```

---

# ESP32 Documentation

* [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/?utm_source=chatgpt.com)

---

# Audio Acquisition System

Audionote uses the:

* INMP441 digital MEMS microphone

for lecture audio acquisition.

The microphone communicates with the ESP32 using:

* the I2S protocol.

---

# Why I2S Was Used

I2S was selected because it:

* simplifies digital audio acquisition,
* improves signal integrity,
* and avoids analog microphone noise issues.

---

# Audio Pipeline

```text id="h1h8az"
Lecturer Audio
        ↓
INMP441 Microphone
        ↓
I2S Audio Stream
        ↓
ESP32 Audio Buffer
        ↓
WAV File Generation
```

---

# INMP441 Documentation

* [INMP441 Datasheet](https://invensense.tdk.com/products/digital/inmp441/?utm_source=chatgpt.com)

---

# WAV File Generation

The firmware converts raw I2S audio data into:

* structured WAV audio files.

This allows:

* compatibility with transcription services,
* easier debugging,
* and standard audio processing workflows.

---

# Why WAV Format Was Chosen

| Reason              | Benefit                      |
| ------------------- | ---------------------------- |
| Standardized format | Wide compatibility           |
| Lossless audio      | Better transcription quality |
| Easy debugging      | Inspectable recordings       |
| STT compatibility   | Cloud integration            |

---

# Storage Architecture

Audio recordings are stored locally using:

* a MicroSD card module.

This enables:

* offline recording,
* upload retries,
* and recording persistence.

---

# Storage Workflow

```text id="4cfbx4"
Audio Capture
        ↓
WAV Generation
        ↓
Local SD Storage
        ↓
Upload Queue
```

---

# Why Local Storage Was Important

Without local storage:

* unstable Wi-Fi could interrupt recordings,
* uploads could fail permanently,
* and reliability would decrease significantly.

Local storage improves:

* fault tolerance,
* reliability,
* and asynchronous operation.

---

# Upload Architecture

Audionote uses:

* delayed asynchronous uploads.

Recordings are uploaded:

* after recording stops,
* and after Wi-Fi becomes available.

---

# Upload Workflow

```text id="yo8gb4"
Recording Stops
        ↓
Wi-Fi Detected
        ↓
Signed URL Requested
        ↓
Chunked Upload Begins
        ↓
Cloud Storage Upload
```

---

# Why Asynchronous Uploading Was Chosen

Streaming audio directly from the ESP32 introduces:

* memory pressure,
* network instability,
* synchronization complexity,
* and increased failure probability.

Asynchronous uploads improve:

* stability,
* reliability,
* and embedded simplicity.

---

# Chunked Uploading

The firmware uploads recordings in chunks to improve:

* upload stability,
* recovery behavior,
* and memory efficiency.

---

# Benefits of Chunked Uploading

| Benefit                     | Explanation                |
| --------------------------- | -------------------------- |
| Lower RAM usage             | Better ESP32 stability     |
| Improved recovery           | Retry failed chunks        |
| Better scalability          | Supports larger recordings |
| Reduced upload failure risk | More robust networking     |

---

# Firmware State Flow

```text id="fxmjlwm"
Idle State
    ↓
Recording State
    ↓
Local Storage State
    ↓
Wi-Fi Detection State
    ↓
Upload State
    ↓
Backend Processing State
```

---

# Push Button Control System

The firmware uses a physical push button for:

* recording start,
* recording stop,
* and interaction control.

---

# Button Workflow

```text id="kkq9gi"
Button Press
        ↓
Recording Starts
        ↓
Button Press
        ↓
Recording Stops
```

---

# Debouncing Considerations

Mechanical push buttons can generate:

* noisy transitions,
* repeated triggers,
* and unstable readings.

The firmware includes debouncing logic to improve:

* reliability,
* user interaction consistency,
* and recording control stability.

---

# Wi-Fi Management

The ESP32 firmware continuously evaluates:

* network availability,
* upload readiness,
* and connection state.

---

# Why Wi-Fi Detection Matters

Uploads are only attempted when:

* recording is complete,
* and stable Wi-Fi is available.

This reduces:

* failed uploads,
* power waste,
* and unstable backend behavior.

---

# Firmware Logging & Debugging

The firmware outputs serial logs for:

* initialization monitoring,
* SD card validation,
* upload debugging,
* and runtime analysis.

---

# Recommended Debugging Tool

Arduino Serial Monitor:

* [Arduino IDE Documentation](https://docs.arduino.cc/software/ide-v2?utm_source=chatgpt.com)

---

# Typical Serial Logs

```text id="lx4v5g"
SD Card Mounted
Wi-Fi Connected
Recording Started
Upload Initialized
Upload Successful
```

---

# Memory Considerations

Embedded memory management was a major consideration during development.

The ESP32 has limited:

* RAM,
* buffer capacity,
* and filesystem resources.

---

# Firmware Design Decisions Influenced by RAM Constraints

| Design Choice                 | Reason                    |
| ----------------------------- | ------------------------- |
| Chunked uploads               | Reduce memory pressure    |
| Post-processing transcription | Simpler embedded workflow |
| SD buffering                  | Avoid RAM overflow        |
| Delayed uploads               | Improve stability         |

---

# Firmware Engineering Trade-offs

| Design Choice        | Trade-off                               |
| -------------------- | --------------------------------------- |
| WAV Storage          | Larger files but better STT quality     |
| Cloud STT            | Better accuracy but internet dependency |
| ESP32 Platform       | Affordable but resource constrained     |
| Asynchronous Uploads | Stable but not real-time                |

---

# Firmware Validation Strategy

The firmware was validated incrementally.

Recommended workflow:

```text id="2u0tk4"
Validate Boot
        ↓
Validate I2S Audio
        ↓
Validate WAV Generation
        ↓
Validate SD Storage
        ↓
Validate Wi-Fi
        ↓
Validate Upload
```

This significantly simplifies debugging.

---

# Important Engineering Note

The firmware layer is one of the most critical parts of the Audionote system because it directly interfaces with:

* physical hardware,
* timing-sensitive audio acquisition,
* filesystem operations,
* and asynchronous cloud communication.

Because of this:

* debugging,
* iterative testing,
* and gradual subsystem validation

are expected parts of development.

---

# Recommended Future Firmware Improvements

Potential future firmware upgrades include:

| Upgrade                 | Purpose                   |
| ----------------------- | ------------------------- |
| OTA Updates             | Remote firmware upgrades  |
| Audio Compression       | Smaller uploads           |
| Real-Time Streaming STT | Live transcription        |
| Battery Monitoring      | Improved power management |
| Deep Sleep Modes        | Lower power consumption   |
| RTOS Task Optimization  | Better concurrency        |

---

# Firmware Engineering Takeaways

The Audionote firmware demonstrates practical integration between:

* embedded audio acquisition,
* local storage systems,
* wireless networking,
* and cloud-connected workflows.

The system intentionally balances:

* embedded limitations,
* cloud scalability,
* reliability,
* and engineering accessibility.

---

# Related Documentation

## Firmware Setup Guide

→ [Firmware Setup Guide](esp32-setup.md)

## Hardware Assembly Guide

→ [Hardware Assembly Guide](../02-hardware/assembly-guide.md)

## Backend Overview

→ [Backend Overview](../04-backend/backend-overview.md)

## Full System Integration

→ [Full System Integration Guide](../05-system-integration/full-system-integration.md)

---

# Additional Resources

## ESP32 Documentation

* [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/?utm_source=chatgpt.com)

## Arduino IDE Documentation

* [Arduino IDE Documentation](https://docs.arduino.cc/software/ide-v2?utm_source=chatgpt.com)

## INMP441 Datasheet

* [INMP441 Datasheet](https://invensense.tdk.com/products/digital/inmp441/?utm_source=chatgpt.com)
