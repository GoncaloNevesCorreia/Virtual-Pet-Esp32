#ifndef NETWORK_HELPER_H
#define NETWORK_HELPER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <Timer.h>
#include <WiFi.h>

#define RECONNECT_MAX_ATTEMPTS 3
#define RECONNECT_INTERVAL 5000
#define RECONNECT_COOLDOWN 300000

class NetworkHelper {
 private:
  typedef void (*onConnectedWifi)(String ssid, String password);
  typedef void (*onConnectedMQTT)(String host);

 public:
  NetworkHelper() : _mqttReconnectionTimer(RECONNECT_INTERVAL) {}

  void setupWifi(String ssid, String password, onConnectedWifi callback);

  void setupMQTT(PubSubClient* client, String mqtt_server, onConnectedMQTT callback);

  bool isOnline();

  void loop();

 private:
  String _ssid;
  String _password;

  bool _wifiConnected = false;

  void onWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
  void onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  void onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
  onConnectedWifi _onWifiConnectedCB;

  PubSubClient* _client = nullptr;
  Timer _mqttReconnectionTimer;
  uint8_t _mqttReconnectionAttempts = 0;

  String _mqtt_server;
  String _UUID;

  onConnectedMQTT _onMQTTConnectedCB;

  bool _mqttServerProvided = false;
  bool _mqttConnected = false;

  String createUniqueUserID();

  void reconnectMQTT();
};

#endif