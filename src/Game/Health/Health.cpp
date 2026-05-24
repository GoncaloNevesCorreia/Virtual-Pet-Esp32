#include "Health.h"

namespace Pet_Health {

bool losingHealth = false;

uint16_t health = 100;

const unsigned long BASE_DECAY_SPEED = 2000;
const unsigned long BASE_RECOVER_SPEED = 2000;

Timer healthDecreaseTimer(decrease, BASE_DECAY_SPEED);
Timer healthRecoverTimer(increase, BASE_RECOVER_SPEED);

Animate healthAnimation(&Game::display);

void increase() {
  if (health == 0) return;

  if (health + 1 >= 100) {
    health = 100;
  } else {
    health += 1;
  }
}

void decrease() {
  if (health == 0) return;

  health -= 1;
}

uint8_t getDecayModifier() {
  uint8_t modifier = 0;

  if (Pet_Hunger::hunger < Pet_Hunger::LowHunger) {
    modifier = modifier + 1;
  }

  if (Pet_Hunger::hunger == 0) {
    modifier = modifier + 2;
  }

  return modifier;
}

void setAnimation() {
  static bool blinking = false;

  if (losingHealth && !blinking) {
    blinking = true;
    healthAnimation.set(&Animation_Health_Blink);
    return;
  }

  if (!losingHealth && blinking) {
    blinking = false;
    healthAnimation.set(&Animation_Health);
  }
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);
  Game::display.setCursor(108, 6);
  Game::display.print(health);
}

void handle() {
  const uint8_t modifier = getDecayModifier();

  if (modifier == 0) {
    if (losingHealth) {
      losingHealth = false;

      healthRecoverTimer.reset();
    }

    healthRecoverTimer.run();
    return;
  }

  if (!losingHealth) {
    losingHealth = true;
    healthDecreaseTimer.setInterval(BASE_DECAY_SPEED - (200 * modifier));
  }

  healthDecreaseTimer.run();
}

void init() {
  healthAnimation.set(&Animation_Health);
}

void render() {
  handle();

  setAnimation();

  printValue();

  healthAnimation.draw();
}

}  // namespace Pet_Health