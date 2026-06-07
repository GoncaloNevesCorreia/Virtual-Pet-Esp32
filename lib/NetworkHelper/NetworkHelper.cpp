#include "NetworkHelper.h"

void NetworkHelper::loop() {
  if (!_wifiConnected || !_mqttServerProvided) return;

  if (!_client->connected()) {
    _mqttConnected = false;
    _mqttReconnectionTimer.run();
    return;
  }

  _client->loop();
}

bool NetworkHelper::isOnline() {
  return _wifiConnected && _mqttConnected;
}