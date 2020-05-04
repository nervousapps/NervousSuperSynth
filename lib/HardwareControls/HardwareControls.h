/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Hardware_Controls_h
#define Hardware_Controls_h

#include "Arduino.h"
// include the ResponsiveAnalogRead library for analog smoothing
#include <ResponsiveAnalogRead.h>
// include the Bounce library for 'de-bouncing' switches -- removing electrical chatter as contacts settle
#include <Bounce.h>
// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <HardwarePins.h>


class HardwareControls
{
  public:
    HardwareControls() { };
    void setup_hardware_controls();

    // ******ANALOG CONSTANT VALUES********
    static const int SLIDERS_PINS = 10; // number of Analog sliders PINS
    static const int POTARDS_PINS = 5; // number of Analog potentiometers PINS
    static const int SWITCH_PINS = 3; // number of Analog switches PINS

    const int SLIDERS[HardwareControls::SLIDERS_PINS] = {
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

    const int POTARDS[HardwareControls::POTARDS_PINS] = {
      POT1,
      POT2,
      POT3,
      POT4,
      POT5
    };

    const int SWITCHES[HardwareControls::SWITCH_PINS] = {
      SW1,
      SW2,
      SW3
    };

    // ******DIGITAL CONSTANT VALUES********
    static const int TRIGGER_PINS = 6; // number of Digital trigger PINS
    static const int ENC_BUTTONS_PINS = 2; // number of Digital encoders switch PINS
    static const int BOUNCE_TIME = 5; // 5 ms is usually sufficient
    static const boolean toggled = true;

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
    // a data array and a lagged copy to tell when MIDI changes are required
    static byte data[SLIDERS_PINS];
    static byte dataLag[SLIDERS_PINS]; // when lag and new are not the same then update MIDI CC value

    //************INITIALIZE LIBRARY OBJECTS**************
    // not sure if there is a better way... some way run a setup loop on global array??
    // use comment tags to comment out unused portions of array definitions

    // initialize the ReponsiveAnalogRead objects
    ResponsiveAnalogRead analog_slide[HardwareControls::SLIDERS_PINS] = {
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

    ResponsiveAnalogRead analog_pot[HardwareControls::POTARDS_PINS] = {
      {POTARDS[0],true},
      {POTARDS[1],true},
      {POTARDS[2],true},
      {POTARDS[3],true},
      {POTARDS[4],true}
    };

    ResponsiveAnalogRead analog_sw[HardwareControls::SWITCH_PINS] = {
      {SWITCHES[0],true},
      {SWITCHES[1],true},
      {SWITCHES[2],true}
    };

    // initialize the bounce objects
    Bounce digital_trig[HardwareControls::TRIGGER_PINS] =   {
      Bounce(TRIGGERS[0],BOUNCE_TIME),
      Bounce(TRIGGERS[1], BOUNCE_TIME),
      Bounce(TRIGGERS[2], BOUNCE_TIME),
      Bounce(TRIGGERS[3], BOUNCE_TIME),
      Bounce(TRIGGERS[4], BOUNCE_TIME),
      Bounce(TRIGGERS[5], BOUNCE_TIME)
    };

    Bounce digital_encsw[HardwareControls::ENC_BUTTONS_PINS] =   {
      Bounce(ENC_BUTTONS[0],BOUNCE_TIME),
      Bounce(ENC_BUTTONS[1], BOUNCE_TIME)
    };

    Encoder knobRight1 = Encoder(ENC1_1, ENC1_2);
    Encoder knobRight2 = Encoder(ENC2_1, ENC2_2);
    long positionRight1 = -999;
    long positionRight2 = -999;

  private:
};

#endif
