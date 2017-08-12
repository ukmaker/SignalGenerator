#include "Arduino.h"
#include <U8g2lib.h>

#define MAX_FREQUENCY_DECADE 7

class FrequencyWidget {

  public:
  
  FrequencyWidget(U8G2& u8g2, int ypos, char *label, int char_w, int line_h);

  void update(int deltaF, int deltaIncrement);
  
  void draw();

  boolean isDirty();

  void setToDecade();

  void set(int_fast32_t frequency, int_fast32_t decade);

  protected:

  U8G2& u8g2;

  int ypos;
  int_fast32_t frequency;
  int_fast32_t increment;
  int decade;
  char *label;
  boolean dirty;
  int char_w;
  int line_h;

  void _updateFrequency(int delta);
  void _updateIncrement(int delta);
  void _draw();
};

