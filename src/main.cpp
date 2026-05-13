

#include <Arduino.h>

#include "Game/Game.h"
#include "Network/Network.h"

void setup() {
  Serial.begin(115200);

  Network::init();

  Game::init();
}

void loop() {
#ifdef DEBUG
  // Slows down code to improve wokwi simulation
  delay(1);
#endif

  Network::loop();

  Game::loop();
}
