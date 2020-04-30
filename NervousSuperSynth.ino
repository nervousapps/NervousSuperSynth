#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <Audio.h>
#include "hardware/pins.h"
#include "display/Display.h"
#include "hardware/HardwareControls.h"

AudioMixer4                 mainMix;
AudioOutputAnalogStereo     DACS1;
AudioConnection             mainpatchcord(mainMix, 0, DACS1, 0);

#include "sampleplayer/SamplePlayer.h"
#include "kelpie/kelpiemaster.h"
#include "chordOrgan/ChordOrgan.h"

#define synthNumber 2

int synthSelect = 0;
char synthName[synthNumber][16] = {"Kelpie", "ChordOrgan"};


void setup(){
  // Configure the ADCs
  analogReadResolution(7);
  analogReadAveraging(4);
  analogReference(EXTERNAL);

  // Configure the DACs
  analogWriteResolution(16);
  DACS1.analogReference(INTERNAL);
  AudioMemory(800);

  //Serial.println("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    //Serial.println("initialization failed!");
    return;
  }

  Serial.begin(9600);
  Serial.print("Begin");

  setup_lcd();
  setup_progressbar();

  setup_hardware_controls();

  lcd.setCursor(0,0);
  lcd.print("SuperSynth");

  init_banks();

  kelpie_setup();
  setup_chordOrgan();
}

void loop(){
  control_sampleplayer();
  sampleplay();
  selectSynth();
  switch (synthSelect) {
    case 0:
    kelpie_run();
    break;

    case 1:
    chordOrgan_run();
    break;
  }
}

void selectSynth(){
  long newRight1;

  // Get rotary encoder1 value
  newRight1 = knobRight1.read()/2;
  if (newRight1 != positionRight1) {
    if (newRight1 >= synthNumber){
      knobRight1.write(0);
      newRight1 = 0;
    }
    if (newRight1 < 0){
      newRight1 = synthNumber-1;
      knobRight1.write(newRight1*2);
    }
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(synthName[newRight1]);
    positionRight1 = newRight1;
  }
  if(digital_encsw[0].update()){
    if(digital_encsw[0].fallingEdge()){
      AudioNoInterrupts();
      synthSelect = newRight1;
      AudioInterrupts();
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(synthName[synthSelect]);
      lcd.print(" selected !");
      switch (synthSelect) {
        case 0:
        chordOrganenvelope1.noteOff();
        AudioNoInterrupts();
        usbMIDI.setHandleNoteOff(KelpieOnNoteOff);
        usbMIDI.setHandleNoteOn(KelpieOnNoteOn);
        AudioInterrupts();
        break;

        case 1:
        keyBuffPoly(0, 127, false);
        AudioNoInterrupts();
        usbMIDI.setHandleNoteOff(ChordOrganOnNoteOff);
        usbMIDI.setHandleNoteOn(ChordOrganOnNoteOn);
        AudioInterrupts();
        chordOrganenvelope1.noteOn();
        break;
      }
    }
  }
}
