#include "ESP32_lora.h"

int checkMode;

void setup() {
  Serial.begin(57600);
  init_lora();
}

void loop() {
  send_lora(1,3.70,-123.456789,49.282729);
  delay(2000);
  checkMode = recieve_lora();
  Serial.println("Change Mode to : " + String(checkMode)); // 11, 22, 33, 0 (for no mode change)
  delay(2000);
}