#include <Animate.h>

#include "Game/Energy/Energy.h"
#include "Game/Game.h"
#include "Game/Health/Health.h"
#include "Game/Hunger/Hunger.h"
#include "Game/Ui/States/Dead.h"
#include "Game/Ui/States/Eating.h"
#include "Game/Ui/States/Falling_Asleep.h"
#include "Game/Ui/States/Idle.h"
#include "Game/Ui/States/Sleeping.h"
#include "Game/Ui/States/Waking_Up.h"

#ifndef PET_H
#define PET_H

namespace Pet {

void init();

void render();

void eat();

void toggleSleep();

bool isHungry();
bool isSleeping();
bool isTired();

}  // namespace Pet

#endif