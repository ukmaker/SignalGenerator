#include "Controller.h"

Controller::Controller(Display &_display, ClickEncoder &_encoder, Joystick &_joystick, View &_view0, View &_view1, View &_view2) :
    
    display(_display), encoder(_encoder), joystick(_joystick),
    view0(_view0), view1(_view1), view2(_view2)

{}

void Controller::update() {
  joystick.update();
  updateSelection();
  updateMode();
}

void Controller::draw() {
     
  if (needRefresh || currentView().isDirty()) {
  currentView().updateDDS();
    needRefresh = false;
    display.u8g2.clear();
    display.u8g2.firstPage();
    do {
        
      currentView().draw(display);

      // Draw the arrow showing what is selected
      display.u8g2.setFont(u8g2_font_7x13_t_symbols);
      display.u8g2.drawGlyph(20 * display.charWidth, 0 + ((selection - 1) * display.lineHeight), 0x25b0 + 6);
      display.u8g2.setFont(u8g2_font_6x10_tf);
      
    } while (display.u8g2.nextPage());
  }
 
}

    
View& Controller::currentView() {
  if(mode == 0) {
    return view0;
  } else if(mode ==1) {
    return view1;
  } else {
    return view2;
  }
}

void Controller::updateMode() {
  
  int m = mode;
  
  ClickEncoder::Button b = encoder.getButton();
  if (b == ClickEncoder::Clicked) {
    switch (selection) {
      case 1:
        m++;
        m = m % 3;
        setMode(m);
        break;

      default:
        break;
    }
  }
}

void Controller::setMode(int m) {
  if(mode != m) {
    
    needRefresh = true;
    currentView().setActive(false);
    mode = m;
    currentView().setActive(true);
  }
}

void Controller::updateSelection() {
  int value = encoder.getValue();
  setSelection(selection + value);
}

void Controller::setSelection(int s) {
    
    if(selection != s) {
        
        needRefresh = true;
        
        selection = s;

        if (selection > SELECTIONS) {
        selection = SELECTIONS;
        }

        if (selection < 1) {
        selection = 1;
        }
        
        currentView().setSelection(selection);
    }
}