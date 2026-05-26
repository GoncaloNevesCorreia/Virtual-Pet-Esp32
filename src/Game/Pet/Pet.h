#include <Animate.h>

#include "Game/Energy/Energy.h"
#include "Game/Fun/Fun.h"
#include "Game/Game.h"
#include "Game/Health/Health.h"
#include "Game/Hunger/Hunger.h"
#include "Game/Ui/States/Dead.h"
#include "Game/Ui/States/Eating.h"
#include "Game/Ui/States/Falling_Asleep.h"
#include "Game/Ui/States/Idle.h"
#include "Game/Ui/States/Playing.h"
#include "Game/Ui/States/Sleeping.h"
#include "Game/Ui/States/Waking_Up.h"

#ifndef PET_H
#define PET_H

namespace Pet {

enum State {
  HAPPY,  // TODO
  IDLE,
  SAD,  // TODO
  PLAYING,

  HUNGRY,  // TODO
  EATING,

  TIRED,  // TODO
  FALLING_ASLEEP,
  SLEEPING,
  WAKING_UP,

  DEAD,
};

typedef void (*OnStateChange)(State newState);

void init();

void render();

void onStateChange(OnStateChange callback);

void eat();

void play();

void toggleSleep();

bool isHungry();
bool isSleeping();
bool isTired();
bool isDead();

}  // namespace Pet

#endif