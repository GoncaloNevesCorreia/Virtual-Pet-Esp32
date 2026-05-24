#ifndef PET_FUN_H
#define PET_FUN_H

#include <Animate.h>
#include <Timer.h>

#include "Game/Game.h"
#include "Game/Ui/Stats/Fun.h"

namespace Pet_Fun {

extern const uint8_t LowFun;

extern uint8_t hunger;

void init();

void render();

void increase();

void decrease();

void save();

}  // namespace Pet_Fun

#endif