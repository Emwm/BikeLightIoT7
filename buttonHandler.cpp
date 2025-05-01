#include "buttonHandler.h" // Include the corresponding header file

// ---- Global Variable Definitions ----
// These match the extern declarations in the header file
volatile bool button1Pressed = false;    // Flag indicating if Button 1 was pressed (set in ISR)
volatile bool button2Pressed = false;    // Flag indicating if Button 2 was pressed (set in ISR)
unsigned long lastActivityTime = 0;      // Timestamp of the most recent button press

// ---- Initialize Button Pins ----
// Configure button pins as inputs with internal pull-down resistors
void setupButtons() {
    pinMode(BUTTON_PIN_1, INPUT_PULLDOWN); // Setup Button 1 pin
    pinMode(BUTTON_PIN_2, INPUT_PULLDOWN); // Setup Button 2 pin
    pinMode(BUTTON_PIN_3, INPUT_PULLDOWN); // Setup Button 3 pin (unused in logic but prepared)
}

// ---- Handle Button Press Flags ----
// Check which buttons were flagged as pressed and clear flags; update activity timer
void checkButtonPresses() {
    if (button1Pressed) {
        button1Pressed = false;              // Reset flag
        lastActivityTime = millis();         // Update activity timestamp
    }
    if (button2Pressed) {
        button2Pressed = false;              // Reset flag
        lastActivityTime = millis();         // Update activity timestamp
    }
}
