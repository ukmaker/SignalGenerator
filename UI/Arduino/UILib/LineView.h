#ifndef LINEVIEW_H
#define LINEVIEW_H

#ifndef LINEVIEW_WIDGETS
#define LINEVIEW_WIDGETS 6
#endif

#include "View.h"
#include "Widget.h"

/**
* View designed to embed widgets which display line-by-line
* Selection (line) 0 is reserved for the Platform MenuWidget
**/
class LineView : public View {

public:

  LineView(char *title) : View(title), _widgetCount(0) {
    setSelection(0);
  }

  /**
  * Supplying sel=0 means nothing in this view is selected
  **/
  virtual void setSelection(int sel) {

      if(sel < 0) {
          sel = _widgetCount - 1;
      } else if(sel >= _widgetCount) {
          sel = 0;
      }

      if(_selection != sel) {
          _selection = sel;
          _dirty = true;
      }
  }
  
  virtual int selection() {
      return _selection;
  }

  /**
  * Return the number of available selections in this view
  **/
  virtual int selections() {
    return _widgetCount + 1;
  }


  virtual void append(Widget* widget) {
    if(_widgetCount < LINEVIEW_WIDGETS) {
      _widgets[_widgetCount] = widget;
      _widgetCount++;
    }
  }
  
  virtual Widget* selectedWidget() {
    return _widgets[_selection];  
  }
  
  virtual bool isDirty() {
    
      if(_dirty) return true;
      
    for(int i=0; i<_widgetCount; i++) {
      if(_widgets[i]->isDirty()) return true;
    }
    
    return false;
  }

protected:

  Widget *_widgets[LINEVIEW_WIDGETS];
  int _widgetCount;
  int _selection;

  virtual void _draw(Display& display) {

    // Menu widget has already been drawn
    for(int i=0; i<_widgetCount; i++) {
      _widgets[i]->draw(display);
    }
  }


};

#endif
