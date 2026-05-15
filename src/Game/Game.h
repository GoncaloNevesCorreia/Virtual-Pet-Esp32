#ifndef GAME_H
#define GAME_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// Inclui a lógica do Pet
#include "Game/Pet/Pet.h"

namespace Game {

// Declaração externa do display para ser usado no .cpp
extern Adafruit_SSD1306 display;

// Funções principais do motor do jogo
void init();
void loop();

}  // namespace Game

#endif