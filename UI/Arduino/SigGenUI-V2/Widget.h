#ifndef WIDGET_H
#define WIDGET_H

#include "Arduino.h"
#include "Display.h"

class Widget {

  public:
  
  Widget() : dirty(true) {
  }

  virtual void draw(Display& display) =0;
  
  boolean isDirty() {
      return dirty;
  }

  
protected:
  boolean dirty;

    
  
};

#endif
