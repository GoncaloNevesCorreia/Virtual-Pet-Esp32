#include "Game/Pet/Pet.h"

namespace Pet {

enum State {
  DEAD,
  EATING,
  IDLE,
};

enum State previousState = State::IDLE;
enum State currentState = State::IDLE;

Animate petAnimation(&Game::display);

void init() {
  petAnimation.set(&Animation_Pet_Idle);

  Pet_Hunger::init();
}

void render() {
  petAnimation.draw();

  Pet_Hunger::render();
}

void updateAnimation() {
  switch (currentState) {
    case State::DEAD:
      petAnimation.set(&Animation_Pet_Dead);
      break;
    case State::EATING:
      petAnimation.set(&Animation_Pet_Eating);
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

void eatEnd() {
  Pet_Hunger::increase();

  revertState();
}

void eat() {
  if (currentState == State::EATING) return;

  setState(State::EATING);

  petAnimation.onEnd(eatEnd);
}

}  // namespace Pet