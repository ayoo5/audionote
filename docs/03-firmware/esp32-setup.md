# Firmware Overview

The firmware is responsible for:

* audio acquisition,
* WAV formatting,
* SD card storage,
* chunked upload handling,
* Wi-Fi connectivity,
* and upload session management.

## Firmware Pipeline

<img src="/docs/08-assets/diagrams/Firmware Pipleine.png" alt="Firmware Pipeline"/>

## Firmware Features

* I2S microphone audio capture
* WAV header generation
* SD card storage management
* Chunked resumable upload system
* Wi-Fi upload management
* Upload state persistence
* Memory-aware streaming buffers
* Automatic recording session handling

## Firmware Setup [Get started with building here. 👇]

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

### Uploading Firmware [The Steps. 🪜]

1. Install Arduino IDE
2. Install ESP32 board support package
3. Clone this repository
4. Open `audionote.ino`
5. Configure Wi-Fi credentials
6. Configure backend endpoint
7. Select ESP32 Dev Module
8. Upload firmware

Detailed firmware documentation:

* [Firmware Overview](docs/03-firmware/firmware-overview.md)
* [Firmware Architecture](docs/03-firmware/firmware-architecture.md)
* [Firmware Troubleshooting](docs/03-firmware/troubleshooting-firmware.md)
