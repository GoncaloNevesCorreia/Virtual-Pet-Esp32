#include "NetworkHelper.h"

void NetworkHelper::setupMQTT(PubSubClient* client, String mqtt_server, onConnectedMQTT callback) {
  _client = client;
  _mqtt_server = mqtt_server;

  _client->disconnect();

  _client->setServer(_mqtt_server.c_str(), 1883);

  _onMQTTConnectedCB = callback;

  _mqttServerProvided = true;

  _mqttReconnectionTimer.setCallback([this]() { this->reconnectMQTT(); });

  _mqttReconnectionAttempts = 0;
}

String NetworkHelper::createUniqueUserID() {
  String clientId = F("IOT2026-");
  clientId += String(random(0xffff), HEX);
  return clientId;
}

void NetworkHelper::reconnectMQTT() {
  if (_mqttReconnectionAttempts >= RECONNECT_MAX_ATTEMPTS) {
    _mqttReconnectionAttempts = 0;
    _mqttReconnectionTimer.setInterval(RECONNECT_INTERVAL);
  }

  Serial.print("Attempting MQTT connection...");

  String userID = createUniqueUserID();

  if (_client->connect(userID.c_str())) {
    Serial.println("connected");
    _UUID = userID;
    _mqttConnected = true;
    _mqttReconnectionAttempts = 0;

    if (_onMQTTConnectedCB != nullptr) _onMQTTConnectedCB(_mqtt_server);

  } else {
    Serial.print("failed, rc=");
    Serial.print(_client->state());
    Serial.print(" trying again in ");
    Serial.print(RECONNECT_INTERVAL / 1000);
    Serial.println(" seconds.");

    _mqttReconnectionAttempts++;

    if (_mqttReconnectionAttempts >= RECONNECT_MAX_ATTEMPTS) {
      _mqttReconnectionTimer.setInterval(RECONNECT_COOLDOWN);
    }

    Serial.print("Failed ");
    Serial.print(RECONNECT_MAX_ATTEMPTS);
    Serial.println(" times, while trying to connect to the MQTT Broker. Please check configuration.");
  }
}
