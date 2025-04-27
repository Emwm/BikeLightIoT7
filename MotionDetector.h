#ifndef MOTION_DETECTOR_H
#define MOTION_DETECTOR_H

#include <Arduino.h>

class MotionDetector {
public:
  MotionDetector(uint8_t deviceAddress, int threshold = 50);
  void begin();
  bool isMoving();

private:
  void writeTo(byte address, byte val);
  void readFrom(byte address, int num, byte* buff);

  const uint8_t device;
  const int threshold;
  static const int toRead = 6;
  byte buff[toRead];
  int prevX, prevY, prevZ;
};

#endif
