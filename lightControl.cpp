#include "lightControl.h" // Include the header for light control functionality

// ---- Light Sensitivity Threshold ----
// Below this value (from LDR), the light will turn ON
int threshold = 150;

// ---- FUNCTION: Initialize Light Pin ----
// Sets up the output pin for controlling the light (LED or relay)
void setupLight() {
    pinMode(LIGHT_PIN, OUTPUT); // Configure light control pin as OUTPUT
    pinMode(LED_LOW_BAT, OUTPUT);     // Low battery warning LED
}

// ---- FUNCTION: Control Light Based on LDR Sensor Reading ----
// Turns light ON if environment is dark (LDR value below threshold), OFF otherwise
void controlLight() {
    int lightLevel = analogRead(LDR_PIN); // Read light intensity from LDR sensor
    Serial.printf("Light Level: %d\n", lightLevel); // Debug print of LDR value

    if (lightLevel < threshold) {
        digitalWrite(LIGHT_PIN, HIGH);     // Turn light ON
        Serial.println("Light ON");        // Log action
    } else {
        digitalWrite(LIGHT_PIN, LOW);      // Turn light OFF
        Serial.println("Light OFF");       // Log action
    }
}

// ---- FUNCTION: Force Light OFF ----
// Ensures the light is turned off regardless of LDR input
void turnLightoff() {
    digitalWrite(LIGHT_PIN, LOW); // Turn OFF the light
    Serial.println("Light OFF");  // Log action
}

// ---- FUNCTION: Manage Low Battery LED Warning ----
// Turns ON the low battery LED if the battery percentage drops below 25%,
// otherwise turns it OFF. This helps save energy by only activating the LED when necessary.
void manageBatteryWarning(float batterypercentage) {
    if (batterypercentage < 25) {
        digitalWrite(LED_LOW_BAT, HIGH);  // Turn ON the low battery LED
    } else {
        digitalWrite(LED_LOW_BAT, LOW);   // Turn OFF the LED if battery is sufficient
    }
}


