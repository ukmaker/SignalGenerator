#ifndef AXIS_H
#define AXIS_H

#include "Arduino.h"

#include "FunctionPointerArg1.h"

#define JOY_DEBOUNCE   20
#define JOY_SHORT_HOLD 3000
#define JOY_LONG_HOLD  7000
#define JOY_REPEAT 500
#define JOY_FAST_REPEAT 100

#define JOY_DIRECTIONS_MASK 3
#define JOY_INC 1
#define JOY_DEC 2

#define JOY_SPEED_MASK 12
#define JOY_SLOW 4
#define JOY_FAST 8


class Axis {

  public:
  
  Axis(int input);
  
  int currentValue;
  int lastValue;
  unsigned long lastChanged;
  int events;
  boolean holding;
  int shortHold;
  int longHold;
  int repeat;
  int fastRepeat;

  FunctionPointerArg1<void,int> _fp;

  void update();

  template<typename T>
  void setEventHandler(T* tptr, void (T::*mptr)(int)) {
    _fp.attach(tptr, mptr);
  }

  void clearEventHandler() {
    _fp.attach(0);
  }

  private:
  
  int input;
  
};

#endif
