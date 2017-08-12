#ifndef RINGWIDGET_H
#define RINGWIDGET_H

#include "Widget.h"
#include "Joystick.h"
#include "Ring.h"

template <typename T, uint8_t S>
class RingWidget : public Widget {

public:

  RingWidget(Ring<T, S>& ring) : Widget(),  _ring(ring), _text((char*)0) {}


    template<typename T1>
    void onChange(T1* tptr, void (T1::*mptr)(void)) {
      _changeHandler.attach(tptr, mptr);
    }
    
    void onChange(void(* fn)(void)) {
        _changeHandler.attach(fn);
    }

    void onChange() {
      _changeHandler.attach(0);
    }
    
    /**
     * Grubby. Call this to set the text to display
     * Should register a function?
     */
    void setText(char *text) {
        _text = text;
    }

    /***************************
    * Overrides
    ****************************/

    /**
    * Select the next view
    * Raise a Change event
    **/
    virtual void handleJoystickXMove(int event) {
      if(event & JOY_DEC) {
        _ring.next();
      } else {
        _ring.prev();
      }
      _changed();
    }

    virtual void handleEncoderScroll(int steps) {
      _ring.select(_ring.index() + steps);
      _changed();
    }

    /**
    * Ignore these events
    **/
    virtual void handleJoystickYMove(int event) {}
    virtual void handleJoystickClick() {}
    virtual void handleEncoderClick() {}

protected:

  Ring<T, S>& _ring;
  FunctionPointerArg1<void,void> _changeHandler;
  
  char* _text;

  virtual void _draw(Display& display) {
    display.u8g2.setCursor(0,display.ypos);
    display.u8g2.print(_text);
    display.nextLine();
  }

  void _changed() {
    _dirty = true;
    _changeHandler.call();
  }

};

#endif
