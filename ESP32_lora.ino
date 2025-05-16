#include "ESP32_lora.h"

void setup() {
  Serial.begin(57600);
  init_lora();
}

// the loop routine runs over and over again forever:
void loop() {
  send_lora(1,3.70,-123.456789,49.282729);
  recieve_lora();
}