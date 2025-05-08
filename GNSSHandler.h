#ifndef GNSS_HANDLER_H
#define GNSS_HANDLER_H

#include <HardwareSerial.h>
#include <HT_TinyGPS++.h>  // Library to parse GPS data

// ---- Struct: GPSPosition ----
// Holds latitude and longitude of the current GPS position
struct GPSPosition {
    float latitude;  // Latitude in decimal degrees
    float longitude; // Longitude in decimal degrees
};

class GNSSHandler {
public:
    // ---- Constructor: Initialize GNSS Handler ----
    // Takes a HardwareSerial object and RX/TX pin numbers for GPS communication
    GNSSHandler(HardwareSerial &serial, int rxPin, int txPin);

    // ---- Begin GPS operation ----
    // Initializes serial communication with GPS module
    void begin();

    // ---- Update GPS data ----
    // Reads incoming GPS data from the serial interface
    void update();

    // ---- Check GPS connection ----
    // Returns true if the GPS has connected to enough satellites for a valid fix
    bool isConnected();

    // ---- Get Current GPS Location ----
    // Returns the current location as a GPSPosition struct
    GPSPosition checkLocation();

private:
    HardwareSerial &gpsSerial; // Reference to the GPS serial interface (e.g., Serial1)
    TinyGPSPlus gps;           // GPS instance to parse incoming GPS data
    float lastLat, lastLng;    // Stores the last known latitude and longitude
    bool firstFix;             // Flag to indicate if the first GPS fix has been achieved

    // Optionally: Additional private variables can be added as needed for handling other GPS parameters or state
};

#endif
