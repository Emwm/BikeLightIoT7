
#include <Wire.h>

#define DEVICE 0x53  // ADXL345 device address
#define TO_READ 6    // Number of bytes to read (two bytes per axis)
#define LED_PIN 23   // GPIO pin connected to the LED

byte buff[TO_READ];  // Buffer to store data read from the sensor
int prevX, prevY, prevZ;  // To store previous X, Y, Z values
unsigned long lastMovementTime = 0;
unsigned long movementTimeout = 5000;  // 5 seconds timeout

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  
  writeTo(DEVICE, 0x2D, 0x08); // Set the device to measurement mode
}

void loop()
{
  readFrom(DEVICE, 0x32, TO_READ, buff); // Read accelerometer data
  
  // Get X, Y, Z values from the buffer
  int x = (((int)buff[1]) << 8) | buff[0];
  int y = (((int)buff[3]) << 8) | buff[2];
  int z = (((int)buff[5]) << 8) | buff[4];

  // Output the acceleration data to serial
  Serial.print("X="); Serial.print(x);
  Serial.print(" Y="); Serial.print(y);
  Serial.print(" Z="); Serial.println(z);

  // Check if movement is detected
  if (abs(x - prevX) > 50 || abs(y - prevY) > 50 || abs(z - prevZ) > 50) {
    digitalWrite(LED_PIN, LOW);  // 
    lastMovementTime = millis(); // Reset timer
  }

  if (millis() - lastMovementTime >= movementTimeout) {
    digitalWrite(LED_PIN, HIGH);
  }

  // Update previous values for the next loop
  prevX = x; prevY = y; prevZ = z;

  delay(300);  // Delay between reads
}

void writeTo(int device, byte address, byte val)
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}

void readFrom(int device, byte address, int num, byte buff[])
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.beginTransmission(device);
  Wire.requestFrom(device, num);
  
  int i = 0;
  while (Wire.available()) {
    buff[i++] = Wire.read();
  }
  Wire.endTransmission();
}
