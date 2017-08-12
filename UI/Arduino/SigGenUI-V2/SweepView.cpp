#include "SweepView.h"
#include "Axis.h"
#include "EventHandlers.h"

SweepView::SweepView(Joystick& _joystick, ValueWidget& _start, ValueWidget& _end) : 
  joystick(_joystick), fStart(_start), fEnd(_end) {
}


void SweepView::draw(Display& display) {
    
    View::draw(display);
    
  display.u8g2.drawStr(0, 0,        "Sweep         Mode");
  display.nextLine();
  
  fStart.draw(display);
  fEnd.draw(display);
  
  display.u8g2.drawStr(0, display.ypos, "       1.23V  Ampl");
  display.nextLine();

  display.u8g2.drawStr(0, display.ypos, "      -0.40V  Offs");
  display.nextLine();

  display.u8g2.drawStr(0, display.ypos, "         20%  Duty");
  display.nextLine();
}

void SweepView::setSelection(int selection) {
    
    View::setSelection(selection);
    
  if(!active) {
    joystick.x.clearEventHandler();
    joystick.y.clearEventHandler();
    joystick.clearClickEventHandler();
  } else {
      switch(selection) {
          case 2:
          case 3:
            joystick.x.setEventHandler( this, &SweepView::updateIncrement );
            joystick.y.setEventHandler( this, &SweepView::updateFrequency );
            joystick.setClickEventHandler( this, &SweepView::handleFrequencyClick );
            break;
          default:
              break;
      }
  }
}

void SweepView::handleFrequencyClick() {
  selectedFrequencyWidget().setToDecade();
}


void SweepView::updateFrequency(int events) {
  EventHandlers::updateValue(selectedFrequencyWidget(), events);
}

void SweepView::updateIncrement(int events) {
  EventHandlers::updateValueIncrement(selectedFrequencyWidget(), events);
}

boolean SweepView::isDirty() {
    return dirty || fStart.isDirty() || fEnd.isDirty();
}

ValueWidget& SweepView::selectedFrequencyWidget() {
    return (selection == 2) ? fStart : fEnd;
}

void SweepView::updateDDS() {
}