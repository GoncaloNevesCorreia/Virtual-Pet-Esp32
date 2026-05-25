#include "Game/Pet/Pet.h"

namespace Pet {

enum State {
  HAPPY,  // TODO
  IDLE,
  SAD,  // TODO
  PLAYING,

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

bool isDead() {
  return currentState == State::DEAD;
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

void init() {
  refreshState();
}

void render() {
  if (Pet_Health::health == 0 && !isDead()) {
    setState(State::DEAD);
  }

  petAnimation.draw();
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
  Pet_Energy::decreaseAmount(Pet_Fun::INCREASE_AMOUNT);
  Pet_Hunger::decreaseAmount(Pet_Fun::INCREASE_AMOUNT);

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