#include "Fun.h"

namespace Pet_Fun {

#define STORAGE_KEY "pet_fun"

const uint8_t LowFun = 80;
uint8_t fun = 100;

const unsigned long DECREASE_TIME = 1000;
Timer funDecreaseTimer(decrease, DECREASE_TIME);

Animate funAnimation(&Game::display);

void decrease() {
  if (fun == 0) return;

  fun -= 1;
}

void increase() {
  if (fun + 10 >= 100) {
    fun = 100;
  } else {
    fun += 10;
  }

  funDecreaseTimer.reset();
}

void init() {
  // Init stat from EEPROM

  fun = Storage::load(STORAGE_KEY);

  funAnimation.set(&Animation_Fun);
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);
  Game::display.setCursor(108, 52);
  Game::display.print(fun);
}

void setAnimation() {
  static bool blinking = false;

  if (fun < LowFun && !blinking) {
    blinking = true;
    funAnimation.set(&Animation_Fun_Blink);
    return;
  }

  if (fun >= LowFun && blinking) {
    blinking = false;
    funAnimation.set(&Animation_Fun);
  }
}

void handle() {
  static bool wasSleeping = false;

  if (Pet::isSleeping()) {
    if (!wasSleeping) {
      wasSleeping = true;

      funDecreaseTimer.setInterval(DECREASE_TIME * 3);
    }
  } else if (wasSleeping) {
    wasSleeping = false;
    funDecreaseTimer.setInterval(DECREASE_TIME);
  }

  funDecreaseTimer.run();
}

void render() {
  // Render UI
  // Update stat if needed

  handle();
  setAnimation();

  printValue();

  funAnimation.draw();
}

void save() {
  Storage::save(STORAGE_KEY, fun);
}

}  // namespace Pet_Fun