#include <Animate.h>

#include "Game/Game.h"
#include "Game/Health/Health.h"
#include "Game/Hunger/Hunger.h"
#include "Game/Ui/States/Dead.h"
#include "Game/Ui/States/Eating.h"
#include "Game/Ui/States/Idle.h"

#ifndef PET_H
#define PET_H

namespace Pet {

void init();

void render();

void eat();

}  // namespace Pet

#endif