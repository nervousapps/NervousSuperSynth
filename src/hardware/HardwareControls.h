#ifndef HardwareControls_h
#define HardwareControls_h

#include "Arduino.h"
// include the ResponsiveAnalogRead library for analog smoothing
#include <ResponsiveAnalogRead.h>
// include the Bounce library for 'de-bouncing' switches -- removing electrical chatter as contacts settle
#include <Bounce.h>
// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "pins.h"

// ******ANALOG CONSTANT VALUES********
const int SLIDERS_PINS = 10; // number of Analog sliders PINS
const int POTARDS_PINS = 5; // number of Analog potentiometers PINS
const int SWITCH_PINS = 3; // number of Analog switches PINS

const int SLIDERS[SLIDERS_PINS] = {
  SLIDE1,
  SLIDE2,
  SLIDE3,
  SLIDE4,
  SLIDE5,
  SLIDE6,
  SLIDE7,
  SLIDE8,
  SLIDE9,
  SLIDE10
};

const int POTARDS[POTARDS_PINS] = {
  POT1,
  POT2,
  POT3,
  POT4,
  POT5
};

const int SWITCHES[SWITCH_PINS] = {
  SW1,
  SW2,
  SW3
};

// ******DIGITAL CONSTANT VALUES********
const int TRIGGER_PINS = 6; // number of Digital trigger PINS
const int ENC_BUTTONS_PINS = 2; // number of Digital encoders switch PINS
const int BOUNCE_TIME = 5; // 5 ms is usually sufficient
const boolean toggled = true;

// define the pins and notes for digital events
const int TRIGGERS[TRIGGER_PINS] = {
  TRIG1,
  TRIG2,
  TRIG3,
  TRIG4,
  TRIG5,
  TRIG6
};

const int ENC_BUTTONS[ENC_BUTTONS_PINS] = {
  ENC1_SW,
  ENC2_SW
};


//******VARIABLES***********

//************INITIALIZE LIBRARY OBJECTS**************
// not sure if there is a better way... some way run a setup loop on global array??
// use comment tags to comment out unused portions of array definitions

// initialize the ReponsiveAnalogRead objects
ResponsiveAnalogRead analog_slide[]{
  {SLIDERS[0],true},
  {SLIDERS[1],true},
  {SLIDERS[2],true},
  {SLIDERS[3],true},
  {SLIDERS[4],true},
  {SLIDERS[5],true},
  {SLIDERS[6],true},
  {SLIDERS[7],true},
  {SLIDERS[8],true},
  {SLIDERS[9],true}
};

ResponsiveAnalogRead analog_pot[]{
  {POTARDS[0],true},
  {POTARDS[1],true},
  {POTARDS[2],true},
  {POTARDS[3],true},
  {POTARDS[4],true}
};

ResponsiveAnalogRead analog_sw[]{
  {SWITCHES[0],true},
  {SWITCHES[1],true},
  {SWITCHES[2],true}
};

// initialize the bounce objects
Bounce digital_trig[] =   {
  Bounce(TRIGGERS[0],BOUNCE_TIME),
  Bounce(TRIGGERS[1], BOUNCE_TIME),
  Bounce(TRIGGERS[2], BOUNCE_TIME),
  Bounce(TRIGGERS[3], BOUNCE_TIME),
  Bounce(TRIGGERS[4], BOUNCE_TIME),
  Bounce(TRIGGERS[5], BOUNCE_TIME)
};

Bounce digital_encsw[] =   {
  Bounce(ENC_BUTTONS[0],BOUNCE_TIME),
  Bounce(ENC_BUTTONS[1], BOUNCE_TIME)
};

Encoder knobRight1(ENC1_1, ENC1_2);
Encoder knobRight2(ENC2_1, ENC2_2);
long positionRight1 = -999;
long positionRight2 = -999;

void setup_hardware_controls(){
  // loop to configure input pins and internal pullup resisters for digital section
  for (int i=0;i<TRIGGER_PINS;i++){
    pinMode(TRIGGERS[i], INPUT_PULLUP);
  }
  for (int j=0;j<ENC_BUTTONS_PINS;j++){
    pinMode(ENC_BUTTONS[j], INPUT_PULLUP);
  }
};

#endif
