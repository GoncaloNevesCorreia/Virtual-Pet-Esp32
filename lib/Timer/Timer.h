#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include <Arduino.h>

typedef void (*Callback)();

class Timer {
 private:
  unsigned long _lastTime = 0;
  const uint16_t _interval;
  Callback _callback;

 public:
  Timer(Callback callback, uint16_t interval) : _callback(callback), _interval(interval) {};

  void run();

  void reset();
};

#endif