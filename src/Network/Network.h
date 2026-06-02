#ifndef NETWORK_H
#define NETWORK_H

#include <NetHelper.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "Config.h"
#include "Game/Game.h"

namespace Network {

void init();

void loop();

bool isOnline();

void sendStats();

}  // namespace Network

#endif