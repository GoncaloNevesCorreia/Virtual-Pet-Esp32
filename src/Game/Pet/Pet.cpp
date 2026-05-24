#include "Game/Pet/Pet.h"

namespace Pet {

enum State {
  HAPPY,  // TODO
  IDLE,
  SAD,      // TODO
  PLAYING,  // TODO

  HUNGRY,  // TODO
  EATING,

  TIRED,  // TODO
  FALLING_ASLEEP,
  SLEEPING,
  WAKING_UP,

  DEAD,
};

enum State currentState = State::IDLE;

Animate petAnimation(&Game::display);

void init() {
  petAnimation.set(&Animation_Pet_Idle);

  Pet_Hunger::init();
  Pet_Health::init();
  Pet_Energy::init();
  Pet_Fun::init();
}

bool canInteract() {
  if (currentState == State::EATING) {
    return false;
  }

  if (currentState == State::FALLING_ASLEEP) {
    return false;
  }

  if (currentState == State::PLAYING) {
    return false;
  }

  return true;
}

bool isHungry() {
  return Pet_Hunger::hunger < Pet_Hunger::LowHunger;
}

bool isTired() {
  return Pet_Energy::energy < Pet_Energy::LowEnergy;
}

bool isSleeping() {
  return currentState == State::SLEEPING;
}

void updateAnimation() {
  switch (currentState) {
    case State::DEAD:
      petAnimation.set(&Animation_Pet_Dead);
      break;
    case State::EATING:
      petAnimation.set(&Animation_Pet_Eating);
      break;
    case State::FALLING_ASLEEP:
      petAnimation.set(&Animation_Pet_Falling_Asleep);
      break;
    case State::SLEEPING:
      petAnimation.set(&Animation_Pet_Sleeping);
      break;
    case State::WAKING_UP:
      petAnimation.set(&Animation_Pet_Waking_Up);
      break;
    case State::PLAYING:
      petAnimation.set(&Animation_Pet_Playing);
      break;
    default:
      petAnimation.set(&Animation_Pet_Idle);
      break;
  }
}

void setState(enum State state) {
  currentState = state;

  updateAnimation();
}

enum State getState() {
  return currentState;
}

void refreshState() {
  if (currentState == State::FALLING_ASLEEP) {
    setState(State::SLEEPING);
    return;
  }

  if (isHungry()) {
    setState(State::HUNGRY);
    return;
  }

  if (isTired()) {
    setState(State::TIRED);
    return;
  }

  setState(State::IDLE);
}

void render() {
  petAnimation.draw();

  Pet_Hunger::render();
  Pet_Health::render();
  Pet_Energy::render();
  Pet_Fun::render();
}

void eatEnd() {
  Pet_Hunger::increase();

  refreshState();
}

void eat() {
  if (!canInteract()) return;

  if (currentState == State::SLEEPING) {
    setState(State::WAKING_UP);

    petAnimation.onEnd(eat);
    return;
  }

  setState(State::EATING);

  petAnimation.onEnd(eatEnd);
}

void playEnd() {
  Pet_Fun::increase();

  refreshState();
}

void play() {
  if (!canInteract()) return;

  if (currentState == State::SLEEPING) {
    setState(State::WAKING_UP);

    petAnimation.onEnd(play);
    return;
  }

  setState(State::PLAYING);

  petAnimation.onEnd(playEnd);
}

void toggleSleep() {
  if (!canInteract()) return;

  if (currentState == State::SLEEPING) {
    setState(State::WAKING_UP);
  } else {
    if (currentState == State::HUNGRY) return;

    setState(State::FALLING_ASLEEP);
  }

  petAnimation.onEnd(refreshState);
}

}  // namespace Pet