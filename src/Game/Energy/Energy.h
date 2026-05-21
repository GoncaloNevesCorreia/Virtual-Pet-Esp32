#ifndef PET_ENERGY_H
#define PET_ENERGY_H

#include <Animate.h>
#include <Timer.h>

#include "Game/Game.h"
// Descomentar a linha abaixo quando tiveres o ficheiro das Animações da UI de Energia criado
// #include "Game/Ui/Stats/Energy.h"

namespace Pet_Energy {

extern const uint8_t LowEnergy;
extern uint8_t energy;
extern bool isSleeping;  // Exposto para que o Pet.cpp saiba quando parar as animações de movimento

void init();
void render();
void increase();
void decrease();

void sleep();
void wakeUp();

}  // namespace Pet_Energy

#endif