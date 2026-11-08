#include <Arduino.h>

// Wire each button's opposite leg to one of these pins.
// The common side of the button bank should be tied together and connected to A0.
// Then, one wire at a time, the probe is connected to that common bus.
const uint8_t buttonPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t probePin = A0;
const uint8_t numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

void releaseAllPins() {
  for (uint8_t i = 0; i < numButtons; ++i) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(probePin, INPUT);
}

void printButtonState(uint8_t index, bool matched) {
  Serial.print("Button ");
  Serial.print(index + 1);
  Serial.print(" (pin ");
  Serial.print(buttonPins[index]);
  Serial.print(")");
  Serial.println(matched ? " -> MATCH" : " -> no match");
}

void scanSingleWire() {
  bool found = false;

  Serial.println("Testing one unknown wire against all button pins...");
  Serial.println("Press each button in turn and watch for a MATCH.");

  for (uint8_t i = 0; i < numButtons; ++i) {
    releaseAllPins();
    pinMode(buttonPins[i], OUTPUT);
    digitalWrite(buttonPins[i], HIGH);
    delay(10);

    int value = analogRead(probePin);
    bool matched = value > 3000;
    printButtonState(i, matched);

    if (matched) {
      found = true;
      Serial.print("This wire belongs to button number ");
      Serial.println(i + 1);
      Serial.println("---");
    }

    digitalWrite(buttonPins[i], LOW);
    pinMode(buttonPins[i], INPUT);
    delay(25);
  }

  releaseAllPins();

  if (!found) {
    Serial.println("No button matched this wire.");
    Serial.println("Check that the wire is on the common side of the button panel and that you pressed each button one at a time.");
    Serial.println("---");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  releaseAllPins();

  Serial.println("Button lead matcher ready");
  Serial.println("Connection idea:");
  Serial.println("- Tie all common button legs together.");
  Serial.println("- Connect that common to A0.");
  Serial.println("- Connect each unique button leg to pins 2..9.");
  Serial.println("- Plug one unknown wire into the common and press buttons one by one.");
  Serial.println("---");
}

void loop() {
  scanSingleWire();
  delay(1000);
}