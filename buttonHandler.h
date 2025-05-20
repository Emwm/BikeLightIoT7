#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h> // Include Arduino core library for pinMode, digitalRead, etc.

// ---- GPIO Pin Definitions ----
#define BUTTON_PIN_1 33  // GPIO for Button 1

// ---- External Variable Declarations ----
extern volatile bool button1Pressed;       // Flag indicating if Button 1 was pressed (set in ISR)
extern volatile bool button2Pressed;       // Flag indicating if Button 2 was pressed (set in ISR)
extern unsigned long lastActivityTime;     // Tracks the last time a button was pressed

// ---- Function Declarations ----
void setupButtons();        // Set up button pins as inputs (likely with pull-up/down)
void checkButtonPresses();  // Check and handle button press flags, update activity time

#endif
