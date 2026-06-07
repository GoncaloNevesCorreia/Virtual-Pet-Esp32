#include "Network.h"

namespace Network {

// Actions sent from the WebServer.
#define TOPIC_EAT "virtual_pet/eat"
#define TOPIC_SLEEP "virtual_pet/sleep"
#define TOPIC_PLAY "virtual_pet/play"

// Request from WebServer to send back the latest Data
#define TOPIC_GET_LATEST "virtual_pet/get_latest"

// Data sent to the WebServer
#define TOPIC_STATS "virtual_pet/stats"

WiFiClient espClient;
PubSubClient client(espClient);
NetworkHelper networkHelper;

void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.println(topic);

  if (strcmp(topic, TOPIC_EAT) == 0) {
    Game::eat();
    return;
  }

  if (strcmp(topic, TOPIC_PLAY) == 0) {
    Game::play();
    return;
  }

  if (strcmp(topic, TOPIC_SLEEP) == 0) {
    Game::sleep();
    return;
  }

  if (strcmp(topic, TOPIC_GET_LATEST) == 0) {
    sendStats();
    return;
  }
}

void sendStats() {
  if (!networkHelper.isOnline()) return;

  uint8_t stats[5];

  Game::getStats(stats);

  client.publish(TOPIC_STATS, stats, sizeof(stats));
}

void onConnectedWifi(String ssid, String password) {
  Storage::saveCredentials("ssid", ssid);
  Storage::saveCredentials("pass", password);
}

void onConnectedMQTT(String mqtt_server) {
  Storage::saveCredentials("mqtt_server", mqtt_server);

  client.subscribe(TOPIC_EAT);
  client.subscribe(TOPIC_PLAY);
  client.subscribe(TOPIC_SLEEP);
  client.subscribe(TOPIC_GET_LATEST);
}

void init() {
  String ssid = Storage::loadCredentials("ssid");
  String password = Storage::loadCredentials("pass");

  String mqtt_server = Storage::loadCredentials("mqtt_server");

  if (ssid.length() > 0) {
    setupWifi(ssid, password);

    if (mqtt_server.length() > 0) {
      setupMQTT(mqtt_server);
    }
  }

  client.setCallback(mqttCallback);

  Web::init();
}

void loop() {
  networkHelper.loop();

  Web::loop();
}

bool isOnline() {
  return networkHelper.isOnline();
}

void setupWifi(String ssid, String password) {
  networkHelper.setupWifi(ssid, password, onConnectedWifi);
}

void setupMQTT(String mqtt_server) {
  networkHelper.setupMQTT(&client, mqtt_server, onConnectedMQTT);
}

}  // namespace Network