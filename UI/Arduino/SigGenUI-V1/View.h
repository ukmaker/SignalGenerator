#include "Arduino.h"

/**
 * Class to represent a view
 * 
 */
 class View {

  public:

  View();


  virtual void setActive(boolean active);

  virtual void draw();

  protected:

  boolean active;

  
 };

