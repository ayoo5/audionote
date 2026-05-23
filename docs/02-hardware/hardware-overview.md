# Hardware Overview

This document explains the physical architecture, subsystem design, and engineering considerations behind the Audionote hardware system.

<img src="/docs/08-assets/diagrams/Hardware Block Diagram.png" alt="Hardware Block Diagram"/>

The hardware layer is responsible for:

* audio acquisition,
* local storage,
* portable power delivery,
* user interaction,
* and embedded system deployment.

The Audionote hardware architecture was designed to balance:

* affordability,
* portability,
* reproducibility,
* and practical engineering constraints.

---

# Hardware System Responsibilities

The hardware system performs several major functions:

| Subsystem                  | Purpose               |
| -------------------------- | --------------------- |
| Audio Capture System       | Lecture recording     |
| Embedded Processing System | Device control        |
| Local Storage System       | WAV file storage      |
| Power Management System    | Portable operation    |
| User Input System          | Recording control     |
| Enclosure System           | Mechanical protection |

---

# High-Level Hardware Architecture

```text id="n9lh6j"
Lecturer Audio
        ↓
INMP441 Microphone
        ↓
ESP32 Processing System
        ↓
MicroSD Storage
        ↓
Power Management System
        ↓
Cloud Upload Infrastructure
```

---

# Hardware Design Philosophy

The hardware architecture prioritizes:

| Goal              | Reason               |
| ----------------- | -------------------- |
| Low Cost          | Accessibility        |
| Modularity        | Easier debugging     |
| Portability       | Real-world usability |
| Simplicity        | Reproducibility      |
| Cloud Integration | Scalability          |

---

# Hardware Architecture Overview

The system combines:

* embedded processing,
* digital audio acquisition,
* removable storage,
* battery operation,
* and wireless networking.

---

# Core Hardware Components

| Component      | Role                    |
| -------------- | ----------------------- |
| ESP32 DevKitC  | Main controller         |
| INMP441        | Digital MEMS microphone |
| MicroSD Module | Audio storage           |
| TP4056         | Battery charging        |
| MT3608         | Voltage boosting        |
| Li-ion Battery | Portable power          |
| Push Button    | Recording control       |

---

# Main Processing System

The primary embedded controller used is:

```text id="sjvjlwm"
ESP32-WROOM-32D DevKitC
```

The ESP32 manages:

* audio sampling,
* file generation,
* SD card access,
* Wi-Fi communication,
* and upload orchestration.

---

# Why the ESP32 Was Selected

The ESP32 was selected because it provides:

* integrated Wi-Fi,
* native I2S support,
* strong embedded performance,
* and wide ecosystem support.

---

# ESP32 Advantages

| Feature           | Benefit                    |
| ----------------- | -------------------------- |
| Wi-Fi             | Cloud uploads              |
| I2S Peripheral    | Digital microphone support |
| Dual-Core MCU     | Better task handling       |
| GPIO Availability | Peripheral integration     |
| Low Cost          | Accessibility              |

---

# ESP32 Documentation

* [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/?utm_source=chatgpt.com)

---

# Audio Capture System

Audionote uses:

* the INMP441 digital MEMS microphone

for lecture audio acquisition.

The microphone interfaces with the ESP32 using:

* the I2S digital audio protocol.

---

# Why the INMP441 Was Chosen

The INMP441 was selected because it:

* simplifies digital audio acquisition,
* improves signal quality,
* and integrates well with ESP32 I2S peripherals.

---

# Audio Capture Pipeline

```text id="iyqbjlwm"
Lecturer Speech
        ↓
INMP441 MEMS Microphone
        ↓
Digital I2S Audio Stream
        ↓
ESP32 Audio Buffer
        ↓
WAV File Generation
```

---

# INMP441 Documentation

* [INMP441 Datasheet](https://invensense.tdk.com/products/digital/inmp441/?utm_source=chatgpt.com)

---

# Storage System

The system uses:

* a MicroSD card module

for local WAV file storage.

This allows:

* offline recording,
* asynchronous uploads,
* and recording persistence.

---

# Storage Workflow

```text id="xjlwm1"
Audio Recording
        ↓
WAV File Generation
        ↓
MicroSD Storage
        ↓
Upload Queue
```

---

# Why Local Storage Was Important

Without local storage:

* network instability could interrupt recordings,
* upload failures could result in data loss,
* and the device would become less reliable.

Local storage improves:

* reliability,
* fault tolerance,
* and scalability.

---

# Power System Overview

Audionote was designed as a:

* portable battery-powered embedded device.

The power subsystem includes:

* rechargeable lithium-ion batteries,
* charging circuitry,
* voltage regulation,
* and power stabilization.

---

# Power Architecture

```text id="8q2jlwm"
Li-ion Battery
        ↓
TP4056 Charging Module
        ↓
MT3608 Boost Converter
        ↓
ESP32 + Peripheral Systems
```

---

# Why Battery Power Was Chosen

Battery operation enables:

* portability,
* wearable deployment,
* classroom flexibility,
* and independent operation.

---

# Power System Components

| Component      | Purpose                |
| -------------- | ---------------------- |
| Li-ion Battery | Portable energy source |
| TP4056         | Safe charging          |
| MT3608         | Voltage boosting       |
| Capacitor      | Power stabilization    |

---

# Power Stability Considerations

The ESP32 can experience:

* sudden current spikes,
* Wi-Fi transmission surges,
* and transient instability.

To improve stability:

* a 470uF capacitor was added across the power rails.

---

# Capacitor Placement

The capacitor should be connected:

* between VCC and GND,
* near the ESP32 power input.

This helps:

* reduce voltage dips,
* improve stability,
* and reduce unexpected resets.

---

# Recommended Capacitor Orientation

```text id="sjlwm2"
Capacitor (+) → VCC
Capacitor (-) → GND
```

---

# User Interaction System

A push button is used for:

* recording control,
* user interaction,
* and workflow management.

---

# Button Workflow

```text id="jlwm3r"
Button Press
        ↓
Recording Starts
        ↓
Button Press
        ↓
Recording Stops
```

---

# Mechanical Design Philosophy

The enclosure design prioritizes:

* portability,
* durability,
* accessibility,
* and iterative prototyping.

---

# Initial Prototype Enclosure

The initial prototype utilized:

* a double socket pattress enclosure.

This allowed:

* rapid prototyping,
* easier drilling,
* and faster iteration.

---

# Optional Final Enclosure

A dedicated:

* 3D printable enclosure

can also be used for:

* cleaner assembly,
* improved portability,
* and professional presentation.

---

# Enclosure Resources

→ [3D Models](/hardware/enclosure)

---

# Wearable Deployment Concept

The device can optionally integrate:

* Velcro straps,
* buckle systems,
* and soft material mounting solutions

for wearable lecture recording deployment.

---

# Example Deployment Concept

```text id="jlwm4p"
Lecturer Waist Strap
        ↓
Portable Recording Unit
        ↓
Wearable Lecture Capture
```

---

# Wiring & Signal Integrity Considerations

Signal quality can be affected by:

* long wire lengths,
* poor solder joints,
* loose grounds,
* and unstable power delivery.

---

# Recommended Hardware Practices

| Practice               | Why It Helps            |
| ---------------------- | ----------------------- |
| Short microphone wires | Better signal integrity |
| Proper solder joints   | Reliability             |
| Heat shrink insulation | Safety                  |
| Stable grounds         | Reduced noise           |
| Power rail decoupling  | Better stability        |

---

# Hardware Validation Workflow

Recommended incremental validation strategy:

```text id="jlwm5q"
Validate Power Rails
        ↓
Validate ESP32 Boot
        ↓
Validate I2S Microphone
        ↓
Validate SD Card
        ↓
Validate Recording
        ↓
Validate Upload
```

This significantly simplifies debugging.

---

# Recommended Debugging Tools

| Tool                   | Purpose              |
| ---------------------- | -------------------- |
| Multimeter             | Voltage verification |
| Arduino Serial Monitor | Firmware debugging   |
| SD Card Reader         | File inspection      |
| USB Power Meter        | Power analysis       |

---

# Important Engineering Note

The hardware layer directly interfaces with:

* timing-sensitive audio systems,
* embedded storage systems,
* battery-powered circuitry,
* and wireless communication hardware.

Because of this:

* wiring quality,
* soldering consistency,
* power stability,
* and subsystem isolation

strongly influence overall system reliability.

---

# Hardware Engineering Trade-offs

| Design Choice     | Trade-off                             |
| ----------------- | ------------------------------------- |
| ESP32 Platform    | Affordable but resource constrained   |
| WAV Storage       | Better STT quality but larger files   |
| Battery Operation | Portable but power-sensitive          |
| Modular Wiring    | Easier debugging but bulkier assembly |

---

# Recommended Future Hardware Improvements

Potential future upgrades include:

| Upgrade                 | Purpose                 |
| ----------------------- | ----------------------- |
| Custom PCB              | Cleaner integration     |
| Better MEMS Microphones | Improved audio quality  |
| Battery Monitoring IC   | Power analytics         |
| Dedicated Audio Codec   | Improved signal quality |
| Compact Enclosure       | Better portability      |

---

# Hardware Engineering Takeaways

The Audionote hardware architecture demonstrates practical integration between:

* embedded electronics,
* digital audio systems,
* portable power systems,
* and cloud-connected embedded workflows.

The system intentionally balances:

* affordability,
* engineering accessibility,
* modularity,
* and practical deployment constraints.

---

# Related Documentation

## Hardware Assembly Guide

→ [Hardware Assembly Guide](assembly-guide.md)

## Power System Guide

→ [Power System Guide](power-system.md)

## Firmware Overview

→ [Firmware Overview](../01-firmware/firmware-overview.md)

## Full System Integration Guide

→ [Full System Integration Guide](../05-system-integration/full-system-integration.md)

---

# Additional Resources

## ESP32 Documentation

* [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/?utm_source=chatgpt.com)

## Arduino IDE Documentation

* [Arduino IDE Documentation](https://docs.arduino.cc/software/ide-v2?utm_source=chatgpt.com)

## INMP441 Datasheet

* [INMP441 Datasheet](https://invensense.tdk.com/products/digital/inmp441/?utm_source=chatgpt.com)

## TP4056 Reference

* [TP4056 Datasheet Reference](https://www.datasheet-pdf.info/entry/TP4056?utm_source=chatgpt.com)

## MT3608 Reference

* [MT3608 Reference Documentation](https://www.sunrom.com/download/327.pdf?utm_source=chatgpt.com)
