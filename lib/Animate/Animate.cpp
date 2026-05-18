#include "Animate.h"

void Animate::updateFrame() {
  _framePtr = _animation->bitmap + (_currentFrame * _frameSize);
}

void Animate::set(const Animation* animation) {
  _animation = animation;
  _lastFrameTime = 0;
  _currentFrame = 0;
  _frameSize = _animation->width * _animation->height / 8;
  _framePtr = _animation->bitmap + (_currentFrame * _frameSize);
}

void Animate::draw() {
  if (_display == nullptr || _animation == nullptr) return;

  _display->drawBitmap(_animation->x, _animation->y, _framePtr, _animation->width, _animation->height, 1);

  const unsigned long now = millis();

  if (now - _lastFrameTime < _animation->delay) return;

  _lastFrameTime = now;

  _currentFrame = (_currentFrame + 1) % _animation->length;

  updateFrame();
}