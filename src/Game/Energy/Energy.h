#ifndef PET_ENERGY_H
#define PET_ENERGY_H

#include <Animate.h>
#include <Arduino.h>
#include <Timer.h>

#include "Game/Game.h"
#include "Game/Ui/Stats/Energy.h"

namespace Pet_Energy {

extern const uint8_t LowEnergy;
extern uint8_t energy;

void init();

void render();

void increase();

void decrease();

}  // namespace Pet_Energy

#endif