#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <OneButton.h>
#include "FunctionPointerArg1.h"
#include "Axis.h"

class Joystick {

  public:

  Joystick(Axis& _x, Axis& _y, OneButton& _button) :
    x(_x), y(_y), button(_button) {}
    

  Axis& x;
  Axis& y;

  OneButton& button;

  void update() {
      x.update();
      y.update();
  }
  
  void clearEventHandlers() {
      x.clearEventHandler();
      y.clearEventHandler();
      clearClickHandler();
  }
  
  void handleClick() {
      _fp.call();
  }
  
  
  FunctionPointerArg1<void,void> _fp;

  template<typename T>
  void setClickHandler(T* tptr, void (T::*mptr)(void)) {
    _fp.attach(tptr, mptr);
  }

  void clearClickHandler() {
    _fp.attach(0);
  }
  
  
};

#endif
