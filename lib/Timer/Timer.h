#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include <Arduino.h>

#define TIMER_CALLBACK_SIGNATURE std::function<void()> callback

class Timer {
 private:
  unsigned long _lastTime = 0;
  uint16_t _interval;
  std::function<void()> _callback;

 public:
  Timer() {};
  Timer(uint16_t interval) : _interval(interval) {};
  Timer(TIMER_CALLBACK_SIGNATURE, uint16_t interval) : _callback(callback), _interval(interval) {};

  void run();

  void reset();

  void setCallback(TIMER_CALLBACK_SIGNATURE);

  void setInterval(uint16_t interval);
};

#endif