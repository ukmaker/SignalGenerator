#ifndef VOLTAGE_WIDGET_H
#define VOLTAGE_WIDGET_H

#include "Arduino.h"
#include "Display.h"
#include "Widget.h"

#define MAX_VOLTAGE_DECADE 4

class VoltageWidget : public Widget {

  public:
  
  VoltageWidget(char *label, int_fast32_t max, int_fast32_t min, int maxDecade, int minDecade);

  void updateValue(int deltaV);
  void updateIncrement(int deltaIncrement);
  
  void draw(Display& display) override;

  void setToDecade();
  void set(int_fast32_t frequency, int_fast32_t decade);

  protected:
  
  int_fast32_t value, maximum, minimum;
  int_fast32_t increment;
  int decade, maxDecade, minDecade;
  char *label;

    
};

#endif
