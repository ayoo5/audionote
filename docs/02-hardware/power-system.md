# Power System Guide

This section documents the portable power architecture used in the Audionote embedded lecture transcription system.

The objective of this stage is to safely integrate:

* the lithium battery system,
* charging circuitry,
* voltage regulation,
* and power distribution,

while ensuring stable ESP32 operation during:

* audio recording,
* SD card access,
* Wi-Fi transmission,
* and backend upload operations.m 

At the end of this section, you should have:

* a portable battery-powered Audionote device,
* stable regulated voltage rails,
* safe charging capability,
* and a validated power delivery system ready for enclosure integration.

---

# Power System Architecture

```text id="ytm9dy"
18650 Battery Cells
        ↓
Parallel Battery Pack
        ↓
TP4056 Charging Module
        ↓
MT3608 Boost Converter
        ↓
ESP32 + Peripheral Power Rail
        ↓
INMP441 + SD Module + Upload System
```

---

# Power System Objectives

The power subsystem must:

* provide stable voltage to the ESP32,
* support Wi-Fi current spikes,
* maintain SD card stability,
* prevent brownouts,
* support portable operation,
* and safely recharge the battery system.

---

# Before You Begin

Ensure the following stages are already completed:

* Firmware setup
* Hardware assembly
* Audio recording validation
* SD card validation
* Upload system validation

Continue from:

→ [Hardware Assembly Guide](assembly-guide.md)

---

# Required Power Components

| Component              | Purpose                         |
| ---------------------- | ------------------------------- |
| 18650 Li-Ion Cells     | Main portable power source      |
| TP4056 Module          | Battery charging and protection |
| 470uF Capacitor        | Voltage stabilizer, transient buffer|
| MT3608 Boost Converter | Voltage step-up regulation      |
| Power Switch           | Main system power control       |
| Hook-up Wires          | Power distribution              |
| Heat Shrink Tubing     | Electrical insulation           |
| Multimeter             | Voltage validation              |
| Soldering Tools        | Permanent connections           |

---

# Important Safety Notes

Lithium battery systems must be handled carefully.

Before proceeding:

* Never short battery terminals
* Never puncture Li-Ion cells
* Do not charge damaged batteries
* Never connect LiPo batteries directly to the ESP32 3.3V rail
* Double-check polarity before powering the ESP32
* Always validate voltage outputs before connecting peripherals
* Insulate exposed terminals properly
* Disconnect batteries during wiring modifications
* Avoid powering the ESP32 from multiple unstable sources simultaneously

If you are unfamiliar with lithium battery handling:

* [Lithium-Ion Battery Safety Basics](https://batteryuniversity.com/article/bu-304a-safety-concerns-with-li-ion?utm_source=chatgpt.com)

---

# Power System Overview

Audionote uses:

* parallel 18650 lithium battery cells,
* a TP4056 charging/protection module,
* and an MT3608 boost converter

to create a rechargeable portable embedded power platform.

---

## Recommended Wiring Diagram

![Power System Wiring](/docs/08-assets/diagrams/Power Management System Schematic.png)

---

# Why Parallel Battery Configuration?

The batteries are connected in parallel primarily to:

* increase available current capacity,
* improve runtime,
* reduce voltage sag during Wi-Fi transmission,
* and improve overall system stability.

The system still operates at approximately:

```text id="6vysal"
3.7V nominal
```

while increasing battery capacity.

---

# Stage 1 — Battery Pack Assembly

## Recommended Battery Configuration

```text id="2k0xqx"
Battery (+) → Battery (+)
Battery (-) → Battery (-)
```

This creates a parallel battery pack.

---

# Important Parallel Battery Notes

Before connecting cells in parallel:

* Ensure both cells are at similar voltages
* Use identical battery types where possible
* Avoid mixing damaged or aged cells
* Insulate exposed terminals properly

---

# Validation Checkpoint

Using a multimeter:

| Test                  | Expected Result |
| --------------------- | --------------- |
| Battery pack voltage  | ~3.7V to 4.2V   |
| No excessive heating  | Stable          |
| Stable voltage output | Stable          |

---

# Stage 2 — TP4056 Charging Module Integration

The TP4056 module provides:

* battery charging,
* overcharge protection,
* over-discharge protection,
* and short-circuit protection.

---

# TP4056 Wiring

| TP4056 Pin | Connection              |
| ---------- | ----------------------- |
| B+         | Battery Positive        |
| B-         | Battery Negative        |
| OUT+       | Boost Converter Input + |
| OUT-       | Boost Converter Input - |

---

# Important TP4056 Notes

Recommended:

* use the protected TP4056 version,
* ensure proper polarity,
* avoid loose battery connections.

TP4056 documentation and reference:

* [TP4056 Reference Documentation](https://www.datasheet-pdf.info/4226?utm_source=chatgpt.com)

---

# Charging Validation

Connect USB power to the TP4056.

Expected behavior:

| LED Indicator  | Meaning       |
| -------------- | ------------- |
| Red LED        | Charging      |
| Blue/Green LED | Fully charged |

---

# Validation Checkpoint

| Test                      | Expected Result |
| ------------------------- | --------------- |
| Charging LED activates    | Successful      |
| Battery voltage increases | Successful      |
| No overheating            | Stable          |

---

# Stage 3 — MT3608 Boost Converter Setup

The ESP32 and some peripherals require a stable regulated voltage source.

The MT3608 is used to boost the battery voltage to the required operating voltage.

---

# Important Warning

DO NOT connect the boost converter to the ESP32 before adjusting the output voltage.

Improper voltage may permanently damage the ESP32.

---

# MT3608 Wiring

| MT3608 Pin | Connection    |
| ---------- | ------------- |
| IN+        | TP4056 OUT+   |
| IN-        | TP4056 OUT-   |
| OUT+       | ESP32 5V Pin  |
| OUT-       | Common Ground |

---

# Adjusting the Output Voltage

Before connecting the ESP32:

1. Power the MT3608
2. Use a multimeter
3. Rotate the adjustment potentiometer slowly
4. Set output voltage carefully

Recommended output:

```text id="mjw9j7"
5.0V
```

---

# Validation Checkpoint

| Test              | Expected Result |
| ----------------- | --------------- |
| Output voltage    | ~5.0V           |
| Voltage stability | Stable          |
| No overheating    | Stable          |

---

# Stage 3.5 — Power Rail Stabilization Capacitor

To improve system stability during:

- Wi-Fi transmission,
- SD card writes,
- boot current spikes,
- and upload operations,

a bulk electrolytic capacitor is recommended across the regulated ESP32 power rail.

---

# Recommended Capacitor

| Component | Recommended Value |
|---|---|
| Electrolytic Capacitor | 470µF |
| Voltage Rating | ≥ 10V |

---

# Recommended Placement

Connect the capacitor across:

```text
ESP32 5V ↔ GND
```

or equivalently:

```text
MT3608 OUT+ ↔ MT3608 OUT-
```

---

# Wiring Diagram

```text
MT3608 OUT+  ───────┬──────────→ ESP32 5V
                    │
                 470µF
               Capacitor
                    │
MT3608 OUT-  ───────┴──────────→ ESP32 GND
```

---

# Important Polarity Warning

Electrolytic capacitors are polarized.

Ensure:

| Capacitor Leg | Connection |
|---|---|
| Positive (+) | 5V Rail |
| Negative (-) | GND |

Incorrect polarity may damage the capacitor.

---

# Why This Capacitor Helps

The capacitor helps:

- reduce transient voltage drops,
- improve ESP32 stability,
- reduce brownouts,
- stabilize SD card writes,
- and improve Wi-Fi reliability.

This is especially useful in portable battery-powered ESP32 systems.

---

# Validation Checkpoint

After capacitor installation:

| Test | Expected Result |
|---|---|
| ESP32 boot stability | Improved |
| Wi-Fi upload stability | Improved |
| Reduced random resets | Improved |
| SD reliability | Improved |

---

# Stage 4 — Power Switch Integration

A physical power switch is strongly recommended.

This allows:

* safe shutdown,
* battery isolation,
* and portable usability.

---

# Recommended Placement

Place the switch:

```text id="i6u8qn"
Between TP4056 OUT+ and MT3608 IN+
```

This isolates downstream power consumption.

---

# Validation Checkpoint

| Test                        | Expected Result |
| --------------------------- | --------------- |
| Switch powers system on/off | Successful      |
| No intermittent power loss  | Stable          |

---

# Stage 5 — ESP32 Power Validation

After validating regulated voltage:

1. Connect the MT3608 output to the ESP32 5V pin
2. Power on the system
3. Observe serial monitor

Expected output:

```text id="g4b8yk"
System Boot Successful
WiFi Initialized
SD Mounted
System Ready
```

---

# Important ESP32 Power Notes

The ESP32 may draw significant current during:

* Wi-Fi transmission,
* SD writes,
* and boot initialization.

Weak power systems may cause:

* brownouts,
* unexpected resets,
* or SD corruption.

---

# Brownout Testing

Observe the system during:

* Wi-Fi uploads,
* long recordings,
* and repeated recording cycles.

Watch for:

| Symptom           | Possible Cause     |
| ----------------- | ------------------ |
| Unexpected reboot | Voltage drop       |
| SD corruption     | Power instability  |
| Upload failure    | Current starvation |
| Boot loops        | Weak battery       |

---

# Recommended Stability Improvements

If instability occurs:

* shorten power wiring,
* improve solder joints,
* increase battery capacity,
* improve grounding,
* or add bulk capacitance near the ESP32.

Optional improvement:

```text id="c4u1zk"
470µF capacitor across ESP32 power rails
```

This may help reduce transient voltage drops.

---

# Power System Validation Checklist

| Validation Item               | Expected Result |
| ----------------------------- | --------------- |
| Battery charging works        | Successful      |
| Boost converter stable        | Successful      |
| ESP32 boots reliably          | Stable          |
| No brownouts                  | Stable          |
| Uploads complete successfully | Stable          |
| Long recordings remain stable | Stable          |

---

# Common Power System Issues

| Problem                    | Possible Cause           | Suggested Fix                    |
| -------------------------- | ------------------------ | -------------------------------- |
| ESP32 resets during upload | Voltage sag              | Improve battery/current delivery |
| SD corruption              | Unstable power rail      | Improve regulation               |
| TP4056 overheating         | Incorrect wiring         | Verify polarity                  |
| MT3608 output unstable     | Poor adjustment          | Recalibrate output voltage       |
| Device does not boot       | Incorrect output voltage | Measure with multimeter          |
| Random reboot loops        | Weak USB/battery supply  | Improve power source             |

---

# Recommended Next Step

Once the portable power subsystem is validated successfully, proceed to backend infrastructure deployment.

Continue to:

→ [Backend Deployment Guide](../04-backend/deployment-guide.md)

Then:

→ [Shared Drive Setup Guide](../04-backend/shared-drive-integration.md)

---

# Additional Resources

## ESP32 Power Guidelines

* [ESP32 Hardware Design Guidelines](https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32/esp-hardware-design-guidelines-en-master-esp32.pdf?utm_source=chatgpt.com)

## TP4056 Overview

* [TP4056 Module Overview](https://components101.com/modules/tp4056a-li-ion-battery-chargingdischarging-module?utm_source=chatgpt.com)

## MT3608 Overview

* [MT3608 Boost Converter Overview](https://components101.com/modules/mt3608-2a-dc-dc-step-up-power-module?utm_source=chatgpt.com)

## Multimeter Basics

* [How to Use a Multimeter](https://learn.sparkfun.com/tutorials/how-to-use-a-multimeter/all?utm_source=chatgpt.com)
