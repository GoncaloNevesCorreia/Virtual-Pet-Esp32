#include "NetHelper.h"

void NetHelper::setupWifi(String ssid, String password) {
  _ssid = ssid;
  _password = password;
  _wifiConnected = false;

  WiFi.disconnect(true);

  delay(500);

  WiFi.onEvent([this](arduino_event_id_t event, arduino_event_info_t info) { this->onWifiConnected(event, info); },
               WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);

  WiFi.onEvent([this](arduino_event_id_t event, arduino_event_info_t info) { this->onWiFiGotIP(event, info); },
               WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);

  WiFi.onEvent([this](arduino_event_id_t event, arduino_event_info_t info) { this->onWiFiDisconnected(event, info); },
               WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.begin(_ssid.c_str(), _password.c_str());

  Serial.println();
  Serial.println();
  Serial.println("Connecting to WiFi... ");
}

void NetHelper::onWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Connected to AP successfully!");
}

void NetHelper::onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  _wifiConnected = true;

  if (!_client->connected()) {
    _mqttReconnectionTimer.reset();
    _mqttConnected = false;
    reconnectMQTT();
  }
}

void NetHelper::onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  _wifiConnected = false;

  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(_ssid, _password);
}

void NetHelper::setupMQTT(PubSubClient* client, String mqtt_server, onConnected callback) {
  _client = client;
  _mqtt_server = mqtt_server;

  _client->disconnect();

  _client->setServer(_mqtt_server.c_str(), 1883);

  _onMQTTConnectedCB = callback;

  _mqttReconnectionTimer.setCallback([this]() { this->reconnectMQTT(); });
}

String NetHelper::createUniqueUserID() {
  String clientId = F("IOT2026-virtualpet-");
  clientId += String(random(0xffff), HEX);
  return clientId;
}

void NetHelper::reconnectMQTT() {
  Serial.print("Attempting MQTT connection...");

  String userID = createUniqueUserID();

  if (_client->connect(userID.c_str())) {
    Serial.println("connected");
    _mqttConnected = true;

    if (_onMQTTConnectedCB != nullptr) _onMQTTConnectedCB();

  } else {
    Serial.print("failed, rc=");
    Serial.print(_client->state());
    Serial.println(" try again in 5 seconds");
  }
}

void NetHelper::loop() {
  if (!_wifiConnected) return;

  if (!_client->connected()) {
    _mqttConnected = false;
    _mqttReconnectionTimer.run();
    return;
  }

  _client->loop();
}

bool NetHelper::isOnline() {
  return _wifiConnected && _mqttConnected;
}