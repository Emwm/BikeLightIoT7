#include "buttonHandler.h"

volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
unsigned long lastActivityTime = 0; // Now defined only in buttonHandler.cpp

void setupButtons() {
    pinMode(BUTTON_PIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_PIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_PIN_3, INPUT_PULLDOWN);
}

void checkButtonPresses() {
    if (button1Pressed) {
        button1Pressed = false;
        lastActivityTime = millis();
    }
    if (button2Pressed) {
        button2Pressed = false;
        lastActivityTime = millis();
    }
}
