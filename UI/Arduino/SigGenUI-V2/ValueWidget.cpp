#include "ValueWidget.h"
#include <U8g2lib.h>


ValueWidget::ValueWidget(char *_label, int_fast32_t _max, int_fast32_t _min, int _maxDecade, int _minDecade, int _scale) 
: Widget(), label(_label), maximum(_max), minimum(_min), maxDecade(_maxDecade), minDecade(_minDecade), scale(_scale) {

  // Value is mV
  value = 1000;
  increment = 1000;
  // or 1V
  decade = 3;

    
}

/**
 * Values can only range from -10V to +10V
 * Allow control down to 10mV
 * Treat values as mV
 * 
 * - 9.43 V
 */
void ValueWidget::draw(Display& display) {
  
  int_fast32_t v = abs(value);
  int digit;
  int xpos = display.charWidth * 10;
  int digitDecade = minDecade;
  boolean printing = true;

  U8G2& u8g2 = display.u8g2;
  
  /**
   * Adjust the starting value to the precision given by minDecade
   */
  for(int i=0; i<minDecade; i++) {
      v = v / 10;
  }

  while (printing) {

    digit = v % 10;
    v = v / 10;
    u8g2.setCursor(xpos, display.ypos);
    u8g2.print(digit);

    if (decade == digitDecade) u8g2.drawHLine(xpos, display.ypos + display.lineHeight - 1, display.charWidth);

    xpos -= display.charWidth;
    digitDecade++;

    printing = v || digitDecade <= decade || digitDecade <= scale;

    if (printing && (digitDecade == scale) ) {
      u8g2.setCursor(xpos, display.ypos);
      u8g2.print(".");
      xpos -= display.charWidth;
    }
    
    if (printing && ((digitDecade - scale) % 3) == 0 && digitDecade > scale) {
      u8g2.setCursor(xpos, display.ypos);
      u8g2.print(",");
      xpos -= display.charWidth;
    }   
  }
  
  if(value < 0) {
      u8g2.setCursor(xpos, display.ypos);
      u8g2.print("-");
  }

  u8g2.setCursor(11 * display.charWidth, display.ypos);
  u8g2.print(label);
  
  display.nextLine();
  dirty = false;
}

void ValueWidget::setToDecade() {
  int_fast32_t v = value;
  for (int i = 0; i < decade; i++) {
    v = v / 10;
  }

  v = v % 10;

  for (int i = 0; i < decade; i++) {
    v = v * 10;
  }

  value = v;
  dirty = true;
}

void ValueWidget::set(int_fast32_t _value, int_fast32_t _decade) {
  value = _value;
  decade = _decade;
  increment = 1;
  for (int i = 0; i < decade; i++) {
    increment *= 10;
  }
  dirty = true;
}

void ValueWidget::updateValue(int delta) {

  if (delta) {

    dirty = true;

    value = value + (increment * delta);

    if (value > maximum) {
      value = minimum;
    }

    if (value < minimum) {
      value = maximum;
    }
  }
}

void ValueWidget::updateIncrement(int delta) {

  if (delta) {
    
    dirty = true;

    decade += delta;

    if (decade > maxDecade) {
      decade = minDecade;
    }

    if (decade < minDecade) {
      decade = maxDecade;
    }

    increment = 1;
    for (int i = 0; i < decade; i++) {
      increment *= 10;
    }
  }
}

int_fast32_t ValueWidget::getValue() {
    return value;
}