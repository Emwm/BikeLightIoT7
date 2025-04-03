#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

extern volatile bool button1Pressed;
extern volatile bool button2Pressed;
extern unsigned long lastActivityTime; // Use extern to share across files

void setupButtons();
void checkButtonPresses();
void IRAM_ATTR handleInterrupt1();
void IRAM_ATTR handleInterrupt2();

#endif
