#ifndef PET_HEALTH_H
#define PET_HEALTH_H

#include <Animate.h>
#include <Timer.h>

#include "Game/Game.h"
#include "Game/Ui/Stats/Health.h"

namespace Pet_Health {

void init();

void render();

void increase();

void decrease();

void save();

}  // namespace Pet_Health

#endif