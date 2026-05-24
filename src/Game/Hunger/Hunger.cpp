#include "Hunger.h"

namespace Pet_Hunger {
const unsigned long INCREASE_AMOUNT = 10;
const unsigned long DECREASE_AMOUNT = 1;

const unsigned long BASE_DECAY_SPEED = 500;

#define STORAGE_KEY "PET_HUNGER"

const uint8_t LowHunger = 15;
uint8_t hunger = 100;

Timer hungerDecreaseTimer(decrease, BASE_DECAY_SPEED);

Animate hungerIcon(&Game::display);

void increase() {
  if (hunger + INCREASE_AMOUNT >= 100) {
    hunger = 100;
  } else {
    hunger += INCREASE_AMOUNT;
  }

  hungerDecreaseTimer.reset();
}

void decrease() {
  if (hunger <= DECREASE_AMOUNT) {
    hunger = 0;
    return;
  }

  hunger -= DECREASE_AMOUNT;
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);

  if (hunger < 10) {
    Game::display.setCursor(107, 54);
  } else if (hunger < 100) {
    Game::display.setCursor(101, 54);
  } else {
    Game::display.setCursor(95, 54);
  }

  Game::display.print(hunger);
}

void setIconAnimation() {
  static bool blinking = false;

  if (hunger < LowHunger && !blinking) {
    blinking = true;
    hungerIcon.set(&Animation_Hunger_Blink);
    return;
  }

  if (hunger >= LowHunger && blinking) {
    blinking = false;
    hungerIcon.set(&Animation_Hunger);
  }
}

void handleHungerLogic() {
  static bool wasSleeping = false;

  if (Pet::isSleeping()) {
    if (!wasSleeping) {
      wasSleeping = true;

      hungerDecreaseTimer.setInterval(BASE_DECAY_SPEED * 3);
    }
  } else if (wasSleeping) {
    wasSleeping = false;
    hungerDecreaseTimer.setInterval(BASE_DECAY_SPEED);
  }

  hungerDecreaseTimer.run();
}

void init() {
  hunger = Storage::load(STORAGE_KEY);

  hungerIcon.set(&Animation_Hunger);
}

void render() {
  handleHungerLogic();

  setIconAnimation();

  printValue();

  hungerIcon.draw();
}

void save() {
  Storage::save(STORAGE_KEY, hunger);
}

}  // namespace Pet_Hunger