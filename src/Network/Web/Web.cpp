#include "Web.h"

namespace Web {
const char* ssid = "ESP32-Virtual-Pet";
const char* password = "123456789";

const uint8_t DNS_PORT = 53;

IPAddress apIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
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
<!doctype html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>ESP32 WiFi / MQTT Setup</title>
    <style>
      *,
      *::before,
      *::after {
        box-sizing: border-box;
        padding: 0;
        margin: 0;
      }

      body {
        font-family: Arial, sans-serif;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
      }

      h1 {
        text-align: center;
      }

      p {
        padding-top: 2rem;
        padding-bottom: 2rem;
      }

      input {
        display: block;
        width: 100%;
        min-width: 300px;
        margin: 8px 0 16px;
        padding: 8px;
      }

      button {
        padding: 8px 1em;
        width: 100%;
        cursor: pointer;
      }

      .password-field {
        position: relative;
      }

      .password-field button {
        position: absolute;
        width: auto;
        top: 50%;
        left: 100%;
        transform: translate(-100%, -50%);
      }
    </style>
  </head>
  <body>
    <h1>ESP32 WiFi / MQTT Setup</h1>
    <main>
    )rawliteral" +
         getStatusHtml() + R"rawliteral(
  <form method="POST" action="/save">
        <label for="ssid">WiFi SSID</label>
        <input id="ssid" name="ssid" required />

        <label for="pass">WiFi Password</label>

        <div class="password-field">
          <input id="pass" name="pass" type="password" />
          <button type="button">Show</button>
        </div>

        <label for="mqtt">MQTT Broker</label>
        <input id="mqtt" name="mqtt_server" required />

        <button type="submit">Connect</button>
      </form>
    </main>

    <script type="text/javascript">
      const passToggle = document.querySelector(".password-field button");
      passToggle.addEventListener("click", (e) => {
        const el = e.target;

        const input = el.parentNode.firstElementChild;

        console.log(input);

        if (input.type === "password") {
          input.type = "text";
          el.textContent = "Hide";
        } else {
          input.type = "password";
          el.textContent = "Show";
        }
      });
    </script>
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

  if (!server.hasArg("mqtt_server") || server.arg("mqtt_server").length() == 0) {
    server.send(400, "text/plain", "Missing MQTT Host");
    return;
  }

  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  String mqtt_server = server.arg("mqtt_server");

  Network::setupWifi(ssid, pass);
  Network::setupMQTT(mqtt_server);

  connecting = true;

  server.sendHeader("Location", "/", true);
  server.send(303, "text/plain", "Connecting...");
}

void handleCaptivePortal() {
  server.sendHeader("Location", "http://" + apIP.toString() + "/", true);
  server.send(302, "text/plain", "");
}

}  // namespace Web