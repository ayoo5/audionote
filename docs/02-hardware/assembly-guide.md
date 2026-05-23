# Hardware Assembly Guide

This section walks through the physical hardware integration process for the Audionote embedded lecture transcription system.

Make sure to read through each build instructions (at least) one stage after another before implementing the build instructions in order to avoid damaging components.

The assembly workflow is intentionally divided into incremental validation stages to:

* simplify debugging,
* isolate subsystem failures,
* reduce wiring complexity,
* and improve development reliability.

Rather than assembling the entire system at once, each major subsystem is validated independently before moving to the next integration stage.

At the end of this section, you should have:

* a fully operational audio acquisition system,
* local WAV recording capability,
* validated upload functionality,
* and a stable hardware platform ready for power system integration and enclosure deployment.

---

# Hardware Integration Workflow

```text id="xz6h9s"
ESP32 Validation
        ↓
Microphone Integration
        ↓
Audio Capture Testing
        ↓
SD Card Integration
        ↓
WAV Recording Validation
        ↓
Wi-Fi Upload Validation
        ↓
Core System Stability Testing
        ↓
Power System Integration
        ↓
Final Enclosure Assembly
```

---

# Before You Begin

Ensure you have completed:

* Firmware setup
* ESP32 board validation
* Firmware upload verification

Continue from:

→ [Firmware Setup](../03-firmware/esp32-setup.md)

---

# Required Hardware

Ensure all required components from the BOM are available before proceeding.

Continue to:

→ [Bill of Materials](bill-of-materials.md)

---

# Recommended Tools

| Tool                  | Purpose                          |
| --------------------- | -------------------------------- |
| Soldering Iron        | Permanent electrical connections |
| Glue Gun              | Internal component mounting      |
| Wire Cutter/Stripper  | Cable preparation                |
| Multimeter            | Voltage and continuity testing   |
| Breadboard            | Temporary prototyping            |
| SD Card Adapter       | File debugging and extraction    |
| USB Cable             | ESP32 programming and power      |

---

# Initial Device Development Image

<img src="/docs/08-assets/demo-photos/Audionote Stage 2 Development.jpg" alt="Initial Device Development" width = 350 height = 450/>

---

# Stage 1 — ESP32 Base Validation

Before connecting additional modules, first validate the ESP32 independently.

## Objectives

Validate:

* firmware upload,
* serial communication,
* stable boot operation,
* and Wi-Fi initialization.

---

## Steps

1. Connect the ESP32 to your computer
2. Upload the firmware
3. Open Serial Monitor
4. Verify initialization logs

Expected output:

```text id="zw7vwo"
System Boot Successful
WiFi Initialized
Awaiting Hardware Initialization
```

---

# Validation Checkpoint

| Test                  | Expected Result |
| --------------------- | --------------- |
| ESP32 powers on       | Successful      |
| Serial monitor output | Stable          |
| No reboot loops       | Stable          |
| Wi-Fi initialization  | Successful      |

---

# Common Issues

| Problem              | Possible Cause         | Suggested Fix           |
| -------------------- | ---------------------- | ----------------------- |
| Continuous rebooting | Insufficient USB power | Use better USB source   |
| ESP32 not detected   | Faulty USB cable       | Replace with data cable |
| Upload failures      | Wrong COM port         | Re-select correct port  |

---

# Stage 2 — INMP441 Microphone Integration

The INMP441 is the primary audio acquisition component of the system.

Audionote uses the ESP32 I2S peripheral to receive digital audio from the microphone.

---

# INMP441 Wiring

## Recommended Wiring Diagram

<img src="/hardware/wiring/ESP 32 Board and INMP441.png" alt="INMP441 Wiring" width = 600 />

Source: [DroneBot Workshop](https://dronebotworkshop.com/esp32-i2s/)

| INMP441 Pin | ESP32 Pin |
| ----------- | --------- |
| VCC         | 3.3V      |
| GND         | GND       |
| WS          | GPIO 25   |
| SCK         | GPIO 32   |
| SD          | GPIO 33   |
| L/R         | GND       |

Verify your firmware pin configuration matches the physical wiring.

---

# I2S Documentation

* [ESP32 I2S Peripheral Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html?utm_source=chatgpt.com)

---

# Important Microphone Notes

* Use short wires where possible (for initial development).
* Utilize just the esp 32 microcontroller and INMP441 mic setup on the breadboard with jumper wires connecting the metal ends in the breadboard for inital development like in the development demo image shown above.
* I noticed that the INMP441 microphone sometimes wasn't outputting the right data, despite verifying that the right connections were made and continuity. I found that tying the microphone connection wires together closely solved this.
* Many jumper wires in the collections don't work well. Test for continuity before use them in your connections, especially the permanent ones.
* Avoid running audio lines near noisy power wiring
* Ensure stable 3.3V supply
* Ground integrity is extremely important for clean audio
* Avoid changing the wiring while the microphone is powered as this can lead to a short.
  P.S.: I learned this lesson (and other ones) after my third microphone got damaged.
* Also, avoid touching the metal ends of the microphone when it's being powered from your esp 32 board as this can also cause shorts. Better to be safe than sorry. The price of the mics stack up as you have to purchase additional ones.
* As mentioned in the [Bill of Materials](bill-of-materials.md), it's very adviced to purchase only the INMP441 mics that come with their metal ends already soldered. i damaged 2 of my first microhphones from the heat of the soldering iron while trying to solder them on propoerly (and it's not because I'm bad at soldering.) 🙃

---

# Audio Capture Test

After connecting the microphone:

1. Upload firmware
2. Start recording
3. Stop recording
4. Verify WAV file generation

Expected result:

* WAV file successfully created on SD card (later stage)
* Serial logs indicate active audio sampling

---

# Validation Checkpoint

| Test                      | Expected Result |
| ------------------------- | --------------- |
| I2S initialization        | Successful      |
| Audio capture starts      | Successful      |
| No major noise/distortion | Stable          |
| No ESP32 crashes          | Stable          |

---

# Common Issues

| Problem         | Possible Cause              | Suggested Fix                 |
| --------------- | --------------------------- | ----------------------------- |
| No audio data   | Incorrect wiring            | Verify I2S pin mapping        |
| Distorted audio | Ground instability          | Improve grounding             |
| Very low audio  | Incorrect gain/config       | Verify firmware configuration |
| ESP32 freeze    | Incorrect DMA configuration | Verify buffer settings        |

---

# Audio Waveform Test Code

As you try to get the INMP441 mic to work, you may (most likely) face problems ensuring that your connections are secure and that the audio is actually being captured.

To test and ensure that your mic is working well and performing as it ought to:
* Download the [INMP441 test code](/firmware/tests/microphone_test/DroneBot_INMP441_File.ino) (courtesy of DroneBot Workshop).
* Upload it to your ESP 32 board
* Open your Serial Plotter to see the output waveform.

Source: [DroneBot Workshop](https://dronebotworkshop.com/esp32-i2s/)

## Correct Audio Waveform Test Diagram

The correct audio waveform for a silent room is shown below:

<img width="500" alt="Correct audio wave for silence" src="https://github.com/user-attachments/assets/f4c3a4fe-d5b0-48cb-a47c-6f3666bff4eb" />

Note:

* You would see frequency spikes as you speak into the mic and it will return back to a waveform similar to the one above when no there is no audio input.
* If your audio waveform is not very similar to this, check your connections.
* Upload your main esp32 setup firmware code back when you've ensured that the INMP441 microphone is capturing the audio data as it ought to.

---

# Stage 3 — SD Card Module Integration

The SD card subsystem provides persistent local storage for recorded lecture audio.

This is one of the most important reliability features of Audionote.

---

# SD Card Wiring

| SD Module Pin | ESP32 Pin |
| ------------- | --------- |
| VCC           | 5V        |
| GND           | GND       |
| MISO          | GPIO 19   |
| MOSI          | GPIO 23   |
| SCK           | GPIO 18   |
| CS            | GPIO 5    |

---

# SD Card Formatting

Before use:

* Format the SD card as FAT32

Recommended:

* Class 10 SD cards
* ≥ 4GB storage

Useful formatting guide:

* [SD Card Formatter Utility](https://www.sdcard.org/downloads/formatter/?utm_source=chatgpt.com)

---

# SD Card Validation

After integration:

1. Insert SD card
2. Power the ESP32
3. Observe serial logs

Expected output:

```text id="58o7x9"
Initializing SD card...
SD card mounted successfully
```

---

# WAV Recording Validation

Perform a short recording test.

Validate:

* file creation,
* file size growth,
* and audio playback.

Recommended:

* Remove SD card
* Play WAV file on a computer

Expected result:

* intelligible speech recording

---

# Validation Checkpoint

| Test               | Expected Result |
| ------------------ | --------------- |
| SD card mount      | Successful      |
| WAV file creation  | Successful      |
| Audio playback     | Intelligible    |
| No file corruption | Stable          |

---

# Common Issues

| Problem                  | Possible Cause       | Suggested Fix           |
| ------------------------ | -------------------- | ----------------------- |
| SD mount failed          | Incorrect formatting | Reformat FAT32          |
| Corrupted WAV file       | Improper shutdown    | Stop recording properly |
| Recording stops randomly | Power instability    | Improve power delivery  |
| No files created         | Incorrect CS pin     | Verify SPI wiring       |

---

# Important SD Card Module Notes

* Strictly avoid hot wiring the SD Card while it is being powered. Hot-wiring or pulling an SD card while it is powered poses severe risks to your hardware and data. The practice bypasses safe hardware handshakes and can trigger critical faults.
*  The module may briefly pause operation after several write cycles while the SD card's internal controller erases flash pages. When tis occurs, briefly stop testing or replace with another SD Card module.
*  Make sure the (temporary) jumper connections are good and tight as the pins of the SD card module are sensitive. If not connected well, audio files would be corrupted.
*  After full assembly, I noticed that I needed to scratch between the SD card solder connection lines on the main veroboard. I think this is due to cross-talk of the SD communication lines even though I verified zero continuity between the soldering connection lines.

---

# Stage 4 — Wi-Fi Upload Validation

Once local recording is validated, test cloud upload functionality. To do this, you have to set up your backend GCF endpoint which can be seen in [Backend Deployment Guide](../04-backend/deployment-guide.md).

---

# Objectives

Validate:

* Wi-Fi connectivity,
* backend communication,
* chunk upload handling,
* and successful upload completion.

---

# Upload Validation Procedure

1. Connect to Wi-Fi
2. Record short audio sample
3. Stop recording
4. Wait for upload sequence

Expected serial output:

```text id="utrz6o"
WiFi connected
Requesting signed upload URL...
Upload session initialized
Uploading chunks...
Upload completed successfully
```

---

# Validation Checkpoint

| Test                  | Expected Result |
| --------------------- | --------------- |
| Wi-Fi connection      | Successful      |
| Signed URL generation | Successful      |
| Upload begins         | Successful      |
| Upload completes      | Successful      |

---

# Common Issues

| Problem        | Possible Cause      | Suggested Fix             |
| -------------- | ------------------- | ------------------------- |
| Wi-Fi timeout  | Weak signal         | Improve signal quality    |
| Upload failure | Backend unavailable | Verify backend deployment |
| HTTPS error    | Invalid endpoint    | Verify URL configuration  |
| Upload stalls  | Oversized chunks    | Reduce chunk size         |

---

# Stage 5 — Core System Stability Testing

Before integrating the power subsystem, validate the complete recording pipeline under repeated usage.

---

# Recommended Stability Tests

## Recording Loop Test

Perform:

* multiple consecutive recordings,
* repeated upload cycles,
* and extended runtime validation.

---

## Long Recording Test

Validate:

* SD stability,
* memory usage,
* and upload integrity.

Recommended:

* ≥ 15 minute recordings

---

## Brownout Observation Test

Watch for:

* unexpected ESP32 resets,
* SD corruption,
* upload interruptions,
* or unstable Wi-Fi behavior.

---

# Stability Validation Checklist

| Test                    | Expected Result |
| ----------------------- | --------------- |
| Multiple recordings     | Stable          |
| Long-duration recording | Stable          |
| Repeated uploads        | Stable          |
| No unexpected resets    | Stable          |
| No file corruption      | Stable          |

---

# Recommended Next Step

Once the core recording and upload pipeline is validated successfully, proceed to portable power integration.

Continue to:

→ [Power System Documentation](power-system.md)

Then:

→ [Backend Deployment Guide](../04-backend/deployment-guide.md)

---

# Additional Resources

## ESP32 Documentation

* [ESP32 Documentation by Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/?utm_source=chatgpt.com)

## SPI Documentation

* [Arduino SPI Documentation](https://www.arduino.cc/en/reference/SPI?utm_source=chatgpt.com)

## SD Library Documentation

* [Arduino SD Library Documentation](https://docs.arduino.cc/libraries/sd/?utm_source=chatgpt.com)

## FAT32 Formatting Utility

* [Official SD Card Formatter Utility](https://www.sdcard.org/downloads/formatter/?utm_source=chatgpt.com)
