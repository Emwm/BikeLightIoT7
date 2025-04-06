#include "powerModes.h"
#include "buttonHandler.h"
#include "lightControl.h"
#include "GNSS.h"

void setup() {
    Serial.begin(115200);
    setupButtons();    // Initialize buttons and interrupts
    setupPowerModes(); // Initialize power modes
    setupLight();      // Initialize light control
    //GNSS.GNSS()        // Initialize GNSS

    Serial.println("ESP32 Ready!");
}

void loop() {
    checkButtonPresses();  // Handle button events
    checkInactivity();      // Auto turn-off after 30 sec of inactivity

    // Handle different power modes
    switch (getCurrentMode()) {
        case ACTIVE:
            activeMode();
            //if(GNSS.moving() == true) // Checking if bike is moving
            {
              //lastActivityTime = millis();
            }
            Serial.println("Check movement");
            break;
        case PARK:
            parkMode();
            break;
        case SLEEP:
            sleepMode();
            break;
    }
}
