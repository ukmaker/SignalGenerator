

#include <ClickEncoder.h>

#include <TimerOne.h>

#include "Joystick.h"

#include "FrequencyWidget.h"

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

FrequencyWidget fManual(u8g2, LINE_H, "Hz  Freq", CHAR_W, LINE_H);
FrequencyWidget fStart(u8g2, LINE_H, "Hz  Start", CHAR_W, LINE_H);
FrequencyWidget fEnd(u8g2, LINE_H * 2, "Hz  End ", CHAR_W, LINE_H);

ClickEncoder *encoder;

int16_t lastSelectionEnc = 0;
uint8_t selection = 1;
#define SELECTIONS 6

boolean needRefresh = true;

int mode = 0;

#define MODE_MANUAL 0
#define MODE_SWEEP 1
#define MODE_ANALYSE 2

#define MENU_MANUAL 0
#define MENU_SWEEP 1
#define MENU_SWEEP_PARAMS 2


void setup(void) {
  joystick.y.repeat = 200;
  joystick.y.fastRepeat = 100;
  joystick.y.shortHold = 2000;

  button.attachClick(joystickClick);

  u8g2.begin();
  u8g2_prepare();

  encoder = new ClickEncoder(A1, A2, A0, 4);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  Serial.begin(38400);
  Serial.println("F10000");
}

void timerIsr() {
  encoder->service();
  button.tick();
}

void joystickClick() {
  // set the frequency to the value of the underlined digit
  currentF().setToDecade();
}

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  //u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void draw() {

  switch(mode) {
    case 1: drawSweep(); break;
    case 2: drawAnalyse() ; break;
    default: drawManual();
  }


  draw_selection();

}

void drawManual(){
  u8g2.drawStr(0, 0,        "Manual        Mode");
  currentF().draw();
  u8g2.drawStr(0, LINE_H * 2, "       1.23V  Ampl");
  u8g2.drawStr(0, LINE_H * 3, "      -0.40V  Offs");
  u8g2.drawStr(0, LINE_H * 4, "         20%  Duty");
  u8g2.drawStr(0, LINE_H * 5, "       0.00V  Vinp");

}

void drawSweep() {
  u8g2.drawStr(0, 0,        "Sweep         Mode");
  fStart.draw();
  fEnd.draw();
  u8g2.drawStr(0, LINE_H * 3, "       1.23V  Ampl");
  u8g2.drawStr(0, LINE_H * 4, "      -0.40V  Offs");
  u8g2.drawStr(0, LINE_H * 5, "         20%  Duty");
}

void drawAnalyse() {
  u8g2.drawStr(0, 0,        "Analyse       Mode");
}

void loop(void) {

  readJoystick();
  updateSelection();
  updateMode();

  if (needRefresh || currentF().isDirty()) {
    needRefresh = false;
    u8g2.clear();
    u8g2.firstPage();
    do {
      draw();
    } while (u8g2.nextPage());
  }
}

void updateMode() {
  
  int m = mode;
  
  ClickEncoder::Button b = encoder->getButton();
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

void setMode(int m) {
  if(mode != m) {
    
    needRefresh = true;

      switch(m) {
        case MODE_MANUAL: return setModeManual();
        case MODE_SWEEP: return setModeSweep();
        case MODE_ANALYSE: return setModeAnalyse();
        default:
        return setModeManual();
      }
  }
}

void setModeManual() {
  mode = MODE_MANUAL;
  // Setup callback functions on the joystick
  
}

void setModeSweep() {
  
}

void setModeAnalyse() {
  
}

void updateSelection() {
  int value = encoder->getValue();
  if (value != 0) {

    needRefresh = true;

    selection += value;

    if (selection > SELECTIONS) {
      selection = SELECTIONS;
    }

    if (selection < 1) {
      selection = 1;
    }
  }
}

void readJoystick() {
  int events = joystick.y.read();
  int deltaF = 0;
  int deltaI = 0;

  if (events & JOY_INC) {
    deltaF = 1;
  }

  if (events & JOY_DEC) {
    deltaF = -1;
  }

  if (events & JOY_FAST) {
    deltaF *= 5;
  }

  events = joystick.x.read();
  if (events & JOY_INC) {
    deltaI = 1;
  }

  if (events & JOY_DEC) {
    deltaI = -1;
  }

  currentF().update(deltaF, deltaI);

}

void draw_selection() {
  u8g2.setFont(u8g2_font_7x13_t_symbols);
  u8g2.drawGlyph(20 * CHAR_W, 0 + ((selection - 1) * LINE_H), 0x25b0 + 6);
  u8g2.setFont(u8g2_font_6x10_tf);
}


