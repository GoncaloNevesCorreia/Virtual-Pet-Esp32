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

OnStateChange _onStateChangeCallback = nullptr;

bool canInteract() {
  if (currentState == State::DEAD) {
    return false;
  }

  if (currentState == State::EATING) {
    return false;
  }

  if (currentState == State::FALLING_ASLEEP) {
    return false;
  }

  if (currentState == State::WAKING_UP) {
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

bool isHappy() {
  return Pet_Fun::fun > 70;
}

bool isSad() {
  return Pet_Fun::fun < Pet_Fun::LowFun;
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
    case State::HAPPY:
      petAnimation.set(&Animation_Pet_Happy);
      break;
    case State::SAD:
      petAnimation.set(&Animation_Pet_Sad);
      break;
    case State::HUNGRY:
      petAnimation.set(&Animation_Pet_Hungry);
      break;
    case State::TIRED:
      petAnimation.set(&Animation_Pet_Tired);
      break;
    default:
      petAnimation.set(&Animation_Pet_Idle);
      break;
  }
}

void setState(enum State state) {
  if (currentState == state) return;

  currentState = state;

  updateAnimation();

  if (_onStateChangeCallback != nullptr) {
    _onStateChangeCallback();
  }
}

void refreshState() {
  if (isDead() || isSleeping()) return;

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

  if (isHappy()) {
    setState(State::HAPPY);
    return;
  }

  if (isSad()) {
    setState(State::SAD);
    return;
  }

  setState(State::IDLE);
}

void init() {
  if (isDead()) {
    currentState = State::IDLE;
  }

  refreshState();
}

void render() {
  if (Pet_Health::health == 0 && !isDead()) {
    setState(State::DEAD);
  }

  if (canInteract()) {
    refreshState();
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

    petAnimation.onEnd(refreshState);
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

    petAnimation.onEnd(refreshState);
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

void onStateChange(OnStateChange callback) {
  _onStateChangeCallback = callback;
}

uint8_t getState() {
  return currentState;
}

}  // namespace Pet