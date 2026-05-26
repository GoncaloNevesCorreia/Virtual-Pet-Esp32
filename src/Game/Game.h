#ifndef GAME_H
#define GAME_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Button.h>
#include <SPI.h>
#include <Wire.h>

#include "Game/Pet/Pet.h"
#include "Network/Network.h"
#include "Storage/Storage.h"

namespace Game {

extern Adafruit_SSD1306 display;

void init();

void loop();

void save();

void eat();

void play();

void sleep();

}  // namespace Game

#endif