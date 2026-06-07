#ifndef NETWORK_H
#define NETWORK_H

#include <NetworkHelper.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <WiFi.h>

#include "Game/Game.h"
#include "Web/Web.h"

namespace Network {

void setupWifi(String ssid, String password);

void setupMQTT(String mqtt_server);

void init();

void loop();

bool isOnline();

void sendStats();

}  // namespace Network

#endif