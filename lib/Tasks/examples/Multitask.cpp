#include <Arduino.h>
#include <Tasks.h>

const byte LED_PINS[] = {2, 3, 4};

byte currentLedPin = LED_PINS[0];

Tasks tasks;

void enableRed();
void enableYellow();
void enableGreen();
void fastBlink();
void slowBlink();

void setup() {
  for (byte i = 0; i < 3; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  tasks.delay(enableRed, 3000)
      .then(enableYellow, 1000)
      .then(enableGreen, 3000);

  tasks.delay(slowBlink, 1500);
  tasks.delay(fastBlink, 200);
}

void loop() { tasks.update(); }

void enableRed() {
  digitalWrite(currentLedPin, LOW);

  currentLedPin = LED_PINS[2];

  digitalWrite(currentLedPin, HIGH);
}

void enableYellow() {
  digitalWrite(currentLedPin, LOW);

  currentLedPin = LED_PINS[1];

  digitalWrite(currentLedPin, HIGH);
}

void enableGreen() {
  digitalWrite(currentLedPin, LOW);

  currentLedPin = LED_PINS[0];

  digitalWrite(currentLedPin, HIGH);
}

void slowBlink() {
  static bool enabled = false;

  if (enabled == true) {
    digitalWrite(7, HIGH);
  } else {
    digitalWrite(7, LOW);
  }

  enabled = !enabled;
}

void fastBlink() {
  static bool enabled = false;

  if (enabled == true) {
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }

  enabled = !enabled;
}