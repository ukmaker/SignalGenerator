
#include "UILib.h"


#define CHAR_W 6
#define LINE_H 10

Axis x(A3);
Axis y(A4);
OneButton button(A5, true);
Joystick joystick(x, y, button);
ClickEncoder encoder(A1, A2, A0, 4);


U8G2_ST7920_128X64_2_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13 /* A4 */ , /* data=*/ 11 /* A2 */, /* CS=*/ 10 /* A3 */, /* reset=*/ U8X8_PIN_NONE);

Display display(u8g2);

/**
* Declare all the widgets used by the application in one place
* Views can share widgets
**/
// Manual frequency
ValueWidget fManual("Hz  Freq", 50000000, 0, 7, 0, 0);

// Sweep settings
ValueWidget fStart("Hz  Start", 50000000, 0, 7, 0, 0);
ValueWidget fEnd("Hz  End ", 50000000, 0, 7, 0, 0);
ValueWidget fStep("Hz  Step ", 50000000, 0, 7, 0, 0);

class MenuItem {
public:
    MenuItem(char* t) : text(t) {}
    char *text;
};

MenuItem linear("LINEAR");
MenuItem logarithmic("LOG");
Ring<MenuItem,2> sweepRing;
RingWidget<MenuItem, 2> sweepTypeMenu(sweepRing);


// Signal level
ValueWidget level("V   Level", 10000, -10000, 4, 1, 3);
ValueWidget offset("V  Offset", 10000, -10000, 4, 1, 3);

LineView manualView("Manual");
LineView sweepSettingsView("Set Sweep");

Ring<View, 8> viewRing;
RingWidget<View, 8> viewWidget(viewRing);

Platform<8> platform(display, joystick, encoder, viewRing);

void setup() {
    // put your setup code here, to run once:
    sweepRing.add(&linear);
    sweepRing.add(&logarithmic);
    sweepTypeMenu.onChange(changeSweepType);
    sweepTypeMenu.setText(linear.text);
    
    // Every view starts with the menu
    manualView.append(&viewWidget);
    manualView.append(&fManual);
    manualView.append(&level);
    manualView.append(&offset);
    
    sweepSettingsView.append(&viewWidget);
    sweepSettingsView.append(&fStart);
    sweepSettingsView.append(&fEnd);
    sweepSettingsView.append(&sweepTypeMenu);
    sweepSettingsView.append(&fStep);
    
    viewRing.add(&manualView);
    viewRing.add(&sweepSettingsView);
    
    viewWidget.onChange(changeView);
    viewWidget.setText(manualView.title());

    
      
  display.lineHeight = LINE_H;
  display.charWidth = CHAR_W;
  
  joystick.y.repeat = 200;
  joystick.y.fastRepeat = 100;
  joystick.y.shortHold = 2000;

  button.attachClick(joystickClick);

  u8g2.begin();
  u8g2_prepare();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  Serial.begin(38400);
  Serial.println("F10000");


}

void changeView() {
    viewWidget.setText(viewRing.current()->title());
}

void changeSweepType() {
    sweepTypeMenu.setText(sweepRing.current()->text);
    fStep.setActive(!fStep.isActive());
}
    

void timerIsr() {
  encoder.service();
  button.tick();
}

void joystickClick() {
  joystick.handleClick();
}

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  //u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}


void loop() {
  // put your main code here, to run repeatedly:

  platform.process();

 }
