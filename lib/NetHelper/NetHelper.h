#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <Timer.h>
#include <WiFi.h>

class NetHelper {
 private:
  typedef void (*onConnected)();

  Timer _mqttReconnectionTimer;

  String _ssid;
  String _password;
  bool _wifiConnected = false;

  PubSubClient* _client = nullptr;

  String _mqtt_server;
  String _UUID;
  onConnected _onMQTTConnectedCB;
  bool _mqttConnected = false;

  void onWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
  void onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  void onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

  String createUniqueUserID();

  void reconnectMQTT();

 public:
  NetHelper() : _mqttReconnectionTimer(5000) {}

  void setupWifi(String ssid, String password);

  void setupMQTT(PubSubClient* client, String mqtt_server, onConnected callback);

  bool isOnline();

  void loop();
};

#endif