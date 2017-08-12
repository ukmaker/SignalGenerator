#include "Display.h"

Display::Display(U8G2& _u8g2) : u8g2(_u8g2) {

  lineHeight = 0;
  charWidth = 0;
  xpos = 0;
  ypos = 0;
  
}

void Display::nextLine() {
  xpos = 0;
  ypos += lineHeight;
}

void Display::reset() {
    xpos = 0;
    ypos = 0;
}