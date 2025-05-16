#include "MotionDetector.h"

#define DEVICE_ADDR 0x53

MotionDetector detector(DEVICE_ADDR);

void setup() {
  Serial.begin(9600);
  detector.begin();
}

void loop() {
  if (detector.isMoving()) {
    Serial.println("Moving!");
  } else {
    Serial.println("Still.");
  }

  delay(1000);
}
