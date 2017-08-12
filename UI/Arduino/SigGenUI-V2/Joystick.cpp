#include "Joystick.h"

Joystick::Joystick( Axis& _x, Axis& _y, OneButton& _button) :
  x(_x), y(_y), button(_button)
{

  
  
}

void Joystick::update() {
  x.update();
  y.update();
}

