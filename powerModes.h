#ifndef POWER_MODES_H
#define POWER_MODES_H

#include <Arduino.h>
#include "esp_sleep.h"

enum PowerMode { ACTIVE, PARK, SLEEP };

void setupPowerModes();  // Initialize power modes
void IRAM_ATTR handleInterrupt1();
void IRAM_ATTR handleInterrupt2();
void checkInactivity();  // Auto sleep after 30 sec of inactivity
PowerMode getCurrentMode(); // Get current power mode
void activeMode();       // Active mode operation
void parkMode();         // Park mode operation
void sleepMode();        // Sleep mode operation

#endif
