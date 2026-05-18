#ifndef PET_HUNGER_H
#define PET_HUNGER_H

#include <Animate.h>
#include <Timer.h>

#include "Game/Game.h"
#include "Game/Ui/Stats/Hunger.h"

namespace Pet_Hunger {

void init();

void render();

void increase();

void decrease();

}  // namespace Pet_Hunger

#endif