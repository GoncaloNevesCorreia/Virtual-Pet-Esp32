#include "Energy.h"

#include <Arduino.h>

#include "../Hunger/Hunger.h"  // Necessário para verificar o nível de fome do pet

namespace Pet_Energy {

// Energia <= 15 equivale ao "sono entre 85 e 100"
const uint8_t LowEnergy = 15;
uint8_t energy = 100;
bool isSleeping = false;

const int BUTTON_SLEEP_PIN = 6;

const unsigned long TICK_TIME = 1000;  // Altera a energia a cada 1 segundo

// Função intermédia para o Timer: a energia desce quando acordado e sobe quando dorme
void updateEnergyTick() {
  if (isSleeping) {
    increase();
  } else {
    decrease();
  }
}

Timer energyTimer(updateEnergyTick, TICK_TIME);

Animate energyAnimation(&Game::display);

void decrease() {
  if (energy == 0) return;
  energy -= 1;
}

void increase() {
  // A recuperar a energia. Usa saltos de +5 para recuperar mais rápido a dormir
  if (energy + 5 >= 100) {
    energy = 100;
    wakeUp();  // Acorda automaticamente se a energia estiver totalmente recuperada
  } else {
    energy += 5;
  }
}

void sleep() {
  isSleeping = true;
}

void wakeUp() {
  isSleeping = false;
}

void init() {
  // Configurar o pino do botão como entrada (assume INPUT_PULLUP, LOW = pressionado)
  pinMode(BUTTON_SLEEP_PIN, INPUT_PULLUP);

  // energyAnimation.set(&Animation_Energy); // Descomentar quando existirem assets
}

void printValue() {
  Game::display.setTextSize(1);
  Game::display.setTextColor(SSD1306_WHITE);
  // Posição ajustada (Y=10) para não sobrepor o texto da Fome que está no Y=22
  Game::display.setCursor(108, 10);
  Game::display.print(energy);
}

void setAnimation() {
  static bool blinking = false;

  // Pisca quando tem muito sono e não está a dormir
  if (energy <= LowEnergy && !blinking && !isSleeping) {
    blinking = true;
    // energyAnimation.set(&Animation_Energy_Blink);
    return;
  }

  // Pára de piscar se estiver a dormir ou se já não tiver sono
  if ((energy > LowEnergy || isSleeping) && blinking) {
    blinking = false;
    // energyAnimation.set(&Animation_Energy);
  }
}

void handleSleepLogic() {
  // Lê o estado do botão
  bool buttonPressed = (digitalRead(BUTTON_SLEEP_PIN) == LOW);

  if (isSleeping) {
    // 1. Se começar a ficar com fome, acorda automaticamente
    if (Pet_Hunger::hunger < Pet_Hunger::LowHunger) {
      wakeUp();
    }
    // 2. Se carregar no botão enquanto dorme, acorda
    else if (buttonPressed) {
      wakeUp();
      delay(200);  // Debounce rápido para evitar duplos cliques no botão
    }
  } else {
    // 3. Se estiver com sono (energia baixa) e carregar no botão, dorme
    if (energy <= LowEnergy && buttonPressed) {
      sleep();
      delay(200);  // Debounce
    }
  }
}

void render() {
  energyTimer.run();

  handleSleepLogic();  // Verifica inputs e regras de estado

  setAnimation();
  printValue();

  // energyAnimation.draw(); // Descomentar quando existirem as sprites
}

}  // namespace Pet_Energy