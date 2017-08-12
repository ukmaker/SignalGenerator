#ifndef VIEW_H
#define VIEW_H

#include "Arduino.h"
#include "Display.h"
#include "Joystick.h"

/**
 * Class to represent a view
 * 
 */
class View {

  public:

    View() : active(false), dirty(true), selection(0) {
        
    }
    
    virtual void updateDDS() =0;


  virtual void setActive(boolean _active) {
    active = _active;
    setSelection(1);
  }

  virtual boolean isDirty() {
    return dirty;
  }

virtual void setSelection(int sel) {
    if(selection != sel) {
        selection = sel;
        dirty = true;
    }
}

virtual void draw(Display& display) {
      display.reset();
      dirty = false;
}

  protected:

  boolean active;
  boolean dirty;
  int selection;
  
 };

#endif