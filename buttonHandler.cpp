#include "buttonHandler.h"

#define BUTTON_PIN_1 33
#define BUTTON_PIN_2 18

volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
unsigned long lastActivityTime = 0; // Now defined only in buttonHandler.cpp

void setupButtons() {
    pinMode(BUTTON_PIN_1, INPUT_PULLUP);
    pinMode(BUTTON_PIN_2, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN_1, handleInterrupt1, FALLING);
    attachInterrupt(BUTTON_PIN_2, handleInterrupt2, FALLING);
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
