#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <OneButton.h>

#include "Axis.h"

class Joystick {

  public:

  Joystick(Axis x, Axis y, OneButton button);

  Axis x;
  Axis y;

  OneButton button;
  
};

#endif
