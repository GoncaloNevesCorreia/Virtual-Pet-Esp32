#define DEBUG

#include <Arduino.h>

#include "Network/Network.h"
#include "Ui/Engine.h"

void setup() {
  Serial.begin(115200);

  Network::init();

  Engine::init();
}

void loop() {
#ifdef DEBUG
  // Slows down code to improve wokwi simulation
  delay(1);
#endif

  Network::loop();

  Engine::RenderPet();
}
