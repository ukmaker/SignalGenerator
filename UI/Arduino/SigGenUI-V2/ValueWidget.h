#ifndef VALUE_WIDGET_H
#define VALUE_WIDGET_H

#include "Arduino.h"
#include "Display.h"
#include "Widget.h"

class ValueWidget : public Widget {

  public:
  
  ValueWidget(char *label, int_fast32_t max, int_fast32_t min, int maxDecade, int minDecade, int scale);

  void updateValue(int deltaV);
  void updateIncrement(int deltaIncrement);
  
  void draw(Display& display) override;

  void setToDecade();
  void set(int_fast32_t frequency, int_fast32_t decade);
  int_fast32_t getValue();

  protected:
  
  int_fast32_t value, maximum, minimum;
  int_fast32_t increment;
  int decade, maxDecade, minDecade, scale;
  char *label;

    
};

#endif
