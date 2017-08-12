#include "EventHandlers.h"
#include "Joystick.h"

void EventHandlers::updateValue(ValueWidget& widget, int events) {
  int deltaV = 0;

  if (events & JOY_INC) {
    deltaV = 1;
  }

  if (events & JOY_DEC) {
    deltaV = -1;
  }

  if (events & JOY_FAST) {
    deltaV *= 5;
  }

  widget.updateValue(deltaV);
}

void EventHandlers::updateValueIncrement(ValueWidget& widget, int events) {
  int deltaI = 0;

  if (events & JOY_INC) {
    deltaI = 1;
  }

  if (events & JOY_DEC) {
    deltaI = -1;
  }

  widget.updateIncrement(deltaI);

}
