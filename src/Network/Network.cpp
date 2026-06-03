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

String ssid = "";
String password = "";

String mqtt_server = SECRET_MQTT;

WiFiClient espClient;
PubSubClient client(espClient);
NetHelper netHelper;

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
  if (!netHelper.isOnline()) return;

  uint8_t stats[5];

  Game::getStats(stats);

  Serial.print("Sending Stats: ");
  Serial.print(stats[0]);
  Serial.print(stats[1]);
  Serial.print(stats[2]);
  Serial.print(stats[3]);
  Serial.println(stats[4]);

  client.publish(TOPIC_STATS, stats, sizeof(stats));
}

void onConnectedMQTT() {
  client.subscribe(TOPIC_EAT);
  client.subscribe(TOPIC_PLAY);
  client.subscribe(TOPIC_SLEEP);
  client.subscribe(TOPIC_GET_LATEST);
}

void onConnectedWifi(String ssid, String password) {
  Storage::saveCredentials("ssid", ssid);
  Storage::saveCredentials("pass", password);
}

void init() {
  ssid = Storage::loadCredentials("ssid");
  password = Storage::loadCredentials("pass");

  if (ssid.length() > 0) {
    setupWifi(ssid, password);

    setupMQTT(mqtt_server);
  }

  client.setCallback(mqttCallback);

  Web::init();
}

void loop() {
  netHelper.loop();

  Web::loop();
}

bool isOnline() {
  return netHelper.isOnline();
}

void setupWifi(String ssid, String password) {
  netHelper.setupWifi(ssid, password, onConnectedWifi);
}

void setupMQTT(String mqtt_server) {
  netHelper.setupMQTT(&client, mqtt_server, onConnectedMQTT);
}

}  // namespace Network