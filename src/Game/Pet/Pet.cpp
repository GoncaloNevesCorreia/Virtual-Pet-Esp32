#include "Game/Pet/Pet.h"

#include "../Energy/Energy.h"  // Incluir o sistema de energia

namespace Pet {

enum State {
  DEAD,
  EATING,
  SLEEPING,  // Novo estado adicionado
  IDLE,
};

enum State previousState = State::IDLE;
enum State currentState = State::IDLE;

Animate petAnimation(&Game::display);

void init() {
  petAnimation.set(&Animation_Pet_Idle);

  Pet_Hunger::init();
  Pet_Health::init();
  Pet_Energy::init();  // Inicializar a Energia
}

void updateAnimation() {
  switch (currentState) {
    case State::DEAD:
      petAnimation.set(&Animation_Pet_Dead);
      break;
    case State::EATING:
      petAnimation.set(&Animation_Pet_Eating);
      break;
    case State::SLEEPING:
      // petAnimation.set(&Animation_Pet_Sleeping); // Descomentar quando tiveres esta animação
      break;
    default:
      petAnimation.set(&Animation_Pet_Idle);
      break;
  }
}

void setState(enum State state) {
  previousState = currentState;
  currentState = state;

  updateAnimation();
}

void revertState() {
  currentState = previousState;

  updateAnimation();
}

// Função para sincronizar a variável isSleeping com a máquina de estados do Pet
void handleSleepState() {
  // Se o botão mandou dormir, e o Pet ainda não está no estado SLEEPING
  if (Pet_Energy::isSleeping && currentState != State::SLEEPING) {
    setState(State::SLEEPING);
  }
  // Se o botão (ou a fome/energia cheia) mandou acordar, e o Pet ainda está a dormir
  else if (!Pet_Energy::isSleeping && currentState == State::SLEEPING) {
    setState(State::IDLE);  // Acorda e volta a ficar IDLE
  }
}

void render() {
  handleSleepState();  // Verifica se deve adormecer/acordar antes de desenhar

  petAnimation.draw();

  Pet_Hunger::render();
  Pet_Health::render();
  Pet_Energy::render();  // Renderizar a UI e Timer da Energia
}

void eatEnd() {
  Pet_Hunger::increase();

  revertState();
}

void eat() {
  // Se já estiver a comer, ou se estiver a dormir, não faz nada
  if (currentState == State::EATING || currentState == State::SLEEPING) return;

  setState(State::EATING);

  petAnimation.onEnd(eatEnd);
}

}  // namespace Pet