#include <Arduino.h>

// Connection idea:
// - All button commons are tied together and connected to A0.
// - Each unique button leg is connected to its own Arduino pin: D2..D9.
// - The sketch waits for a button press and then reports which one matched.
const uint8_t buttonPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t probePin = A0;
const uint8_t numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

void releaseAllPins() {
  for (uint8_t i = 0; i < numButtons; ++i) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(probePin, INPUT);
}

bool isProbeConnected() {
  // With the common bus pulled down through 10k to GND, a pressed button
  // will raise the A0 voltage significantly. This is a simple threshold check.
  int value = analogRead(probePin);
  return value > 3000;
}

void waitForButtonPress() {
  while (true) {
    for (uint8_t i = 0; i < numButtons; ++i) {
      releaseAllPins();
      pinMode(buttonPins[i], OUTPUT);
      digitalWrite(buttonPins[i], HIGH);
      delay(10);

      if (isProbeConnected()) {
        Serial.print("SUCCESS: Wire matches button ");
        Serial.print(i + 1);
        Serial.print(" (pin ");
        Serial.print(buttonPins[i]);
        Serial.println(")");

        digitalWrite(buttonPins[i], LOW);
        pinMode(buttonPins[i], INPUT);
        return;
      }

      digitalWrite(buttonPins[i], LOW);
      pinMode(buttonPins[i], INPUT);
      delay(5);
    }

    // Idle while waiting for a press.
    delay(20);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  releaseAllPins();

  Serial.println("Button tester ready.");
  Serial.println("Connect the common side of the panel to A0 through a 10k pull-down to GND.");
  Serial.println("Press a button and the sketch will print SUCCESS for the matching button.");
  Serial.println("---");
}

void loop() {
  waitForButtonPress();
  delay(250);
}