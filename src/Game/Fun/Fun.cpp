#include "Fun.h"

namespace Pet_Fun {
const unsigned long INCREASE_AMOUNT = 10;
const unsigned long DECREASE_AMOUNT = 1;

const unsigned long BASE_DECAY_SPEED = 5000;

#define STORAGE_KEY "PET_FUN"

const uint8_t LowFun = 80;
uint8_t fun = 100;

Timer funDecreaseTimer(decrease, BASE_DECAY_SPEED);

Animate funIcon(&Game::display);

void increase() {
  if (fun + INCREASE_AMOUNT >= 100) {
    fun = 100;
  } else {
    fun += INCREASE_AMOUNT;
  }

  funDecreaseTimer.reset();
}

void decrease() {
  if (fun <= DECREASE_AMOUNT) {
    fun = 0;
    return;
  }

  fun -= DECREASE_AMOUNT;
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);
  Game::display.setCursor(17, 54);
  Game::display.print(fun);
}

void setIconAnimation() {
  static bool blinking = false;

  if (fun < LowFun && !blinking) {
    blinking = true;
    funIcon.set(&Animation_Fun_Blink);
    return;
  }

  if (fun >= LowFun && blinking) {
    blinking = false;
    funIcon.set(&Animation_Fun);
  }
}

void handleFunLogic() {
  static bool wasSleeping = false;

  if (Pet::isSleeping()) {
    if (!wasSleeping) {
      wasSleeping = true;

      funDecreaseTimer.setInterval(BASE_DECAY_SPEED * 3);
    }
  } else if (wasSleeping) {
    wasSleeping = false;
    funDecreaseTimer.setInterval(BASE_DECAY_SPEED);
  }

  funDecreaseTimer.run();
}

void init() {
  fun = Storage::load(STORAGE_KEY);

  funIcon.set(&Animation_Fun);
}

void render() {
  handleFunLogic();

  setIconAnimation();

  printValue();

  funIcon.draw();
}

void save() {
  Storage::save(STORAGE_KEY, fun);
}

}  // namespace Pet_Fun