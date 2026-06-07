// Slows down code to improve wokwi simulation
// and adds Wokwi Network credentials to Storage
// #define WOKWI

// Disables Network Functionality
// #define OFFLINE

#include <Arduino.h>

#include "Game/Game.h"
#include "Network/Network.h"
#include "Storage/Storage.h"

void setup() {
  Serial.begin(115200);

#ifdef WOKWI
  Serial.print("Flash size: ");
  Serial.print(ESP.getFlashChipSize());
  Serial.println(" bytes");

  Serial.print("PSRAM found: ");
  Serial.println(psramFound() ? "yes" : "no");

  Serial.print("PSRAM size: ");
  Serial.print(ESP.getPsramSize());
  Serial.println(" bytes");

  Serial.print("Free PSRAM: ");
  Serial.print(ESP.getFreePsram());
  Serial.println(" bytes");
#endif

  Storage::init();

#ifndef OFFLINE

#ifdef WOKWI
  Storage::saveCredentials("ssid", "Wokwi-GUEST");
  Storage::saveCredentials("pass", "");
  Storage::saveCredentials("mqtt_server", "host.wokwi.internal");
#endif

  Network::init();
#endif

  Game::init();
}

void loop() {
#ifdef WOKWI
  // https://docs.wokwi.com/faq#the-simulation-is-slow-how-can-i-make-it-faster
  delay(10);
#endif

#ifndef OFFLINE
  Network::loop();
#endif

  Game::loop();
}
