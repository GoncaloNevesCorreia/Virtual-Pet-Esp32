#ifndef UI_ENGINE_H
#define UI_ENGINE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Tasks.h>
#include <Wire.h>

#include "Pet/Dead.h"

namespace Engine {

enum PetState {
  DEAD,
  IDLE,
};

void init();

void SetPetState(enum PetState state);
void RenderPet();

}  // namespace Engine

#endif