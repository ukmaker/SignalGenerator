#ifndef WIDGET_H
#define WIDGET_H

#include "Display.h"
#include "Joystick.h"

/**
 * Base Widget class which understands events sourced by a Joystick
 */

 class Widget {
     
  public:

  Widget() : _dirty(true), _active(true) {}

  /**
   * Base class provides no implementation
   */
   virtual void handleJoystickXMove(int event) {}
   virtual void handleJoystickYMove(int event) {}
   virtual void handleJoystickClick() {}
   virtual void handleEncoderClick() {}
   virtual void handleEncoderScroll(int steps) {}

  bool isDirty() {
    return _dirty;
  }
  
  bool isActive() {
      return _active;
  }
  
  void setActive(bool a) {
      _active = a;
  }

  virtual void draw(Display& display) {
    _dirty = false;
    if(_active) {
        _draw(display);
    }
  };



  protected:
  bool _dirty;
  bool _active;

  virtual void _draw(Display& display) =0;

};

#endif
