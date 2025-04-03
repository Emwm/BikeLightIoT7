#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <Arduino.h>

#define LDR_PIN 34  // LDR sensor input (Analog)
#define LIGHT_PIN 5 // Light control (LED or Relay)

void setupLight();    // Initialize light control
void controlLight();  // Turn light ON/OFF based on LDR sensor
void turnLightoff(); // Turn LED off

#endif
