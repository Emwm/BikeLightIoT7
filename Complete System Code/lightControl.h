#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <Arduino.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the type of module you're using (most common is FC16)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Number of MAX7219 devices in series (number of 8x8 matrices)
#define MAX_DEVICES 1

// ESP32 SPI pin connections
#define DATA_PIN 14  // DIN
#define CLK_PIN  13  // CLK
#define CS_PIN   12  // CS (sometimes labeled as LOAD)

// ---- Pin Definitions ----
#define LDR_PIN 34   // LDR sensor input pin (Analog)
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
