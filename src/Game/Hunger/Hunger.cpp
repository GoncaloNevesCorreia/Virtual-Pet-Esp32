#include "Hunger.h"

namespace Pet_Hunger {

const uint8_t LowHunger = 80;
uint8_t hunger = 100;

const unsigned long DECREASE_TIME = 500;
Timer hungerDecreaseTimer(decrease, DECREASE_TIME);

Animate hungerAnimation(&Game::display);

void decrease() {
  if (hunger == 0) return;

  hunger -= 1;
}

void increase() {
  if (hunger == 0) return;

  if (hunger + 10 >= 100) {
    hunger = 100;
  } else {
    hunger += 10;
  }

  hungerDecreaseTimer.reset();
}

void init() {
  // Init stat from EEPROM

  hungerAnimation.set(&Animation_Hunger);
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);
  Game::display.setCursor(108, 22);
  Game::display.print(hunger);
}

void setAnimation() {
  static bool blinking = false;

  if (hunger < LowHunger && !blinking) {
    blinking = true;
    hungerAnimation.set(&Animation_Hunger_Blink);
    return;
  }

  if (hunger >= LowHunger && blinking) {
    blinking = false;
    hungerAnimation.set(&Animation_Hunger);
  }
}

void render() {
  // Render UI
  // Update stat if needed

  hungerDecreaseTimer.run();

  setAnimation();

  printValue();

  hungerAnimation.draw();
}

}  // namespace Pet_Hunger