#ifndef WEB_PORTAL_H
#define WEB_PORTAL_H

#include <Arduino.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>

#include "Network/Network.h"

namespace Web {

void init();

void loop();

}  // namespace Web

#endif