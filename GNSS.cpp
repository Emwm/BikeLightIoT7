#include "GNSS.h"

GNSS::GNSS(int rxPin, int txPin, long baudRate) 
    : SerialGPS(1), lastLat(0.0), lastLng(0.0), firstFix(false) {
    SerialGPS.begin(baudRate, SERIAL_8N1, rxPin, txPin);
}

void GNSS::begin() {
    Serial.begin(115200);
    Serial.println("Waiting for GPS fix...");
}

void GNSS::update() {
    if (SerialGPS.available()) {
        char c = SerialGPS.read();
        gps.encode(c);

        if (gps.location.isUpdated()) {
            float currentLat = gps.location.lat();
            float currentLng = gps.location.lng();

            if (!firstFix) {
                lastLat = currentLat;
                lastLng = currentLng;
                firstFix = true;
                Serial.println("First GPS fix acquired!");
            }

            float distanceMoved = haversineDistance(lastLat, lastLng, currentLat, currentLng);

            Serial.print("Current Location: ");
            Serial.print(currentLat, 6);
            Serial.print(", ");
            Serial.println(currentLng, 6);

            if (distanceMoved > 0.1) {  // 10 cm threshold
                Serial.println("🚴 Movement detected (small movement)!");
                lastLat = currentLat;
                lastLng = currentLng;
            } else {
                Serial.println("📍 Device is stationary.");
            }

            Serial.println("--------------------");
        }
    }
}

bool GNSS::isMoving() {
    return haversineDistance(lastLat, lastLng, gps.location.lat(), gps.location.lng()) > 0.1;
}

float GNSS::getLatitude() {
    return gps.location.lat();
}

float GNSS::getLongitude() {
    return gps.location.lng();
}

float GNSS::haversineDistance(float lat1, float lon1, float lat2, float lon2) {
    const float R = 6371000; // Earth radius in meters
    float dLat = radians(lat2 - lat1);
    float dLon = radians(lon2 - lon1);
    float a = sin(dLat/2) * sin(dLat/2) + cos(radians(lat1)) * cos(radians(lat2)) * sin(dLon/2) * sin(dLon/2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * c;
}
