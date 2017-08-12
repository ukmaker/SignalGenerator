#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Arduino.h"

#include <ClickEncoder.h>
#include "Joystick.h"
#include "Display.h"
#include "View.h"

#define MODE_MANUAL 0
#define MODE_SWEEP 1
#define MODE_ANALYSE 2

#define MENU_MANUAL 0
#define MENU_SWEEP 1
#define MENU_SWEEP_PARAMS 2

#define SELECTIONS 6

class Controller {

public:
    
    Controller(Display &display, ClickEncoder &_encoder, Joystick &_joystick, View &_view0, View &_view1, View &_view2);
    
    void update();
    void draw();

    View& currentView();
    
    void updateMode();
    void updateSelection();
    void setMode(int m);
    void setSelection(int s);
    

protected:


Display &display;

Joystick &joystick;
ClickEncoder &encoder;

View &view0;
View &view1;
View &view2;

int mode = -1;
int selection = -1;
boolean needRefresh = true;

};

#endif