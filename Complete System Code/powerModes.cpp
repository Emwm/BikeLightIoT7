#include "powerModes.h"
#include "buttonHandler.h"  // Access shared button flags and lastActivityTime
#include "lightControl.h"
#include "GNSSHandler.h"

// ---- Global Variables ----
PowerMode currentMode = ACTIVE;                   // Tracks current power state
const unsigned long inactivityTimeout = 60000;    // Auto-sleep timeout (60 seconds)
volatile bool sendLocationFlag = false;           // Flag to trigger GPS location sending

// ---- Initialize Power Modes and Attach Interrupts ----
void setupPowerModes() {
    lastActivityTime = millis(); // Initialize last activity timestamp

    // Attach interrupts to handle button presses
    attachInterrupt(BUTTON_PIN_1, handleInterrupt1, RISING);
}

// ---- ISR: Handle Button 1 Press ----
void IRAM_ATTR handleInterrupt1() {
    static unsigned long lastInterruptTime = 0;
    unsigned long currentTime = millis();

    // Debounce: Ignore presses within 4000ms
    if (currentTime - lastInterruptTime > 4000) {
        button1Pressed = true;
        lastActivityTime = millis(); // Reset inactivity timer
    }
    lastInterruptTime = currentTime;

    sendLocationFlag = true;               // Flag for GPS update
    Serial.println("GPS Update");

    // Toggle between ACTIVE and SLEEP modes
    if (getCurrentMode() == ACTIVE) {
        setCurrentMode(22);
        Serial.println("Switched to PARK mode.");
    } else if (getCurrentMode() == PARK) {
        setCurrentMode(33);
        Serial.println("Switched to SLEEP mode.");
    }
}

// ---- Configure Wakeup Sources for Sleep Modes ----
void configureWakeupSources() {
    // Enable BUTTON_PIN_1 and BUTTON_PIN_3 as wakeup sources
    uint64_t wakeup_pins = (1ULL << BUTTON_PIN_1);
    esp_sleep_enable_ext1_wakeup(wakeup_pins, ESP_EXT1_WAKEUP_ANY_HIGH);
    
}

// ---- Return Current Power Mode ----
PowerMode getCurrentMode() {
    return currentMode;
}

// ---- Set Current Power Mode ----
void setCurrentMode(int powermode) { 
    if (powermode == 11){
        currentMode = ACTIVE;
        sendLocationFlag = true;               // Flag for GPS update
    } else if (powermode == 22) {
        currentMode = PARK;
        sendLocationFlag = true;               // Flag for GPS update
    } else if (powermode == 33) {
        currentMode = SLEEP;
        sendLocationFlag = true;               // Flag for GPS update
    }
}

// ---- Monitor Inactivity and Trigger Auto-Sleep ----
void checkInactivity() {
    if (millis() - lastActivityTime >= inactivityTimeout) {
        Serial.println("No activity detected for 30 seconds. Entering Sleep Mode.");
        currentMode = PARK;
        sendLocationFlag = true;  // GPS update on inactivity
    }
}

// ---- ACTIVE Mode Operation ----
void activeMode() {
    Serial.println("ESP32 in ACTIVE mode.");
    controlLight();       // Adjust light based on ambient level
    delay(1000);          // Allow time for reading/display
}

// ---- PARK Mode Operation ----
void parkMode() {
    Serial.println("ESP32 in PARK mode.");
    turnLightoff();           // Ensure light is off
    configureWakeupSources(); // Setup wakeup GPIOs
    esp_sleep_enable_timer_wakeup(30 * 1000000ULL); // Wake after 30 seconds
    delay(2000);
    Serial.println("Entering Light Sleep...");
    delay(1000);
    esp_light_sleep_start();  // Enter light sleep
    Serial.println("Woke up from Light Sleep.");
}

// ---- SLEEP Mode Operation ----
void sleepMode() {
    Serial.println("ESP32 entering SLEEP mode.");
    turnLightoff();           // Ensure light is off
    configureWakeupSources(); // Setup wakeup GPIOs
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);

    delay(1000);
    Serial.println("Going into Deep Sleep now...");
    esp_deep_sleep_start();   // Enter deep sleep (resets on wake)

    // This line should never execute unless sleep fails
    Serial.println("Woke up from Deep Sleep.");
}
