#include "GNSSHandler.h"

GNSSHandler gnssHandler(Serial1, 16, 17);  // Replace with actual RX and TX pins

void setup() {
    Serial.begin(9600);
    gnssHandler.begin();
}

void loop() {
    gnssHandler.update();  // Continuously read incoming GPS data

    if (gnssHandler.isConnected()) {
        // Only check location if GPS is connected (fix + enough satellites)
        GPSPosition pos = gnssHandler.checkLocation();

        // Print the latest location
        Serial.print("Latitude: ");
        Serial.print(pos.latitude, 6);
        Serial.print(", Longitude: ");
        Serial.println(pos.longitude, 6);
    } else {
        Serial.println("Waiting for valid GPS signal...");
    }

    delay(1000);  // Wait 1 second before the next loop
}
