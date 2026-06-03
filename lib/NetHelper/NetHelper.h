#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <Arduino.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <Timer.h>
#include <WiFi.h>

class NetHelper {
 private:
  typedef void (*onConnectedWifi)(String ssid, String password);
  typedef void (*onConnectedMQTT)(String host);

  Timer _mqttReconnectionTimer;
  Preferences prefs;

  String _ssid;
  String _password;

  onConnectedWifi _onWifiConnectedCB;
  bool _wifiConnected = false;

  PubSubClient* _client = nullptr;

  String _mqtt_server;
  String _UUID;

  onConnectedMQTT _onMQTTConnectedCB;
  bool _mqttServerProvided = false;
  bool _mqttConnected = false;

  void onWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
  void onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  void onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

  String createUniqueUserID();

  void reconnectMQTT();

 public:
  NetHelper() : _mqttReconnectionTimer(5000) {}

  void setupWifi(String ssid, String password, onConnectedWifi callback);

  void setupMQTT(PubSubClient* client, String mqtt_server, onConnectedMQTT callback);

  bool isOnline();

  void loop();
};

#endif