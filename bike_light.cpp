#include <Wire.h>

#define DEVICE (0x53) // ADXL345 device address
#define TO_READ (6)   // Number of bytes to read (two bytes per axis)

#define LED_PIN 23  // GPIO pin connected to the LED

byte buff[TO_READ];   // Buffer to store data read from the sensor
int prevX = 65533, prevY = 65494, prevZ = 247; // To store previous X, Y, Z values (Resting value of sensor)
unsigned long lastMovementTime = 0;  // To store the time of the last movement
unsigned long movementTimeout = 5000; // 5 seconds of inactivity to turn off the LED

void setup()
{
  Wire.begin();       // Initialize I2C
  Serial.begin(9600); // Initialize serial communication
  pinMode(LED_PIN, OUTPUT);  // Set GPIO 23 as output
  digitalWrite(LED_PIN, LOW);  // Initially turn off LED
  writeTo(DEVICE, 0x2D, 0x08); // Set the device to measurement mode
}

void loop()
{
  int regAddress = 0x32; // Data register address
  int x, y, z;

  readFrom(DEVICE, regAddress, TO_READ, buff); // Read accelerometer data
  
  // Each axis is a 16-bit value
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3]) << 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];

  // Output the acceleration data to serial
  Serial.print("X=");
  Serial.print(x);
  Serial.print(" Y=");
  Serial.print(y);
  Serial.print(" Z=");
  Serial.println(z);

  // Check if any axis has a change greater than 50
  if (abs(x - prevX) > 50 || abs(y - prevY) > 50 || abs(z - prevZ) > 50) {
    digitalWrite(LED_PIN, LOW);  // Turn LED off when movement detected (reverse logic)
    lastMovementTime = millis();  // Reset the movement timer
  }

  // If no movement for 5 seconds, turn LED on
  if (millis() - lastMovementTime >= movementTimeout) {
    digitalWrite(LED_PIN, HIGH);   // Turn LED on when no movement
  }

  // Update previous values for the next loop
  prevX = x;
  prevY = y;
  prevZ = z;

  delay(300);  // Delay between reads
}

// Function to write to a sensor register
void writeTo(int device, byte address, byte val)
{
  Wire.beginTransmission(device); // Start transmission to device
  Wire.write(address);            // Send register address
  Wire.write(val);                // Send value to write
  Wire.endTransmission();         // End transmission
}

// Function to read multiple bytes from a sensor register
void readFrom(int device, byte address, int num, byte buff[])
{
  Wire.beginTransmission(device); // Start transmission to device
  Wire.write(address);            // Send register address
  Wire.endTransmission();         // End transmission
  
  Wire.beginTransmission(device); // Restart transmission to read data
  Wire.requestFrom(device, num);  // Request data from the device
  
  int i = 0;
  while (Wire.available())        // While data is available
  {
    buff[i] = Wire.read();        // Read byte
    i++;
  }
  Wire.endTransmission();         // End transmission
}
