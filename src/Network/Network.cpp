#include "Network.h"

namespace Network {

#define OFFLINE

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

const char* mqtt_server = SECRET_MQTT;

WiFiClient espClient;
PubSubClient client(espClient);

void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    String userID = createUniqueUserID();
    if (client.connect(userID.c_str())) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/presence");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void sendData() {
  client.publish("esp32/temperature", "69");
}

void init() {
#ifndef OFFLINE

  setup_wifi();

  setupMqttConnection();
#endif
}

void loop() {
#ifndef OFFLINE
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

#endif
}

}  // namespace Network