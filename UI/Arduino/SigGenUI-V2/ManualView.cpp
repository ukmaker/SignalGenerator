#include "ManualView.h"
#include "Axis.h"
#include "EventHandlers.h"

ManualView::ManualView(Joystick& _joystick, ValueWidget& widget, ValueWidget& _level) : View(), joystick(_joystick), fManual(widget), level(_level) {
}

void ManualView::draw(Display& display) {
    
    View::draw(display);
  display.u8g2.drawStr(0, 0,        "Manual        Mode");
  display.nextLine();
  fManual.draw(display);
  level.draw(display);
  //display.u8g2.drawStr(0, display.lineHeight * 2, "       1.23V  Ampl");
  display.u8g2.drawStr(0, display.lineHeight * 3, "      -0.40V  Offs");
  display.u8g2.drawStr(0, display.lineHeight * 4, "         20%  Duty");
  display.u8g2.drawStr(0, display.lineHeight * 5, "       0.00V  Vinp");
}

void ManualView::setSelection(int selection) {
    
    View::setSelection(selection);
    
  if(!active) {
    joystick.x.clearEventHandler();
    joystick.y.clearEventHandler();
    joystick.clearClickEventHandler();
  } else {
      
      switch(selection) {
          case 2:
            joystick.x.setEventHandler( this, &ManualView::updateFrequencyIncrement );
            joystick.y.setEventHandler( this, &ManualView::updateFrequency );
            joystick.setClickEventHandler( this, &ManualView::handleFrequencyClick );
            break;
            
          case 3:
            joystick.x.setEventHandler( this, &ManualView::updateValueIncrement );
            joystick.y.setEventHandler( this, &ManualView::updateValue );
            joystick.setClickEventHandler( this, &ManualView::handleValueClick );
            break;
            
          default:
              break;
              
      }
  }
}

void ManualView::handleFrequencyClick() {
  fManual.setToDecade();
}

void ManualView::updateFrequency(int events) {
  EventHandlers::updateValue(fManual, events);
}

void ManualView::updateFrequencyIncrement(int events) {
  EventHandlers::updateValueIncrement(fManual, events);
}


void ManualView::handleValueClick() {
  level.setToDecade();
}

void ManualView::updateValue(int events) {
  EventHandlers::updateValue(level, events);
}

void ManualView::updateValueIncrement(int events) {
  EventHandlers::updateValueIncrement(level, events);
}

boolean ManualView::isDirty() {
    return dirty || fManual.isDirty() || level.isDirty();
}

void ManualView::updateDDS() {
    Serial.print("F");
    Serial.println(fManual.getValue());
    Serial.print("G");
    Serial.println(level.getValue());
}
