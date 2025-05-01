#ifndef POWER_MODES_H
#define POWER_MODES_H

#include <Arduino.h>
#include "esp_sleep.h" // Required for ESP32 sleep functions

// Enum to define the three power modes of the system
enum PowerMode { ACTIVE, PARK, SLEEP };

// External flag used to signal when GPS location should be sent
extern volatile bool sendLocationFlag;

// ---- Function Declarations ----

// Initializes interrupts and sets the starting power mode
void setupPowerModes();

// Interrupt Service Routine for button 1 (used to toggle modes and GPS update)
void IRAM_ATTR handleInterrupt1();

// Interrupt Service Routine for button 2 (used to enter sleep and toggle modes)
void IRAM_ATTR handleInterrupt2();

// Configures GPIO pins as wakeup sources for light or deep sleep
void configureWakeupSources();

// Checks for user inactivity and triggers mode change if necessary
void checkInactivity();

// Returns the current power mode (ACTIVE, PARK, or SLEEP)
PowerMode getCurrentMode();

// Behavior while in ACTIVE mode (e.g., running light logic)
void activeMode();

// Behavior while in PARK mode (light sleep, wake on button)
void parkMode();

// Behavior while in SLEEP mode (deep sleep, wake on button)
void sleepMode();

#endif
