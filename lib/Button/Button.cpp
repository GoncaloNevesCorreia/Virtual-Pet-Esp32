#include "Button.h"

Button::Button(byte pin) : _pin(pin) {}

void Button::onClick(OnClickCallback callback) {
  _callback = callback;

  pinMode(_pin, INPUT_PULLUP);
}

void Button::read() {
  const byte isPressed = (digitalRead(_pin) == LOW);

  if (isPressed && !_executed) {
    _executed = true;

    _callback();
  } else if (!isPressed && _executed) {
    _executed = false;
  }
}