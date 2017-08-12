#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "Widget.h"
#include "Joystick.h"
#include "ViewStack.h"

class MenuWidget : public Widget {

public:

  MenuWidget(ViewStack& views) : Widget(),  _views(views) {}


    template<typename T>
    void onChange(T* tptr, void (T::*mptr)(void)) {
      _changeHandler.attach(tptr, mptr);
    }

    void onChange() {
      _changeHandler.attach(0);
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
        _views.nextView();
      } else {
        _views.prevView();
      }
      _changed();
    }

    virtual void handleEncoderScroll(int steps) {
      _views.selectView(_views.viewIndex() + steps);
      _changed();
    }

    /**
    * Ignore these events
    **/
    virtual void handleJoystickYMove(int event) {}
    virtual void handleJoystickClick() {}
    virtual void handleEncoderClick() {}

protected:

  ViewStack& _views;
  FunctionPointerArg1<void,void> _changeHandler;

  virtual void _draw(Display& display) {
    display.u8g2.setCursor(0,0);
    display.u8g2.print(_views.currentView()->title());
    display.nextLine();
  }

  void _changed() {
    _dirty = true;
    _changeHandler.call();
  }

};

#endif
