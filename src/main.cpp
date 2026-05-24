#define DEBUG

#include <Arduino.h>
#include <Preferences.h>

#include "Game/Game.h"
#include "Network/Network.h"
#include "Storage/Storage.h"

void setup() {
  Serial.begin(115200);

  Storage::init();

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
