#include "Timer.h"

void Timer::run() {
  if (_callback == nullptr) return;

  const unsigned long now = millis();

  const unsigned long delta = now - _lastTime;

  if (delta < _interval) return;

  _lastTime = now;

  _callback();
}

void Timer::reset() {
  _lastTime = millis();
}

void Timer::setCallback(TIMER_CALLBACK_SIGNATURE) {
  _callback = callback;
}

void Timer::setInterval(uint16_t interval) {
  _interval = interval;

  reset();
}