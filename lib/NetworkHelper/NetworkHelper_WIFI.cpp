#include "NetworkHelper.h"

void NetworkHelper::setupWifi(String ssid, String password, onConnectedWifi callback) {
  _ssid = ssid;
  _password = password;
  _onWifiConnectedCB = callback;

  _wifiConnected = false;

  WiFi.disconnect(false, false);

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

void NetworkHelper::onWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Connected to AP successfully!");
}

void NetworkHelper::onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  _wifiConnected = true;

  if (_onWifiConnectedCB != nullptr) {
    _onWifiConnectedCB(_ssid, _password);
  }
}

void NetworkHelper::onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  _wifiConnected = false;

  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(_ssid.c_str(), _password.c_str());
}