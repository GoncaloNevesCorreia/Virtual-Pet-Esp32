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

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

const char* mqtt_server = SECRET_MQTT;

WiFiClient espClient;
PubSubClient client(espClient);

bool online = false;

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

void setupMqttConnection() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String createUniqueUserID() {
  String clientId = F("IOT2026-virtualpet-");
  clientId += String(random(0xffff), HEX);
  return clientId;
}

// Improve to not use delay
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    String userID = createUniqueUserID();
    if (client.connect(userID.c_str())) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(TOPIC_EAT);
      client.subscribe(TOPIC_PLAY);
      client.subscribe(TOPIC_SLEEP);
      client.subscribe(TOPIC_GET_LATEST);

      online = true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void sendStats() {
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

void init() {
  setup_wifi();

  setupMqttConnection();

  online = true;
}

void loop() {
  if (!client.connected()) {
    online = false;
    reconnect();
  }

  client.loop();
}

}  // namespace Network