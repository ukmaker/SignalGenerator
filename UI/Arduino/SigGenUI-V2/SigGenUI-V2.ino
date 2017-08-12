#include "FunctionPointerArg1.h"

#include <ClickEncoder.h>

#include <TimerOne.h>

#include "Joystick.h"

#include "FrequencyWidget.h"
#include "VoltageWidget.h"
#include "ValueWidget.h"

#include "Display.h"
#include "View.h"
#include "ManualView.h"
#include "SweepView.h"

#include "Controller.h"

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define CHAR_W 6
#define LINE_H 11

Axis x(A3);
Axis y(A4);
OneButton button(A5, true);
Joystick joystick(x, y, button);


U8G2_ST7920_128X64_2_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13 /* A4 */ , /* data=*/ 11 /* A2 */, /* CS=*/ 10 /* A3 */, /* reset=*/ U8X8_PIN_NONE);

Display display(u8g2);

ValueWidget fManual("Hz  Freq", 50000000, 0, 7, 0, 0);
ValueWidget fStart("Hz  Start", 50000000, 0, 7, 0, 0);
ValueWidget fEnd("Hz  End ", 50000000, 0, 7, 0, 0);

ValueWidget level("V   Level", 10000, -10000, 4, 1, 3);

ManualView manualView(joystick, fManual, level);
SweepView sweepView(joystick, fStart, fEnd);
ManualView analyseView(joystick, fManual, level);

ClickEncoder encoder(A1, A2, A0, 4);
  
Controller controller(display, encoder, joystick, manualView, sweepView, analyseView);


void setup(void) {
  
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

  controller.setMode(MODE_MANUAL);
  controller.setSelection(1);
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

void loop(void) {

  controller.update();
  controller.draw();
}





