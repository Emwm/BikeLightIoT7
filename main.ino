#include "powerModes.h"
#include "buttonHandler.h"
#include "lightControl.h"
#include "GNSSHandler.h"
#include "MotionDetector.h"
#include "LoraWAN_Reese.h"

#define GNSS_RX 16
#define GNSS_TX 17
#define DEVICE_ADDR 0x53

GNSSHandler gnss(Serial1, GNSS_RX, GNSS_TX);
MotionDetector detector(DEVICE_ADDR);
GPSPosition pos;



void setup() {
    Serial.begin(115200);
    setupButtons();    // Initialize buttons and interrupts
    setupPowerModes(); // Initialize power modes
    setupLight();      // Initialize light control
    gnss.begin();
    detector.begin();
    initState();
    joinState();

    Serial.println("ESP32 Ready!");
}

void loop() {
    checkButtonPresses();  // Handle button events
    checkInactivity();      // Auto turn-off after 30 sec of inactivity
    sendState();

    if (sendLocationFlag) {
        sendLocationFlag = false;  // Clear the flag

        pos = gnss.checkLocation();
        Serial.print("Latitude: ");
        Serial.print(pos.latitude, 6);
        Serial.print(", Longitude: ");
        Serial.println(pos.longitude, 6);

        // Add your sending logic here
    }

    // Handle different power modes
    switch (getCurrentMode()) {
        case ACTIVE:
            gnss.update();
            if(detector.isMoving() == true) // Checking if bike is moving
            {
              lastActivityTime = millis();
            }
            Serial.println("Check movement");
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
