#include "Health.h"

namespace Pet_Health {
const uint8_t MAX_HEALTH = 100;

const unsigned long INCREASE_AMOUNT = 1;
const unsigned long DECREASE_AMOUNT = 1;

const unsigned long BASE_DECAY_SPEED = 1200;
const unsigned long BASE_RECOVER_SPEED = 1000;

#define STORAGE_KEY "PET_HEALTH"

bool losingHealth = false;

uint8_t health = MAX_HEALTH;

Timer healthDecreaseTimer(decrease, BASE_DECAY_SPEED);
Timer healthRecoverTimer(increase, BASE_RECOVER_SPEED);

Animate healthIcon(&Game::display);

void increase() {
  if (health + INCREASE_AMOUNT >= MAX_HEALTH) {
    health = MAX_HEALTH;
  } else {
    health += INCREASE_AMOUNT;
  }
}

void decrease() {
  if (health <= DECREASE_AMOUNT) {
    health = 0;
    return;
  }

  health -= DECREASE_AMOUNT;
}

void setIconAnimation() {
  static bool blinking = false;

  if (losingHealth && !blinking) {
    blinking = true;
    healthIcon.set(&Animation_Health_Blink);
    return;
  }

  if (!losingHealth && blinking) {
    blinking = false;
    healthIcon.set(&Animation_Health);
  }
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);
  Game::display.setCursor(16, 4);
  Game::display.print(health);
}

uint8_t getDecayModifier() {
  uint8_t modifier = 0;

  if (Pet_Hunger::hunger < Pet_Hunger::LowHunger) {
    modifier += 1;
  }

  if (Pet_Hunger::hunger == 0) {
    modifier += 2;
  }

  if (Pet_Fun::fun == 0) {
    modifier += 1;
  }

  return modifier;
}

void handleHealthLogic() {
  static uint8_t previousModifier = 0;

  if (Pet::isDead()) return;

  const uint8_t modifier = getDecayModifier();

  if (modifier == previousModifier) {
    if (modifier == 0) {
      healthRecoverTimer.run();
    } else {
      healthDecreaseTimer.run();
    }
    return;
  }

  if (modifier == 0) {
    losingHealth = false;
    previousModifier = 0;

    healthRecoverTimer.reset();
  } else {
    losingHealth = true;
    previousModifier = modifier;
    healthDecreaseTimer.setInterval(BASE_DECAY_SPEED - constrain((200 * modifier), 0, BASE_DECAY_SPEED));
  }
}

void init() {
  health = Storage::load(STORAGE_KEY);

  healthIcon.set(&Animation_Health);
}

void render() {
  handleHealthLogic();

  setIconAnimation();

  printValue();

  healthIcon.draw();
}

void save() {
  static uint8_t prev = 0;

  if (health == prev) return;

  prev = health;

  Storage::save(STORAGE_KEY, health);
}

void clear() {
  Storage::save(STORAGE_KEY, MAX_HEALTH);
}

}  // namespace Pet_Health