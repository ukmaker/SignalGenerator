#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <OneButton.h>
#include "FunctionPointerArg1.h"
#include "Axis.h"

class Joystick {

  public:

  Joystick(Axis& x, Axis& y, OneButton& button);

  Axis& x;
  Axis& y;

  OneButton& button;

  void update();
  
  void clearEventHandlers() {
      x.clearEventHandler();
      y.clearEventHandler();
      clearClickEventHandler();
  }
  
  void handleClick() {
      _fp.call();
  }
  
  
  FunctionPointerArg1<void,void> _fp;

  template<typename T>
  void setClickEventHandler(T* tptr, void (T::*mptr)(void)) {
    _fp.attach(tptr, mptr);
  }

  void clearClickEventHandler() {
    _fp.attach(0);
  }
  
  
};

#endif
