#ifndef PET_FUN_H
#define PET_FUN_H

#include <Animate.h>
#include <Timer.h>

#include "Game/Game.h"
#include "Game/Ui/Stats/Fun.h"

namespace Pet_Fun {
extern const unsigned long INCREASE_AMOUNT;

extern const uint8_t LowFun;

extern uint8_t fun;

void init();

void render();

void increase();

void decrease();

void save();

void clear();

}  // namespace Pet_Fun

#endif