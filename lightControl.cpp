#include "lightControl.h"

#define LDR_PIN 34  // LDR sensor input (Analog)
#define LIGHT_PIN 5 // Light control (LED or Relay)

int threshold = 10;   // Light threshold (adjust based on your LDR readings)

void setupLight() {
    pinMode(LIGHT_PIN, OUTPUT);
}

// ---- FUNCTION: Control Light Based on LDR ----
void controlLight() {
    int lightLevel = analogRead(LDR_PIN);
    Serial.printf("Light Level: %d\n", lightLevel);

    if (lightLevel < threshold) {
        digitalWrite(LIGHT_PIN, HIGH);
        Serial.println("Light ON");
    } else {
        digitalWrite(LIGHT_PIN, LOW);
        Serial.println("Light OFF");
    }
}

void turnLightoff()
{
    digitalWrite(LIGHT_PIN, LOW);
    Serial.println("Light OFF");
}
