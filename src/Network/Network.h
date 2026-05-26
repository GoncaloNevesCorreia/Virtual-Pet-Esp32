#ifndef NETWORK_H
#define NETWORK_H

#include <PubSubClient.h>
#include <WiFi.h>

#include "Config.h"
#include "Game/Game.h"

namespace Network {

void init();

void loop();

void sendNewState(const char* newState);

}  // namespace Network

#endif