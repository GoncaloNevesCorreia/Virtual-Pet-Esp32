#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define DEBUG

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "192.168.1.80";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi();
void reconnect();
void callback(char* topic, byte* message, unsigned int length);
String createUniqueUserID();

void setup() {
  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
#ifdef DEBUG
  // Slows down code to improve wokwi simulation
  delay(1);
#endif

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
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

void callback(char* topic, byte* message, unsigned int length) {
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

String createUniqueUserID() {
  String clientId = F("IOT2026-virtualpet-");
  clientId += String(random(0xffff), HEX);
  return clientId;
}

// void callback(char* topic, byte* message, unsigned int length) {
//   Serial.print("Message arrived on topic: ");
//   Serial.print(topic);
//   Serial.print(". Message: ");
//   String messageTemp;

//   for (int i = 0; i < length; i++) {
//     Serial.print((char)message[i]);
//     messageTemp += (char)message[i];
//   }
//   Serial.println();

//   // Feel free to add more if statements to control more GPIOs with MQTT

//   // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
//   // Changes the output state according to the message
//   // if (String(topic) == "esp32/output") {
//   //   Serial.print("Changing output to ");
//   //   if (messageTemp == "on") {
//   //     Serial.println("on");
//   //     digitalWrite(ledPin, HIGH);
//   //   } else if (messageTemp == "off") {
//   //     Serial.println("off");
//   //     digitalWrite(ledPin, LOW);
//   //   }
//   // }
// }
