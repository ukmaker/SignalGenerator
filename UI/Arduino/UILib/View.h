#ifndef VIEW_H
#define VIEW_H

#include "Display.h"
#include "Widget.h"

/**
 * Class to represent a generic view
 *
 */
class View {

  public:

    View(char *title) : _title(title), _active(false), _dirty(true) {

    }

  virtual void setActive(bool active) {
    _active = _active;
  }

  virtual bool isDirty() {
    return _dirty;
  }

virtual void setSelection(int sel) =0;

virtual int selection() =0;

virtual int selections() =0;

virtual void draw(Display& display) {
      _dirty = false;
      _draw(display);
}

virtual void append(Widget* widget) =0;

virtual Widget* selectedWidget() =0;

char *title() {
    return _title;
}

  protected:

  char *_title;
  bool _active;
  bool _dirty;

  virtual void _draw(Display& display) =0;

 };

#endif
