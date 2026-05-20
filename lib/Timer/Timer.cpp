#include "Timer.h"

void Timer::run() {
  const unsigned long now = millis();

  const unsigned long delta = now - _lastTime;

  if (delta < _interval) return;

  _lastTime = now;

  _callback();
}

void Timer::reset() {
  _lastTime = millis();
}

void Timer::setInterval(uint16_t interval) {
  _interval = interval;

  reset();
}