#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <Arduino.h>

// ---- Pin Definitions ----
#define LDR_PIN 34   // LDR sensor input pin (Analog)
#define LIGHT_PIN 5  // Output pin to control light (e.g., LED or relay)
#define LED_LOW_BAT 2 // Showing battery percentage under 25%

// ---- Function Declarations ----

// Initializes the LIGHT_PIN as an output
void setupLight();    

// Reads LDR value and turns light ON/OFF based on a threshold
void controlLight();  

// Forces the light to turn OFF regardless of LDR value
void turnLightoff();  

// Turns on a LED if the battery percentage is under 25%
void manageBatteryWarning(float batterypercentage);

#endif
