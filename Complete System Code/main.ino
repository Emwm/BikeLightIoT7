#include "powerModes.h"
#include "buttonHandler.h"
#include "lightControl.h"
#include "GNSSHandler.h"
#include "MotionDetector.h"
#include "ESP32_lora.h"
#include "MAX17048.h"

// Define the pins for GNSS module communication
#define GNSS_RX 16
#define GNSS_TX 17
#define DEVICE_ADDR 0x53  // Address of the motion sensor (accelerometer)

// Create instances of the modules for GNSS, motion detection, and GPS position
GNSSHandler gnss(Serial1, GNSS_RX, GNSS_TX);  // Create an object for GNSS communication
MotionDetector detector(DEVICE_ADDR);        // Create an object for the motion detector (accelerometer)
MAX17048 battery;
GPSPosition pos;  // Variable to store the current GPS position
float batterypercentage;


// ---- Setup function: Initializes all components of the system ----
void setup() {
    Serial.begin(115200);       // Start serial communication at 115200 baud rate for debugging

    // Initialize system components
    init_lora();
    setupLight();      // Setup light control and initialize related components
    setupButtons();    // Setup button pins and interrupts
    setupPowerModes(); // Setup power modes and configure interrupts
    gnss.begin();      // Initialize the GNSS (GPS) module
    Wire.begin();  // Initialize I2C
    battery.attatch(Wire);  // Attach MAX17048 to the I2C bus
    detector.begin();  // Initialize the motion detector module (accelerometer)
    

    // Print a ready message once all components are initialized
    Serial.println("ESP32 Ready!");
}

// ---- Main Loop function: Runs continuously to manage system state and handle events ----
void loop() {

    
    // Call button checking to handle button presses and reset inactivity timers
    checkButtonPresses();

    // Automatically enter sleep mode after 30 seconds of inactivity
    checkInactivity();

    // Checking the battery status
    batterypercentage = battery.percent();
    Serial.println(batterypercentage);


    // If the flag to send location is set, update location and print GPS coordinates
    if (sendLocationFlag) {
        sendLocationFlag = false;  // Clear the flag once location data is being processed

        // Get the current GPS position
        pos = gnss.checkLocation();

        // Print the GPS coordinates (latitude and longitude) to the serial monitor for debugging
        Serial.print("Latitude: ");
        Serial.print(pos.latitude, 6);
        Serial.print(", Longitude: ");
        Serial.println(pos.longitude, 6);

        // Add your sending logic here (e.g., send location via LoRa, HTTP, etc.)
    }

    

    // Handle different power modes based on the current mode (ACTIVE, PARK, SLEEP)
    switch (getCurrentMode()) {
        case ACTIVE:
            gnss.update();  // Update GNSS module with new GPS data
            manageBatteryWarning(batterypercentage);
            // Check if the bike is moving by detecting motion via the accelerometer
            if(detector.isMoving() == true) {
                lastActivityTime = millis();  // Reset the inactivity timer if movement is detected
            }
            Serial.println("Check movement");  // Debugging message
            activeMode();  // Perform actions related to active mode (e.g., light control)
            send_lora(getCurrentMode(),batterypercentage,pos.longitude,pos.latitude);
            setCurrentMode(recieve_lora());
            break;

        case PARK:
            lastActivityTime = millis();  // Reset the inactivity timer if movement is detected
            parkMode();  // Perform actions related to park mode (e.g., light sleep)
            send_lora(getCurrentMode(),batterypercentage,pos.longitude,pos.latitude);
            setCurrentMode(recieve_lora());
            break;

        case SLEEP:
            send_lora(getCurrentMode(),batterypercentage,pos.longitude,pos.latitude);
            setCurrentMode(recieve_lora());
            sleepMode();  // Perform actions related to sleep mode (e.g., deep sleep)
            break;
    }
}
