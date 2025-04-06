#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

#define BUTTON_PIN_1 33
#define BUTTON_PIN_2 18
#define BUTTON_PIN_3 25

extern volatile bool button1Pressed;
extern volatile bool button2Pressed;
extern unsigned long lastActivityTime; // Use extern to share across files

void setupButtons();
void checkButtonPresses();

#endif
