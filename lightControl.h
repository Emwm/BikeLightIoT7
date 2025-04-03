#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <Arduino.h>

void setupLight();    // Initialize light control
void controlLight();  // Turn light ON/OFF based on LDR sensor
void turnLightoff(); // Turn LED off

#endif
