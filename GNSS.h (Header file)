#ifndef GNSS_H
#define GNSS_H

#include <HardwareSerial.h>
#include <TinyGPS++.h>

class GNSS {
public:
    GNSS(int rxPin, int txPin, long baudRate);
    void begin();
    void update();
    bool isMoving();
    float getLatitude();
    float getLongitude();

private:
    HardwareSerial SerialGPS;
    TinyGPSPlus gps;
    float lastLat, lastLng;
    bool firstFix;
    float haversineDistance(float lat1, float lon1, float lat2, float lon2);
};

#endif // GNSS_H
