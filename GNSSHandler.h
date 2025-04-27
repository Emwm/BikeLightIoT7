#ifndef GNSS_HANDLER_H
#define GNSS_HANDLER_H

#include <HardwareSerial.h>
#include <TinyGPS++.h>

// Define GPSPosition struct to hold latitude and longitude
struct GPSPosition {
    float latitude;
    float longitude;
};

class GNSSHandler {
public:
    // Constructor
    GNSSHandler(HardwareSerial &serial, int rxPin, int txPin);

    // Begin GPS operation
    void begin();

    // Update GPS data from serial input
    void update();

    // Check if there are enough satellites connected
    bool isConnected();

    // Get current location (latitude and longitude)
    GPSPosition checkLocation();

private:
    HardwareSerial &gpsSerial; // Reference to the GPS serial interface
    TinyGPSPlus gps;           // GPS instance
    float lastLat, lastLng;    // Last known position
    bool firstFix;             // Flag to indicate first GPS fix

    // Optional: You can add any other private variables here if needed
};

#endif
