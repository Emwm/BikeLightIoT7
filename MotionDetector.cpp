#include "MotionDetector.h"
#include <Wire.h>

// Constructor: Initializes the MotionDetector object
MotionDetector::MotionDetector(uint8_t deviceAddress, int threshold)
  : device(deviceAddress), threshold(threshold), prevX(0), prevY(0), prevZ(0) {}

// Initializes the I2C communication and sets the sensor to measurement mode
void MotionDetector::begin() {
  Wire.begin(); // Start I2C communication
  writeTo(0x2D, 0x08);  // Set the device to measurement mode (specific to your sensor)
}

// Checks if the device is moving by comparing the current accelerometer data with the previous data
bool MotionDetector::isMoving() {
  readFrom(0x32, toRead, buff); // Read the accelerometer data from the sensor

  // Extract X, Y, and Z axis data from the buffer
  int x = (((int)buff[1]) << 8) | buff[0];  // Combine the high and low bytes for X axis data
  int y = (((int)buff[3]) << 8) | buff[2];  // Combine the high and low bytes for Y axis data
  int z = (((int)buff[5]) << 8) | buff[4];  // Combine the high and low bytes for Z axis data

  // Print the X, Y, and Z values for debugging
  Serial.print("X="); Serial.print(x);
  Serial.print(" Y="); Serial.print(y);
  Serial.print(" Z="); Serial.println(z);

  // Check if the change in X, Y, or Z exceeds the threshold, indicating movement
  bool moved = (abs(x - prevX) > threshold ||
                abs(y - prevY) > threshold ||
                abs(z - prevZ) > threshold);

  // Update previous X, Y, and Z values to the current ones
  prevX = x;
  prevY = y;
  prevZ = z;

  return moved;  // Return true if movement was detected, false otherwise
}

// Writes a byte of data to the sensor at a specific register address
void MotionDetector::writeTo(byte address, byte val) {
  Wire.beginTransmission(device); // Start communication with the device
  Wire.write(address);             // Send the register address
  Wire.write(val);                 // Send the value to write
  Wire.endTransmission();          // End the transmission to the device
}

// Reads a specified number of bytes from the sensor at a given register address
void MotionDetector::readFrom(byte address, int num, byte* buff) {
  Wire.beginTransmission(device);  // Start communication with the device
  Wire.write(address);             // Send the register address to start reading from
  Wire.endTransmission(false);     // Use repeated start condition (important for I2C)
  
  // Request the specified number of bytes from the device
  Wire.requestFrom(device, num);
  int i = 0;
  // Read the bytes and store them in the buffer
  while (Wire.available() && i < num) {
    buff[i++] = Wire.read();  // Read and store data in the buffer
  }
}
