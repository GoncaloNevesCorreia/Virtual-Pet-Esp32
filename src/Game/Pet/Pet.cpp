#include "Game/Pet/Pet.h"

namespace Pet {

enum State {
  DEAD,
  IDLE,
};

enum State currentState = State::IDLE;

Animate petAnimation(&Game::display);

void init() {
  petAnimation.set(&Animation_Pet_Idle);
}

void render() {
  petAnimation.draw();
}

}  // namespace Pet