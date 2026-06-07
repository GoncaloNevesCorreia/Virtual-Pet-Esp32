#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include <Arduino.h>

#define TIMER_CALLBACK_SIGNATURE std::function<void()> callback

class Timer {
 private:
  unsigned long _lastTime = 0;
  int _interval;
  std::function<void()> _callback;

 public:
  Timer() {};
  Timer(int interval) : _interval(interval) {};
  Timer(TIMER_CALLBACK_SIGNATURE, int interval) : _callback(callback), _interval(interval) {};

  void run();

  void reset();

  void setCallback(TIMER_CALLBACK_SIGNATURE);

  void setInterval(int interval);
};

#endif