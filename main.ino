#include "powerModes.h"
#include "buttonHandler.h"
#include "lightControl.h"

void setup() {
    Serial.begin(115200);
    setupButtons();    // Initialize buttons and interrupts
    setupPowerModes(); // Initialize power modes
    setupLight();      // Initialize light control

    Serial.println("ESP32 Ready!");
}

void loop() {
    checkButtonPresses();  // Handle button events
    checkInactivity();      // Auto turn-off after 30 sec of inactivity

    // Handle different power modes
    switch (getCurrentMode()) {
        case ACTIVE:
            activeMode();
            break;
        case PARK:
            parkMode();
            break;
        case SLEEP:
            sleepMode();
            break;
    }
}
