#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

typedef void (*OnClickCallback)();

class Button {
 public:
  Button(byte pin);

  void onClick(OnClickCallback callback);

  void read();

 private:
  const byte _pin;
  bool _executed = false;
  OnClickCallback _callback;
};

#endif