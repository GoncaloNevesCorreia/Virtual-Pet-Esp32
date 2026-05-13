#include "Game/Pet/Pet.h"

#include "Game/Pet/State/Dead.h"

namespace Pet {

enum State {
  DEAD,
  IDLE,
};

enum State currentState = State::DEAD;

int animationLength = 3;
int currentFrame = 0;

const unsigned long FrameDelay = 500;
unsigned long lastFrameTime = 0;

void draw() {
  Game::display.drawBitmap(48, 16, bitmap_pet_dead[currentFrame], 32, 32, 1);
}

void update() {
  const unsigned long now = millis();

  if (now - lastFrameTime < FrameDelay) return;

  lastFrameTime = now;

  currentFrame = (currentFrame + 1) % animationLength;
}

void render() {
  draw();
  update();
}

}  // namespace Pet