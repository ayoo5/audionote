# Firmware Setup

This section guides you through setting up the embedded firmware development environment for Audionote.

The objective of this stage is to:

* prepare the ESP32 firmware toolchain,
* install all required dependencies,
* configure the development environment,
* validate firmware upload capability,
* and confirm that the ESP32 board can communicate successfully with your computer before hardware integration begins.

At the end of this section, you should be able to:

* upload firmware to the ESP32,
* monitor serial output,
* and begin incremental hardware integration safely.

---

# Firmware Development Workflow

```text
Install Arduino IDE
        ↓
Install ESP32 Board Package
        ↓
Install Required Libraries
        ↓
Download the Firmware Folder
        ↓
Configure Firmware
        ↓
Connect ESP32
        ↓
Upload Firmware
        ↓
Validate Serial Output
        ↓
Proceed to Hardware Integration
```

---

# Requirements

Before proceeding, ensure you have the following:

| Requirement         | Purpose                                |
| ------------------- | -------------------------------------- |
| Computer/Laptop     | Firmware development and flashing      |
| USB Data Cable      | ESP32 communication and programming    |
| ESP32 DevKitC Board | Main embedded controller               |
| Internet Connection | Library and board package installation |
| Arduino IDE         | Firmware development environment       |

---

# 1. Install Arduino IDE

Audionote firmware development is based primarily on the Arduino framework for ESP32.

Download and install the latest version of the Arduino IDE:

* [Arduino IDE Download Page](https://www.arduino.cc/en/software?utm_source=chatgpt.com)

Recommended:

* Arduino IDE 2.x or newer

After installation:

1. Launch Arduino IDE
2. Confirm the IDE opens successfully
3. Proceed to ESP32 board installation

---

# 2. Install ESP32 Board Support Package

The ESP32 board package adds support for ESP32 development boards within the Arduino IDE.

## Add ESP32 Board Manager URL

Open:

```text
File → Preferences
```

Inside the:

```text
Additional Boards Manager URLs
```

field, add:

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Official ESP32 Arduino setup guide:

* [ESP32 Arduino Core Installation Guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html?utm_source=chatgpt.com)

---

## Install ESP32 Package

Open:

```text
Tools → Board → Boards Manager
```

Search for:

```text
ESP32
```

Install:

```text
esp32 by Espressif Systems
```

Recommended:

* Install the 3.3.1 version. I found that the newer versions didn't have good enough i2s support.

---

# 3. Configure the ESP32 Board

After installation:

Open:

```text
Tools → Board
```

Select:

```text
ESP32 Dev Module
```

or the equivalent DevKitC board option available in your installed package version.

Recommended settings:

| Setting          | Value   |
| ---------------- | ------- |
| Flash Frequency  | 80MHz   |
| Upload Speed     | 921600  |
| Partition Scheme | Default |
| Core Debug Level | None    |

---

# 4. Install Required Libraries

Audionote uses several libraries for:

* Wi-Fi communication,
* SD card storage,
* JSON handling,
* HTTPS communication,
* and audio processing.

## Install Libraries via Library Manager

Open:

```text
Sketch → Include Library (You'd see all already installed libraries here) → Manage Libraries
```

Install the following:

| Library               | Purpose                            |
| --------------------- | ---------------------------------- |
| ArduinoJson           | JSON serialization/deserialization |
| FS                    | File system access                 |
| SD                    | SD card filesystem                 |
| SPI                   | SPI communication                  |
| WiFi                  | Wireless connectivity              |
| HTTPClient            | HTTPS uploads                      |
| WiFiClientSecure      | Secure HTTPS communication         |

---
Note:
* Most of the libraries listed above come automatically bundled with the ESP32 Arduino Core, except for the ArduinoJson library. You can see them in Sketch → Include Library.
* If they aren't there, you may have to reinstall the esp 32 board manager or try searching and installing them individually from the Library Manager 📚.
* The i2s support driver is bundled in the ESP-IDF core. You do not need to install anything externally.

## ArduinoJson Installation

Official documentation:

* [ArduinoJson Documentation](https://arduinojson.org/?utm_source=chatgpt.com)

Recommended:

* Use the latest stable release.

---

# 5. Install USB Drivers (If Necessary)

Some ESP32 boards require USB-to-serial drivers depending on the onboard USB chip.

Common USB bridge chips include:

| Chip   | Driver              |
| ------ | ------------------- |
| CP2102 | Silicon Labs Driver |
| CH340  | WCH Driver          |

## CP210x Driver

* [CP210x USB Driver Download](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?utm_source=chatgpt.com)

## CH340 Driver

* [CH340 Driver Download](https://sparks.gogo.co.nz/ch340.html?utm_source=chatgpt.com)

After installation:

1. Reconnect the ESP32
2. Verify a COM port appears in Arduino IDE

---

# 6. Download the [Firmware Folder](...)

Open:

```text
firmware/audionote/audionote.ino
```

inside Arduino IDE.

---

# 7. Configure Firmware Variables

Before uploading firmware, configure:

* Wi-Fi credentials,
* backend endpoint URLs,
* API configuration values,
* and device-specific settings.

---

## Example Wi-Fi Configuration

```cpp
const char *ssid = "YOUR_WIFI_NAME";
const char *password = "YOUR_WIFI_PASSWORD";
```
Replace the placeholders with your WiFi credentials.

---

## Example Backend Configuration

```cpp
#define SIGNED_URL_ENDPOINT "https://your-backend-url.com" // Your GCF endpoint
```
Don't worry about this yet as you haven't set up your backend infrastructure.

---

## Example Course Info Configuration

```cpp
const char *COURSE_CODE = "COURSE_CODE eg., EEE313";
const int COURSE_LEVEL = YEAR_OF_STUDY eg., 300, 100;  // This must be an integer, and not include letters.
const char *DEVICE_ID = "esp32a1";
```

It isn't necssary to change the details here. This basically handles the file organization in the final Shared Drive Folder. It classifies the lectures in an institution according to the course code, year of study being instructed, and unique device id of the esp 32 device used to process the specific lecture.

---

# 8. Connect the ESP32 Board

Using a USB data cable:

1. Connect the ESP32 board to your computer
2. Wait for device detection
3. Open Arduino IDE

Select the correct COM port:

```text
Tools → Port
```

---

# 9. Upload Firmware

Click:

```text
Upload
```

or use:

```text
Ctrl + U
```

The firmware should now compile and upload to the ESP32.

During upload:

* the ESP32 bootloader initializes,
* firmware is flashed,
* and the device automatically reboots.

---

# 10. Open Serial Monitor

After successful upload:

Open:

```text
Tools → Serial Monitor
```

Recommended baud rate:

```text
115200
```

Expected output should include initialization logs similar to:

```text
Initializing SD card...
SD card mounted successfully

Connecting to WiFi...
WiFi connected

System ready
```
Don't worry if the log reads failure at some points here, as you (most likely) haven't set up your backend and hardware systems. The essential thing is that your code uploaded.

---

# Firmware Validation Checkpoint

Before proceeding to hardware integration, validate the following:

| Validation Item       | Expected Result |
| --------------------- | --------------- |
| Firmware Upload       | Successful      |
| Serial Monitor Output | Readable logs   |
| ESP32 Boot            | Stable          |
| Wi-Fi Initialization  | Successful      |
| SD Initialization     | No errors       |
| No Continuous Reboots | Stable runtime  |

---

# Common Firmware Setup Issues

| Problem              | Possible Cause                  | Suggested Fix                |
| -------------------- | ------------------------------- | ---------------------------- |
| ESP32 not detected   | Faulty USB cable                | Use a USB data cable         |
| Upload failed        | Wrong COM port                  | Re-select correct port       |
| Board not recognized | Missing drivers                 | Install USB drivers          |
| Continuous rebooting | Power instability               | Use stable USB power         |
| Missing libraries    | Library installation incomplete | Reinstall required libraries |
| Compilation failure  | ESP32 package mismatch          | Update board package         |

---

# Recommended Next Step

Once firmware upload and serial communication are validated successfully, proceed to component acquisition and incremental hardware integration.

Continue to:

→ [Bill of Materials](../02-hardware/bill-of-materials.md)

Then:

→ [Hardware Assembly Guide](../02-hardware/assembly-guide.md)

---

# Additional Resources

## ESP32 Documentation

* [ESP32 Documentation by Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/?utm_source=chatgpt.com)

## Arduino IDE Documentation

* [Arduino IDE Documentation](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/?utm_source=chatgpt.com)

## ESP32 Arduino GitHub Repository

* [ESP32 Arduino GitHub Repository](https://github.com/espressif/arduino-esp32?utm_source=chatgpt.com)

## I2S Peripheral Documentation

* [ESP32 I2S Peripheral Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html?utm_source=chatgpt.com)
