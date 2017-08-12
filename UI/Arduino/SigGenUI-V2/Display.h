#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include <U8g2lib.h>

class Display {

public:

  Display(U8G2& u8g2);

  U8G2& u8g2;
  int lineHeight;
  int charWidth;

  int xpos;
  int ypos;

  void nextLine();
  void reset();
}
;

#endif
