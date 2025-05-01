#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <Arduino.h>

// ---- Pin Definitions ----
#define LDR_PIN 34   // LDR sensor input pin (Analog)
#define LIGHT_PIN 5  // Output pin to control light (e.g., LED or relay)

// ---- Function Declarations ----

// Initializes the LIGHT_PIN as an output
void setupLight();    

// Reads LDR value and turns light ON/OFF based on a threshold
void controlLight();  

// Forces the light to turn OFF regardless of LDR value
void turnLightoff();  

#endif
