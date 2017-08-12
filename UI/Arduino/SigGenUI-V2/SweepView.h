#ifndef SWEEPVIEW_H
#define SWEEPVIEW_H

#include "View.h"
#include "ValueWidget.h"
#include "Joystick.h"

/**
 * SweepView presents widgets to control
 * Frequency
 * Offset
 * Amplitude
 * Duty cycle
 * 
 * The selection control chooses which of these widgets is bound to the joystick
 * 
 */

class SweepView : public View {

  public:

  SweepView(Joystick& joystick, ValueWidget& fStart, ValueWidget& fEnd);

  void draw(Display& display)  override;

  void setSelection(int selection)  override;
  
  void handleFrequencyClick();
  
  boolean isDirty() override;

    void updateDDS() override;

protected:
    
    Joystick& joystick;
  ValueWidget& fStart;
  ValueWidget& fEnd;


  void updateIncrement(int events);
  void updateFrequency(int events);
  
  ValueWidget& selectedFrequencyWidget();
  
}
;

#endif