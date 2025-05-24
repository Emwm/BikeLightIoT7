#ifndef MOTION_DETECTOR_H
#define MOTION_DETECTOR_H

#include <Arduino.h>  // Include the Arduino core library for standard functions

// MotionDetector class to interface with an accelerometer or motion detection sensor
class MotionDetector {
public:
  // Constructor: Initializes the motion detector with the device address and optional threshold
  // deviceAddress: The I2C address or unique identifier of the motion detector device
  // threshold: The sensitivity level for detecting movement (default is 50)
  MotionDetector(uint8_t deviceAddress, int threshold = 50);

  // Begin: Initializes the motion detector sensor (setup, communication, etc.)
  void begin();

  // isMoving: Checks if motion is detected based on current sensor readings
  // Returns true if motion is detected, otherwise false
  bool isMoving();

private:
  // Helper function: Write a value to a specific register/address in the motion sensor
  void writeTo(byte address, byte val);

  // Helper function: Read data from a specific register/address in the motion sensor
  void readFrom(byte address, int num, byte* buff);

  const uint8_t device;  // I2C address or identifier for the motion sensor device
  const int threshold;   // Threshold value to determine if movement is detected
  static const int toRead = 6;  // Number of bytes to read from the sensor
  byte buff[toRead];     // Buffer to store raw data from the sensor
  int prevX, prevY, prevZ;  // Store previous X, Y, Z axis readings for movement comparison
};

#endif
