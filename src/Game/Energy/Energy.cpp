#include "Energy.h"

namespace Pet_Energy {

const uint8_t LowEnergy = 15;

uint8_t energy = 100;

const unsigned long BASE_DECAY_SPEED = 500;
const unsigned long BASE_RECOVER_SPEED = 500;

Timer energyDecreaseTimer(decrease, BASE_DECAY_SPEED);
Timer energyRecoverTimer(increase, BASE_RECOVER_SPEED);

Animate energyAnimation(&Game::display);

void decrease() {
  if (energy == 0) return;
  energy -= 1;
}

void increase() {
  if (energy + 1 >= 100) {
    energy = 100;
  } else {
    energy += 1;
  }
}

void init() {
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);

  Game::display.setCursor(108, 38);
  Game::display.print(energy);
}

// TODO: Adicionar icon de energia
void setAnimation() {
  static bool blinking = false;

  if (energy <= LowEnergy && !blinking) {
    blinking = true;
    // energyAnimation.set(&Animation_Energy_Blink);
    return;
  }

  if ((energy > LowEnergy) && blinking) {
    blinking = false;
    // energyAnimation.set(&Animation_Energy);
  }
}

void handleSleepLogic() {
  static bool wasSleeping = false;

  if (Pet::isSleeping()) {
    if (!wasSleeping) {
      wasSleeping = true;

      energyRecoverTimer.reset();
    }

    if (energy == 100 || Pet::isHungry()) {
      Pet::toggleSleep();
    } else {
      energyRecoverTimer.run();
    }

    return;
  }

  if (wasSleeping) {
    wasSleeping = false;

    energyDecreaseTimer.reset();
  }

  if (energy == 0) {
    Pet::toggleSleep();
    return;
  }

  energyDecreaseTimer.run();
}

void render() {
  handleSleepLogic();

  setAnimation();
  printValue();
}

}  // namespace Pet_Energy