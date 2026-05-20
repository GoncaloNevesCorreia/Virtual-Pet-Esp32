#include "Health.h"

namespace Pet_Health {

bool losingHealth = false;

uint16_t health = 100;

const unsigned long DECREASE_TIME = 1000;
Timer healthDecreaseTimer(decrease, DECREASE_TIME);

void increase() {
  if (health == 0) return;

  if (health + 10 >= 100) {
    health = 100;
  } else {
    health += 10;
  }

  healthDecreaseTimer.reset();
}

void decrease() {
  if (health == 0) return;

  health -= 1;
}

bool shouldLoseHealth() {
  return Pet_Hunger::hunger < 80;
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);
  Game::display.setCursor(108, 6);
  Game::display.print(health);
}

void handle() {
  if (!shouldLoseHealth()) return;

  if (!losingHealth) {
    losingHealth = true;
    healthDecreaseTimer.reset();
  }

  healthDecreaseTimer.run();
}

void init() {
}

void render() {
  handle();

  printValue();
}

}  // namespace Pet_Health