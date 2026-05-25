#ifndef PET_HEALTH_H
#define PET_HEALTH_H

#include <Animate.h>
#include <Timer.h>

#include "Game/Game.h"
#include "Game/Ui/Stats/Health.h"

namespace Pet_Health {

extern uint8_t health;

void init();

void render();

void increase();

void decrease();

void save();

void clear();

}  // namespace Pet_Health

#endif