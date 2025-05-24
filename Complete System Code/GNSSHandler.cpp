#include "GNSSHandler.h"

// ---- Constructor: Initialize GNSSHandler ----
GNSSHandler::GNSSHandler(HardwareSerial &serial, int rxPin, int txPin)
    : gpsSerial(serial), lastLat(0.0), lastLng(0.0), firstFix(false)
{
    // Initialize the serial interface for GPS communication
    gpsSerial.begin(9600, SERIAL_8N1, rxPin, txPin);  // Set baud rate and pins for communication
}

// ---- Begin GPS operation ----
void GNSSHandler::begin()
{
    // Output a message to the serial monitor indicating that GPS is waiting for a fix
    Serial.println("Waiting for GPS fix...");
}

// ---- Update GPS data ----
void GNSSHandler::update()
{
    // Continuously read incoming GPS data and pass it to the GPS parser
    while (gpsSerial.available()) {
        char c = gpsSerial.read();  // Read the incoming byte from the GPS
        gps.encode(c);  // Feed the byte to the GPS parser
    }
}

// ---- Check GPS connection ----
bool GNSSHandler::isConnected()
{
    // Check if the GPS location is valid and the GPS module has connected to at least 4 satellites
    return gps.location.isValid() && gps.satellites.value() >= 4;
}

// ---- Get current GPS location ----
GPSPosition GNSSHandler::checkLocation()
{
    GPSPosition position = {0.0, 0.0};  // Initialize position to 0.0, 0.0 (invalid)

    // Only proceed if there's a valid and updated location and enough satellites are detected
    if (gps.location.isUpdated() && gps.satellites.value() >= 4) {
        float currentLat = gps.location.lat();  // Get the current latitude
        float currentLng = gps.location.lng();  // Get the current longitude

        // Print the current location and satellite information for debugging
        Serial.print("Current Location: ");
        Serial.print(currentLat, 6);  // Print latitude with 6 decimal places
        Serial.print(", ");
        Serial.println(currentLng, 6);  // Print longitude with 6 decimal places

        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value());  // Print number of satellites connected

        Serial.println("--------------------");

        // Update last known position
        lastLat = currentLat;
        lastLng = currentLng;

        // Store the current position to be returned
        position.latitude = currentLat;
        position.longitude = currentLng;
    }

    return position;  // Return the current position, even if it's invalid (0.0, 0.0)
}
