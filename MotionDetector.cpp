#include "MotionDetector.h"
#include <Wire.h>

MotionDetector::MotionDetector(uint8_t deviceAddress, int threshold)
  : device(deviceAddress), threshold(threshold), prevX(0), prevY(0), prevZ(0) {}

void MotionDetector::begin() {
  Wire.begin();
  writeTo(0x2D, 0x08);  // Set device to measurement mode
}

bool MotionDetector::isMoving() {
  readFrom(0x32, toRead, buff);

  int x = (((int)buff[1]) << 8) | buff[0];
  int y = (((int)buff[3]) << 8) | buff[2];
  int z = (((int)buff[5]) << 8) | buff[4];

  Serial.print("X="); Serial.print(x);
  Serial.print(" Y="); Serial.print(y);
  Serial.print(" Z="); Serial.println(z);

  bool moved = (abs(x - prevX) > threshold ||
                abs(y - prevY) > threshold ||
                abs(z - prevZ) > threshold);

  prevX = x;
  prevY = y;
  prevZ = z;

  return moved;
}

void MotionDetector::writeTo(byte address, byte val) {
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}

void MotionDetector::readFrom(byte address, int num, byte* buff) {
  // Write register address
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission(false); // Important: use 'false' to send repeated start

  // Request bytes
  Wire.requestFrom(device, num);
  int i = 0;
  while (Wire.available() && i < num) {
    buff[i++] = Wire.read();
  }
}
