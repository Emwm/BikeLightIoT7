#include "powerModes.h"
#include "buttonHandler.h" // Include buttonHandler.h to access lastActivityTime
#include "lightControl.h"
#include "GNSSHandler.h"

PowerMode currentMode = ACTIVE;
const unsigned long inactivityTimeout = 30000; // 30 sec auto-sleep timeout
volatile bool sendLocationFlag = false;

void setupPowerModes() {
    lastActivityTime = millis(); // Use shared lastActivityTime
    attachInterrupt(BUTTON_PIN_1, handleInterrupt1, RISING);
    attachInterrupt(BUTTON_PIN_2, handleInterrupt2, RISING);
}

void IRAM_ATTR handleInterrupt1() {
    static unsigned long lastInterruptTime = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastInterruptTime > 400) { // Debounce delay (200ms)
        button1Pressed = true;
        lastActivityTime = millis(); // Reset inactivity timer
    }
    lastInterruptTime = currentTime;
    //GNSS.update();  // GPS Update
    sendLocationFlag = true;
    Serial.println("GPS Update");
    if (currentMode == ACTIVE) {
            currentMode = PARK;
            Serial.println("Switched to PARK mode.");
        } else if (currentMode == PARK) {
            currentMode = ACTIVE;
            Serial.println("Switched to ACTIVE mode.");
        }
}

void IRAM_ATTR handleInterrupt2() {
    static unsigned long lastInterruptTime = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastInterruptTime > 400) { // Debounce delay (200ms)
        button2Pressed = true;
        lastActivityTime = millis(); // Reset inactivity timer
    }
    lastInterruptTime = currentTime;
    //GNSS.update();  // GPS Update
    Serial.println("GPS Update");
    if (currentMode == ACTIVE) {
        currentMode = SLEEP;
        Serial.println("Switched to SLEEP mode.");
    } else if (currentMode == PARK) {
        currentMode = ACTIVE;
        Serial.println("Switched to ACTIVE mode.");
    }
}

void configureWakeupSources() {
    uint64_t wakeup_pins = (1ULL << BUTTON_PIN_1) | (1ULL << BUTTON_PIN_3);

    // Use ext1 wakeup for multiple buttons
    esp_sleep_enable_ext1_wakeup(wakeup_pins, ESP_EXT1_WAKEUP_ANY_HIGH);
}

PowerMode getCurrentMode() {
    return currentMode;
}

void checkInactivity() {
    if (millis() - lastActivityTime >= inactivityTimeout) {
        Serial.println("No activity detected for 30 seconds. Entering Sleep Mode.");
        currentMode = PARK;
        //GNSS.update();  // GPS Update
        sendLocationFlag = true;
    }
}

void activeMode() {
    Serial.println("ESP32 in ACTIVE mode.");
    controlLight();
    delay(1000);
}

void parkMode() {
    Serial.println("ESP32 in PARK mode.");
    turnLightoff();

    configureWakeupSources();  // Set wakeup sources dynamically

    delay(2000);
    Serial.println("Entering Light Sleep...");
    delay(1000);
    esp_light_sleep_start();

    Serial.println("Woke up from Light Sleep.");
}

void sleepMode() {
    Serial.println("ESP32 entering SLEEP mode.");
    turnLightoff();

    configureWakeupSources();  // Set wakeup sources dynamically

    delay(1000);
    Serial.println("Going into Deep Sleep now...");
    esp_deep_sleep_start();

    Serial.println("Woke up from Deep Sleep.");
}
