#ifndef GAME_H
#define GAME_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Tasks.h>
#include <Wire.h>

#include "Game/Hunger/Hunger.h"
#include "Game/Pet/Pet.h"

namespace Game {

extern Adafruit_SSD1306 display;

void init();

void loop();

}  // namespace Game

#endif