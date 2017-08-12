#ifndef EVENTHANDLERS_H
#define  EVENTHANDLERS_H

#include "Arduino.h"
#include "FrequencyWidget.h"
#include "ValueWidget.h"

class EventHandlers {

public:

static void updateValueIncrement(ValueWidget& widget, int events);
static void updateValue(ValueWidget& widget, int events);

  
};

#endif

