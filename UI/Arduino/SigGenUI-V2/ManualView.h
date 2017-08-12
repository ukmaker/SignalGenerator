#ifndef MANUALVIEW_H
#define MANUALVIEW_H

#include "View.h"
#include "FrequencyWidget.h"
#include "ValueWidget.h"
#include "Joystick.h"

/**
 * ManualView presents widgets to control
 * Frequency
 * Offset
 * Amplitude
 * Duty cycle
 * 
 * The selection control chooses which of these widgets is bound to the joystick
 * 
 */

class ManualView : public View {

  public:

  ManualView(Joystick& joystick, ValueWidget& fWidget, ValueWidget& level);


  void draw(Display& display)  override;

  void setSelection(int selection)  override;
  
  boolean isDirty() override;
  
  void updateDDS() override;

protected:

    Joystick& joystick;
    ValueWidget& fManual;
    ValueWidget& level;

  void handleFrequencyClick();
  void updateFrequencyIncrement(int events);
  void updateFrequency(int events);
  
  void handleValueClick();
  void updateValueIncrement(int events);
  void updateValue(int events);
  
}
;

#endif