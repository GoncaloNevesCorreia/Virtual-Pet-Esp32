#include "Energy.h"

namespace Pet_Energy {
const unsigned long INCREASE_AMOUNT = 1;
const unsigned long DECREASE_AMOUNT = 1;

const unsigned long BASE_DECAY_SPEED = 5000;
const unsigned long BASE_RECOVER_SPEED = 500;

#define STORAGE_KEY "PET_ENERGY"

const uint8_t LowEnergy = 15;

uint8_t energy = 100;

Timer energyDecreaseTimer(decrease, BASE_DECAY_SPEED);
Timer energyRecoverTimer(increase, BASE_RECOVER_SPEED);

Animate energyIcon(&Game::display);

void increase() {
  if (energy + INCREASE_AMOUNT >= 100) {
    energy = 100;
  } else {
    energy += INCREASE_AMOUNT;
  }
}

void decrease() {
  if (energy <= DECREASE_AMOUNT) {
    energy = 0;
    return;
  }

  energy -= DECREASE_AMOUNT;
}

void decreaseAmount(unsigned long amount) {
  if (energy <= amount) {
    energy = 0;
    return;
  }

  energy -= amount;
}

void setIconAnimation() {
  static bool blinking = false;

  if (energy <= LowEnergy && !blinking) {
    blinking = true;
    energyIcon.set(&Animation_Energy_Blink);
    return;
  }

  if ((energy > LowEnergy) && blinking) {
    blinking = false;
    energyIcon.set(&Animation_Energy);
  }
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);

  if (energy < 10) {
    Game::display.setCursor(107, 4);
  } else if (energy < 100) {
    Game::display.setCursor(101, 4);
  } else {
    Game::display.setCursor(95, 4);
  }

  Game::display.print(energy);
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

void init() {
  energy = Storage::load(STORAGE_KEY);

  energyIcon.set(&Animation_Energy);
}

void render() {
  handleSleepLogic();

  setIconAnimation();

  printValue();

  energyIcon.draw();
}

void save() {
  Storage::save(STORAGE_KEY, energy);
}

}  // namespace Pet_Energy