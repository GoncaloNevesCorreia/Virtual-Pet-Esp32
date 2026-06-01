#include <Animate.h>

#include "Game/Energy/Energy.h"
#include "Game/Fun/Fun.h"
#include "Game/Game.h"
#include "Game/Health/Health.h"
#include "Game/Hunger/Hunger.h"
#include "Game/Ui/States/Dead.h"
#include "Game/Ui/States/Eating.h"
#include "Game/Ui/States/Falling_Asleep.h"
#include "Game/Ui/States/Happy.h"
#include "Game/Ui/States/Hungry.h"
#include "Game/Ui/States/Idle.h"
#include "Game/Ui/States/Playing.h"
#include "Game/Ui/States/Sad.h"
#include "Game/Ui/States/Sleeping.h"
#include "Game/Ui/States/Tired.h"
#include "Game/Ui/States/Waking_Up.h"

#ifndef PET_H
#define PET_H

namespace Pet {

typedef void (*OnStateChange)();

void init();

void render();

void onStateChange(OnStateChange callback);

uint8_t getState();

void eat();

void play();

void toggleSleep();

bool isHungry();
bool isSleeping();
bool isTired();
bool isDead();

}  // namespace Pet

#endif