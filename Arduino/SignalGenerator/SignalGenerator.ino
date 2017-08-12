
#include <mcp4728.h>
#include <EEPROM.h>

// Pins to control the DDS module
#define SCK 13      // connect to AD9850 module word load clock pin (CLK)
#define WR_DDS 15      // connect to freq update pin (FQ)
#define MOSI 11       // connect to serial data load pin (MOSI)
#define RST_DDS 14  // connect to reset pin (RST) 

// Pins to control the DAC
#define SDA A5
#define SCL A4
#define SEL_DAC_N 10

// Other control pins
#define VI_PEAK A7
#define AIN6 A6
#define VO_RANGE A3
#define VO_MOD_SEL A2

#define F1MHZ 1000000

// DAC channels
#define VO_AMP 0
#define VO_REF0 1
#define VO_REF1 2
#define VO_DUTY 3

// Application state

int stat = 0;

#define MODE_MANUAL 0
#define MODE_SWEEP 2
#define MODE_SET_FREQUENCY 3
#define MODE_SET_LOW 4
#define MODE_SET_HIGH 5
#define MODE_SET_INCREMENT 6
#define MODE_SET_SWEEP_TYPE 7
#define MODE_SET_GAIN 8


#define SWEEP_TYPE_LINEAR 0
#define SWEEP_TYPE_LOG 1

int_fast32_t sweepLow = 1;
int_fast32_t sweepHigh = 1;
int_fast32_t sweepIncrement = 1;
int_fast32_t sweepStep = 1;
int_fast32_t sweepDecade = 1;
int mode = MODE_MANUAL;
int sweepType = SWEEP_TYPE_LOG;

unsigned long lastMillis = 0;

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

int_fast32_t frequency = 7000000; // Starting frequency of VFO
int_fast32_t frequency2 = 1; // variable to hold the updated frequency
int_fast32_t increment = 0; // starting VFO update increment in HZ.

int_fast32_t sf = 0;


#define MAX_FREQUENCY 45000000
#define MIN_FREQUENCY 2

byte ones, tens, hundreds, thousands, tenthousands, hundredthousands, millions = 0; //Placeholders

String incrementUnits;

int_fast32_t timepassed = millis(); // int to hold the arduino miilis since startup
int memstatus = 1;  // value to notify if memory is current or old. 0=old, 1=current.
int ForceFreq = 0;  // Change this to 0 after you upload and run a working sketch to activate the EEPROM memory.  YOU MUST PUT THIS BACK TO 0 AND UPLOAD THE SKETCH AGAIN AFTER STARTING FREQUENCY IS SET!

boolean out = true;


char buff[200];

int calTableIdx = 0;
int lastCalTableIdx = 0;


/**
   Addresses in EEPROM where various values are stored
*/
#define FREQUENCY_ADDR 0
#define INCREMENT_ADDR (FREQUENCY_ADDR + sizeof(frequency))
#define CALIBRATED_ADDR (INCREMENT_ADDR + sizeof(increment))
#define VS_CAL_ADDR (CALIBRATED_ADDR + sizeof(calibrated))
#define VC_CAL_ADDR (VS_CAL_ADDR + sizeof(vsCal))

void setup() {

  Serial.begin(38400);

  setupDDS();
  setupDAC();
  setupControlPins();

  if (!ForceFreq) {
    EEPROM.get(FREQUENCY_ADDR, frequency);
  }

  EEPROM.get(INCREMENT_ADDR, increment);


  setIncrementUnits();
  setFrequency(frequency);
  updateFrequency();

}

void setupControlPins() {
  pinMode(VO_RANGE, OUTPUT);
  digitalWrite(VO_RANGE, 0);

  pinMode(VO_MOD_SEL, OUTPUT);
  digitalWrite(VO_MOD_SEL, 0);
}


void setupDDS() {
  pinMode(WR_DDS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(RST_DDS, OUTPUT);
  pulseHigh(RST_DDS);
  pulseHigh(SCK);
  pulseHigh(WR_DDS);  // Enable serial mode. Shouldn't be necessary since the pins are tied.

}

void setupDAC() {
  dac.begin();  // initialize i2c interface
  dac.vdd(5000); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  dac.setVref(1, 1, 1, 1); // set to use internal voltage reference (2.048V)
  dac.setGain(1, 1, 1, 1); // set to use internal voltage reference (4.096V)
  dac.voutWrite(2500, 2500, 2500, 2500);
  // Set up a 1V offset between the VGAs for sequential gain
  dac.voutWrite(VO_REF0, 2000);
  dac.voutWrite(VO_REF1, 3000);
}


// frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void setFrequency(double frequency) {

  // note 125 MHz clock on 9850.  You can make 'slight' tuning variations here by adjusting the clock frequency.
  int32_t freq = frequency * 4294967295 / 125000000;

  for (int b = 0; b < 4; b++, freq >>= 8) {
    tfr_byte(freq & 0xFF);
  }

  tfr_byte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(WR_DDS);  // Done!  Should see output
}

// transfers a byte, a bit at a time, LSB first to the 9850 via serial MOSI line
void tfr_byte(byte data) {

  for (int i = 0; i < 8; i++, data >>= 1) {
    digitalWrite(MOSI, data & 0x01);
    pulseHigh(SCK);   //after each bit sent, CLK is pulsed high
  }
}

void nextIncrement() {

  increment = increment * 10L;

  if (increment > 10000000L) {
    increment = 1L;
  }
}

void setIncrementUnits() {

  if (increment == 10L) {
    incrementUnits = " 10 Hz";
  } else if (increment == 100L) {
    incrementUnits = "100 Hz";
  } else if (increment == 1000L) {
    incrementUnits = "  1 KHz";
  } else if (increment == 10000L) {
    incrementUnits = " 10 KHz";
  } else if (increment == 100000L) {
    incrementUnits = "100 KHz";
  } else if (increment == 1000000L) {
    incrementUnits = "  1 MHz";
  } else {
    incrementUnits = "  1 Hz";
  }
}

void updateFrequency() {

  millions = int(frequency / 1000000);
  hundredthousands = ((frequency / 100000) % 10);
  tenthousands = ((frequency / 10000) % 10);
  thousands = ((frequency / 1000) % 10);
  hundreds = ((frequency / 100) % 10);
  tens = ((frequency / 10) % 10);
  ones = ((frequency / 1) % 10);

  timepassed = millis();
  memstatus = 0; // Trigger memory write
}

void loop() {

  handleSerial();
  handleSweep();

  if (frequency != frequency2) {
    setFrequency(frequency);
    frequency2 = frequency;
  }
}


void handleSerial() {
  while(Serial.available()) {
    char c = Serial.read();
    switch (mode) {

      case MODE_MANUAL:

        if (c == 'F') {
          mode = MODE_SET_FREQUENCY;
          sf = 0;
          break;
        }

        if (c == 'L') {
          mode = MODE_SET_LOW;
          sf = 0;
          break;
        }

        if (c == 'H') {
          mode = MODE_SET_HIGH;
          sf = 0;
          break;
        }

        if (c == 'I') {
          mode = MODE_SET_INCREMENT;
          sf = 0;
          break;
        }

        if (c == 'T') {
          mode = MODE_SET_SWEEP_TYPE;
          break;
        }

        if (c == 'G') {
          mode = MODE_SET_GAIN;
          sf = 0;
          break;
        }

        break;

      case MODE_SET_FREQUENCY:
        if (lastDigit(c)) frequency = sf;
        break;

      case MODE_SET_LOW:
        if (lastDigit(c)) sweepLow = sf;
        break;

      case MODE_SET_HIGH:
        if (lastDigit(c)) sweepHigh = sf;
        break;

      case MODE_SET_INCREMENT:
        if (lastDigit(c)) {
          sweepIncrement = sf;
          sweepStep = sweepIncrement;
        }
        break;

      case MODE_SET_SWEEP_TYPE:
        if (c == 'L') {
          sweepType = SWEEP_TYPE_LINEAR;
        } else {
          sweepType = SWEEP_TYPE_LOG;
          sweepDecade = 1;
        }
        mode = MODE_SWEEP;

        frequency = sweepLow;
        Serial.println("Sweep");
        Serial.println(sweepLow);
        Serial.println(sweepHigh);
        Serial.println(sweepIncrement);
        Serial.println(sweepType);
        break;

      case MODE_SWEEP:
        if (c != '\r') mode = MODE_MANUAL;
        break;

      case MODE_SET_GAIN:
       if(lastDigit(c)) dac.voutWrite(VO_AMP, sf);
       break;

      default:
        mode = MODE_MANUAL;
        break;

    }

  }
}

void handleSweep() {
  if (mode == MODE_SWEEP) {
    delay(500);
    if (sweepType == SWEEP_TYPE_LINEAR) {
      frequency += sweepIncrement;
      if (frequency > sweepHigh) {
        frequency = sweepLow;
      }
    } else {

      frequency += sweepStep;
      if (sweepDecade == 9) {
        sweepDecade = 1;
        sweepStep = sweepStep * 10;
      } else {
        sweepDecade++;
      }

      if(frequency > sweepHigh) {
        frequency = sweepLow;
        sweepStep = sweepIncrement;
        sweepDecade = 1;
      }

    }
    float x = 4096 * (float)(frequency - sweepLow) / (float)(sweepHigh - sweepLow);
    Serial.println(x);
  }
}

bool lastDigit(char c) {
  if (c == '\r') {
    mode = MODE_MANUAL;
    return true; // done with digits
  }
  if (c >= '0' && c <= '9') {
    sf = (sf * 10) + c - '0';
  } else {
    mode = MODE_MANUAL;
  }

  return false;
}

void printStatus() {
  stat++;
  if (stat < 400) return;
  stat = 0;
}

void storeMEM() {
  EEPROM.put(0, frequency);
  EEPROM.put(INCREMENT_ADDR, increment);
  memstatus = 1;  // Let program know memory has been written
};
