/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "HardwareControls.h"

void HardwareControls::setup_hardware_controls(){
  // loop to configure input pins and internal pullup resisters for digital section
  for (int i=0;i<TRIGGER_PINS;i++){
    pinMode(TRIGGERS[i], INPUT_PULLUP);
  }
  for (int j=0;j<ENC_BUTTONS_PINS;j++){
    pinMode(ENC_BUTTONS[j], INPUT_PULLUP);
  }
}
