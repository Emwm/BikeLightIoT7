# 🚲 ESP32 Smart Bike Tracker

An ESP32-based smart tracking and monitoring system for bicycles, combining GNSS (GPS) tracking, motion detection, power management, battery monitoring, and LoRa communication to create an intelligent, low-power system for bike security and telemetry.

---

## 📚 Table of Contents

- [Features](#-features)
- [Getting Started](#-getting-started)
- [Power Modes](#-power-modes)
- [GNSS (GPS) Handler](#-gnss-gps-handler)
- [Motion Detector](#-motion-detector)
- [Light Control](#-light-control)
- [LoRa Communication](#-lora-communication)
- [Battery Monitoring](#-battery-monitoring)
- [Inactivity Handling](#-inactivity-handling)
- [Main Loop Overview](#-main-loop-overview)
- [Modules Summary](#-modules-summary)
- [Contributing](#-contributing)
- [License](#-license)

---

## 📦 Features

- Real-time GPS tracking  
- LoRa communication to send bike data remotely  
- Motion detection using an accelerometer  
- Battery monitoring via MAX17048 fuel gauge  
- Smart power management with 3 modes: ACTIVE, PARK, SLEEP  
- Inactivity-based sleep transitions  
- Wake-up on button press or timer  
- Integrated button and light control  

---

## 🛠️ Getting Started

### ✅ Prerequisites

- PlatformIO or Arduino IDE  
- ESP32 board definitions installed  
- Required libraries:  
  - TinyGPS++  
  - Wire  
  - SPI  

### 🚀 Flashing the Firmware

```bash
git clone https://github.com/youruser/esp32-smart-bike-tracker.git
cd esp32-smart-bike-tracker
# Open with Arduino IDE or PlatformIO and upload to your ESP32
```

---

## 🔘 Button Handler

Manages button input from GPIO 33, used to:
- Wake up the system
- Trigger GPS updates
- Toggle power modes

### ⚙️ Configuration

| Constant        | Description                  |
|----------------|------------------------------|
| `BUTTON_PIN_1` | GPIO pin for Button 1 (GPIO33) |

```cpp
#define BUTTON_PIN_1 33
```

### 📦 Variables

```cpp
extern volatile bool button1Pressed;
extern unsigned long lastActivityTime;
```

### 📋 API

| Function            | Description                              |
|---------------------|------------------------------------------|
| `setupButtons()`    | Initializes GPIOs as `INPUT_PULLDOWN`    |
| `checkButtonPresses()` | Resets flags, updates `lastActivityTime` |

---

## 🧠 Power Modes

System behavior varies by mode:

### 🟢 ACTIVE Mode
- Full GPS + LoRa operation
- Motion detection and lighting enabled
- Auto-exit on inactivity

### 🟡 PARK Mode
- Enters light sleep
- Periodic wake via RTC or button
- Ideal for short-term stationary periods

### 🔴 SLEEP Mode
- Enters deep sleep for max power saving
- Wake on button press only
- Best for long inactivity

---

## 🛰 GNSS (GPS) Handler

Handles GPS data using the TinyGPS++ library. Sends the location everytime the mode of the system changes.

### 📋 API

| Method              | Description                                |
|---------------------|--------------------------------------------|
| `begin()`           | Initializes GPS module                     |
| `update()`          | Feeds serial data to TinyGPS++             |
| `isConnected()`     | Checks for GPS fix (≥4 satellites)         |
| `checkLocation()`   | Returns `GPSPosition` with lat/lon         |

### 📍 Example

This flag is used to update the location and send it.

```cpp
if (sendLocationFlag) {
    GPSPosition pos = gnss.checkLocation();
    // Use pos.latitude, pos.longitude
}
```

---

## 🕹️ Motion Detector

Detects bike movement using a 3-axis accelerometer (e.g. ADXL345).

### 📋 API

| Method                  | Description                                  |
|--------------------------|----------------------------------------------|
| `isMoving()`             | Returns `true` if movement detected          |
| `begin()`                | Initializes the sensor                       |
| `MotionDetector(...)`    | Constructor with I2C address and threshold   |

```cpp
MotionDetector motion(0x53); // ADXL345 default I2C address
```

---

## 💡 Light Control

Uses an LDR + MAX7219 LED matrix to control lights.

### ⚙️ Configuration

| Item               | Pin          |
|--------------------|--------------|
| `LDR_PIN`          | Analog Input |
| `LED_LOW_BAT`      | Digital Output |
| `DIN`              | GPIO 14      |
| `CLK`              | GPIO 13      |
| `CS`               | GPIO 12      |

### 📋 API

| Function                      | Description                               |
|-------------------------------|-------------------------------------------|
| `setupLight()`                | Initializes LED matrix                    |
| `controlLight()`              | Enables/disables display based on LDR     |
| `turnLightoff()`              | Manually disables display                 |
| `manageBatteryWarning(float)`| Activates LED when battery < 25%          |

---

## 📡 LoRaWAN Communication

Handles uplink/downlink communication via RN2483 LoRa module using OTAA.

### 🔧 ESP32 to RN2483 Wiring

| Function | ESP32 Pin |
|----------|-----------|
| TX → RN  | GPIO 19   |
| RX ← RN  | GPIO 18   |
| RESET    | GPIO 23   |

### 📋 API

| Function                                  | Description                         |
|-------------------------------------------|-------------------------------------|
| `init_lora()`                             | Initializes LoRaWAN + joins TTN     |
| `send_lora(devMode, battery, lon, lat)`   | Sends telemetry as CSV string       |
| `recieve_lora()`                          | Reads JSON downlink and parses value|

### 📤 Uplink Format (CSV)

```text
1,3.85,12.34,56.78
```

### 📥 Downlink Format

TTN → JSON string in HEX:

```json
{"value": 2}
```

TTN Payload: `7b2276616c7565223a20327d`

Parsed into `int value = 2`.

---

## 🔋 Battery Monitoring

Powered by the MAX17048 fuel gauge.

```cpp
batterypercentage = battery.percent();
```

Low battery warning is triggered by:

```cpp
manageBatteryWarning(batterypercentage);
```

---

## ⏱️ Inactivity & Mode Handling

Automatically switches to `PARK` mode if idle for 60 seconds:

```cpp
if (millis() - lastActivityTime > 60000) {
    // Switch to PARK
}
```

Button interrupt handler toggles modes:

```cpp
void IRAM_ATTR handleInterrupt1();
```

Transitions:
- `ACTIVE → PARK`
- `PARK → SLEEP`

---

## 🔁 Main Loop Structure

```cpp
switch (getCurrentMode()) {
    case ACTIVE:
        // Motion + LoRa
        break;
    case PARK:
        // Light sleep
        break;
    case SLEEP:
        // Deep sleep
        break;
}
```

---

## 🛠 Modules Summary

| Module         | Description                            |
| -------------- | -------------------------------------- |
| powerModes     | Manages sleep states and wake sources  |
| GNSSHandler    | Handles GPS via GNSS                   |
| MotionDetector | Reads accelerometer to detect movement |
| ESP32\_lora    | Sends and receives data via LoRa       |
| MAX17048       | Monitors battery charge level          |
| buttonHandler  | Detects button presses                 |
| lightControl   | Manages LED or light control           |

## Authors

Group 7 in the DTU course 34346 Networking technologies and application development for IoT


