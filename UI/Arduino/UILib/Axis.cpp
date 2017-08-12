#include "Arduino.h"
#include "Axis.h"

Axis::Axis(int _input) : input(_input) {

  currentValue = 0;
  lastValue = 0;
  lastChanged = 0;
  events = 0;
  holding = false;
  shortHold = JOY_SHORT_HOLD;
  longHold = JOY_LONG_HOLD;
  repeat = JOY_REPEAT;
  fastRepeat = JOY_FAST_REPEAT;
}

// Read and debounce the axis and Raise an event if needed
void Axis::update() {

  events = 0;
  
  currentValue =  analogRead(input);
  // Map to -1, 0 or 1
  if(currentValue < 450) {
    currentValue = -1;
  } else if(currentValue > 600) {
    currentValue = 1;
  } else {
    currentValue = 0;
  }

  unsigned long now = millis();
  unsigned long delta = now - lastChanged;
  unsigned long ticks;
  
  if(currentValue != lastValue) {
    
    lastChanged = now;

  } else if(delta > JOY_DEBOUNCE) {

    if(currentValue != 0) {
      // We have an event of some sort
      if(delta > longHold) {
        // Emit an event every JOY_REPEAT ms
        ticks = delta - longHold;
        if((ticks % fastRepeat) == 0) {
           events |= JOY_FAST;
           events |= currentValue > 0 ? JOY_INC : JOY_DEC;
        }
      } else if(delta > shortHold) {
        // Emit an event every JOY_REPEAT ms
        ticks = delta - shortHold;
        if((ticks % repeat) == 0) {
           events |= JOY_SLOW;
           events |= currentValue > 0 ? JOY_INC : JOY_DEC;
        }        
      } else {
        if(!holding) {
          events |= currentValue > 0 ? JOY_INC : JOY_DEC;
          holding = true;
        }
      }
      
    } else {
      events = 0;
      holding = false;
    }
  }
  lastValue = currentValue;
  if(events) _fp.call(events);
}
