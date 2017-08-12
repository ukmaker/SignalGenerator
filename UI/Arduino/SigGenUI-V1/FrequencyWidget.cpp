#include "FrequencyWidget.h"
#include <U8g2lib.h>


FrequencyWidget::FrequencyWidget(U8G2& _u8g2, int _ypos, char *_label, int _char_w, int _line_h) :
  u8g2(_u8g2), ypos(_ypos), label(_label), char_w(_char_w), line_h(_line_h) {


  frequency = 1000;
  increment = 1000;
  decade = 3;
  dirty = true;

}


void FrequencyWidget::update(int deltaF, int deltaIncrement) {
  _updateIncrement(deltaF);
  _updateFrequency(deltaIncrement);
}

void FrequencyWidget::draw() {
  _draw();
  dirty = false;
}

boolean FrequencyWidget::isDirty() {
  return dirty;
}

void FrequencyWidget::setToDecade() {
  int_fast32_t f = frequency;
  for (int i = 0; i < decade; i++) {
    f = f / 10;
  }

  f = f % 10;

  for (int i = 0; i < decade; i++) {
    f = f * 10;
  }

  frequency = f;
  dirty = true;
}

void FrequencyWidget::set(int_fast32_t _frequency, int_fast32_t _decade) {
  frequency = _frequency;
  decade = _decade;
  increment = 1;
  for (int i = 0; i < decade; i++) {
    increment *= 10;
  }
  dirty = true;
}

void FrequencyWidget::_updateFrequency(int delta) {

  if (delta) {

    dirty = true;

    frequency = frequency + (increment * delta);

    if (frequency > 49999999) {
      frequency = 1;
    }

    if (frequency < 1) {
      frequency = 49999999;
    }
  }
}

void FrequencyWidget::_updateIncrement(int delta) {

  if (delta) {
    
    dirty = true;

    decade += delta;

    if (decade > MAX_FREQUENCY_DECADE) {
      decade = 0;
    }

    if (decade < 0) {
      decade = MAX_FREQUENCY_DECADE;
    }

    increment = 1;
    for (int i = 0; i < decade; i++) {
      increment *= 10;
    }
  }
}

void FrequencyWidget::_draw() {

  int_fast32_t f = frequency;
  int digit;
  int xpos = char_w * 10;
  int digitDecade = 0;
  boolean printing = true;

  while (printing) {

    digit = f % 10;
    f = f / 10;
    u8g2.setCursor(xpos, ypos);
    u8g2.print(digit);

    if (decade == digitDecade) u8g2.drawHLine(xpos, ypos + line_h - 1, char_w);

    xpos -= char_w;
    digitDecade++;

    printing = f || digitDecade <= decade;

    if (printing && (digitDecade % 3) == 0) {
      u8g2.setCursor(xpos, ypos);
      u8g2.print(",");
      xpos -= char_w;
    }

  }

  u8g2.setCursor(11 * char_w, ypos);
  u8g2.print(label);
}
