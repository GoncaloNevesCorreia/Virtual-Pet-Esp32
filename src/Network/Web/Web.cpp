#include "Web.h"

namespace Web {
const char* ssid = "ESP32-Virtual-Pet";
const char* password = "123456789";

const uint8_t DNS_PORT = 53;

IPAddress apIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);
DNSServer dnsServer;

bool connecting = false;

String pendingSsid;
String pendingPass;

void handleRoot();
void handleSave();
void handleCaptivePortal();

void init() {
  WiFi.mode(WIFI_AP_STA);

  // Configure ESP32 AP IP
  WiFi.softAPConfig(apIP, gateway, subnet);

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Captive portal DNS: redirect all domains to ESP32 AP IP
  dnsServer.start(DNS_PORT, "*", apIP);

  // Web routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);

  server.on("/generate_204", HTTP_GET, handleRoot);         // Android
  server.on("/fwlink", HTTP_GET, handleRoot);               // Windows
  server.on("/hotspot-detect.html", HTTP_GET, handleRoot);  // Apple

  server.onNotFound(handleCaptivePortal);

  server.begin();
  Serial.println("Captive Portal Online.");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

String getStatusHtml() {
  if (WiFi.status() == WL_CONNECTED) {
    connecting = false;
    return "<p><b>STA:</b> Connected to " + WiFi.SSID() + "<br><b>STA IP:</b> " + WiFi.localIP().toString() + "</p>";
  }

  if (connecting) {
    return "<p><b>STA:</b> Connecting to " + pendingSsid + "...</p>";
  }

  return "<p><b>STA:</b> Not connected</p>";
}

String getPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 WiFi Setup</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 24px; }
    input { display: block; width: 100%; max-width: 320px; margin: 8px 0 16px; padding: 8px; }
    button { padding: 10px 16px; }
  </style>
</head>
<body>
  <h1>ESP32 WiFi Setup</h1>
)rawliteral" +
         getStatusHtml() + R"rawliteral(
  <form method="POST" action="/save">
    <label>WiFi SSID</label>
    <input name="ssid" required>

    <label>WiFi Password</label>
    <input name="pass" type="password">

    <button type="submit">Connect</button>
  </form>
</body>
</html>
)rawliteral";
}

void handleRoot() {
  server.send(200, "text/html", getPage());
}

void handleSave() {
  if (!server.hasArg("ssid") || server.arg("ssid").length() == 0) {
    server.send(400, "text/plain", "Missing SSID");
    return;
  }

  String ssid = server.arg("ssid");
  String pass = server.arg("pass");

  Network::setupWifi(ssid, pass);

  server.sendHeader("Location", "/", true);
  server.send(303, "text/plain", "Connecting...");
}

void handleCaptivePortal() {
  server.sendHeader("Location", "http://" + apIP.toString() + "/", true);
  server.send(302, "text/plain", "");
}

}  // namespace Web