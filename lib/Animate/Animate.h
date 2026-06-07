#ifndef ANIMATE_H
#define ANIMATE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

typedef void (*OnAnimationEnd)();

typedef struct {
  const int16_t x;
  const int16_t y;

  const int16_t width;
  const int16_t height;

  const uint8_t length;
  const uint8_t* bitmap;

  const unsigned long delay;
} Animation;

class Animate {
 private:
  unsigned long _lastFrameTime = 0;
  uint8_t _currentFrame = 0;
  uint8_t _frameSize = 0;

  const uint8_t* _framePtr = nullptr;

  Adafruit_SSD1306* _display = nullptr;
  const Animation* _animation = nullptr;

  OnAnimationEnd _onEndCallback = nullptr;

  void updateFrame();

 public:
  Animate(Adafruit_SSD1306* display) : _display(display) {}

  bool hasAnimation();

  void set(const Animation* animation);

  void draw();

  void onEnd(OnAnimationEnd callback);
};

#endif