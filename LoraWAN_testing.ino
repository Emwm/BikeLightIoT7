#include "LoraWAN_Reese.h"

void setup() {
  Serial.begin(115200);
  initState();
  joinState();
  Serial.println("entering code loop");
}

void loop() {
  sendState();
}