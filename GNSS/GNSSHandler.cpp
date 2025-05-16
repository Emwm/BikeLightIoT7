#include "GNSSHandler.h"

GNSSHandler::GNSSHandler(HardwareSerial &serial, int rxPin, int txPin)
    : gpsSerial(serial), lastLat(0.0), lastLng(0.0), firstFix(false)
{
    gpsSerial.begin(9600, SERIAL_8N1, rxPin, txPin);
}

void GNSSHandler::begin()
{
    Serial.println("Waiting for GPS fix...");
}

void GNSSHandler::update()
{
    while (gpsSerial.available())
    {
        char c = gpsSerial.read();
        gps.encode(c);
    }
}

bool GNSSHandler::isConnected()
{
    return gps.location.isValid() && gps.satellites.value() >= 4;
}

GPSPosition GNSSHandler::checkLocation()
{
    GPSPosition position = {0.0, 0.0};

    // Only proceed if a new location is available and enough satellites are detected
    if (gps.location.isUpdated() && gps.satellites.value() >= 4)
    {
        float currentLat = gps.location.lat();
        float currentLng = gps.location.lng();

        Serial.print("Current Location: ");
        Serial.print(currentLat, 6);
        Serial.print(", ");
        Serial.println(currentLng, 6);

        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value());

        Serial.println("--------------------");

        // Update last known position
        lastLat = currentLat;
        lastLng = currentLng;

        position.latitude = currentLat;
        position.longitude = currentLng;
    }

    return position;
}
