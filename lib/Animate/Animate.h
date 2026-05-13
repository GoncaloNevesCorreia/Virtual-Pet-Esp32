#ifndef ANIMATE_H
#define ANIMATE_H

#include <Arduino.h>

class Animate {
 private:
  unsigned long lastFrameTime = 0;
  unsigned long animationDelay = 500;

  unsigned char* bitmap;

 public:
  void setSize(byte size);
  void setBitmap(unsigned char* bitmap);
  void setDelay(unsigned long animationDelay);

  void update();
};

#endif