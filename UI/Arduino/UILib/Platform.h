#ifndef PLATFORM_H
#define PLATFORM_H

#include "Arduino.h"
#include <ClickEncoder.h>

#include <TimerOne.h>
#include "Ring.h"
#include "View.h"

/**
* Defines a singleton which represents the hardware
* platform for a particular application.
* The platform comprises the display, joystick and rotary encoder
*
* Input device events are routed to the appropriate views
* Applications must define the constant MENU_SIZE to set the maximum number
* of views managed by the platform
**/
template <uint8_t S>
class Platform : public Widget {

public:

  Platform(Display& display, Joystick &joystick, ClickEncoder &encoder, Ring<View, S>& views) :
    _display(display),
    _joystick(joystick), _encoder(encoder),
    _views(views), _grabbed(false), _dirty(false)
    {

      _joystick.x.setEventHandler(this, &Platform::handleJoystickXMove);
      _joystick.y.setEventHandler(this, &Platform::handleJoystickYMove);
      _joystick.setClickHandler(this, &Platform::handleJoystickClick);
    }

    /**
    * Joystick events are routed to the active widget
    **/
    void handleJoystickXMove(int event) {
      selectedWidget()->handleJoystickXMove(event);
    }

    void handleJoystickYMove(int event) {
      selectedWidget()->handleJoystickYMove(event);
    }

    void handleJoystickClick() {
      selectedWidget()->handleJoystickClick();
    }

    void handleMenuChange() {
      // nothing to do for now
    }

    /**
    * A click on the encoder button transfers focus to/from the active Widget
    * When a widget has focus, it receives encoder scroll events
    * When the platform has focus, encoder scroll events are used to change
    * the selection
    **/
    void handleEncoderClick() {
      _grabbed = !_grabbed;
      _dirty = true;
    }

    void handleEncoderScroll(int steps) {
    
        int selection = _views.current()->selection();

        if(_grabbed) {
        selectedWidget()->handleEncoderScroll(steps);
      } else {
        setSelection(selection + steps);
      }
    }

    /**
    * If the current selection is zero, then the active Widget is the
    * Platform's MenunWidget
    * Otherwise it is the selected widget of the currentView
    **/
    Widget* selectedWidget() {

        return _views.current()->selectedWidget();

    }

    void setSelection(int selection) {
      _views.current()->setSelection(selection);
    }

    void process() {
      // pump events
      _processJoystick();
      _processEncoder();

      if( _dirty || _views.current()->isDirty()) {
          _dirty = false;
        _draw(_display);
      }
    }



  protected:

    Display& _display;
    Joystick& _joystick;
    ClickEncoder& _encoder;
    Ring<View, S>& _views;

    bool _grabbed;
    bool _dirty;

    void _processJoystick() {
      _joystick.update();
    }

    void _processEncoder() {
        ClickEncoder::Button b = _encoder.getButton();
        if (b == ClickEncoder::Clicked) {
          handleEncoderClick();
        }

        int value = _encoder.getValue();
        if(value != 0) {
          handleEncoderScroll(value);
        }
    }

    virtual void _draw(Display& _display) {
        
        int selection = _views.current()->selection();
        
      
      _display.u8g2.clear();
      _display.u8g2.firstPage();
     
      do {

        _display.reset();

        _views.current()->draw(_display);

        // Draw the arrow showing what is selected
        _display.u8g2.setFont(u8g2_font_7x13_t_symbols);
        if(_grabbed) {
            _display.u8g2.drawGlyph(20 * _display.charWidth, 0 + ((selection) * _display.lineHeight), 0x25c0);
        } else {
            _display.u8g2.drawGlyph(20 * _display.charWidth, 0 + ((selection) * _display.lineHeight), 0x25b0 + 6);
        }
        _display.u8g2.setFont(u8g2_font_6x10_tf);

      } while (_display.u8g2.nextPage());
    }
}
;
#endif
