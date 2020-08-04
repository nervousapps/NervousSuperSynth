#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <Audio.h>
#include "hardware/pins.h"
#include "display/Display.h"
#include <MIDI.h>
#include "hardware/HardwareControls.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

AudioMixer4                 mainMix;
AudioAmplifier              mainAMP;
AudioOutputAnalogStereo     DACS1;
AudioOutputPT8211           pt8211_1;
AudioConnection             mainpatchcord0(mainMix, 0, mainAMP, 0);
AudioConnection             mainpatchcord2(mainAMP, 0, DACS1, 0);
AudioConnection             mainpatchcord3(mainAMP, 0, DACS1, 1);

boolean synthParam = false;
boolean sampleParam = false;
boolean firstTime = true;

#include "sampleplayer/SamplePlayer.h"
#include "kelpie/kelpiemaster.h"
#include "chordOrgan/ChordOrgan.h"
#include "braids/braids.h"


#define synthNumber 3

int synthSelect = 0;
char synthName[synthNumber][16] = {"Kelpie", "ChordOrgan", "Braids"};

const int chipSelect = BUILTIN_SDCARD;


void selectSynth(){
  if(!synthParam){
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
      lcd.setCursor(0, 0);
      lcd.print(synthName[newRight1]);
      positionRight1 = newRight1;
    }
    if(digital_encsw[0].update()){
      if(digital_encsw[0].fallingEdge()){
        Serial.print("Note ON");
        AudioNoInterrupts();
        synthSelect = newRight1;
        synthParam = !synthParam;
        AudioInterrupts();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(synthName[synthSelect]);
        switch (synthSelect) {
          case 0:
          toggle_braids(0,1,0);
          chordOrganenvelope1.noteOff();
          AudioNoInterrupts();
          kelpieOn();
          MIDI.setHandleNoteOff(KelpieOnNoteOff);
          MIDI.setHandleNoteOn(KelpieOnNoteOn);
          AudioInterrupts();
          break;

          case 1:
          toggle_braids(0,1,0);
          kelpieOff();
          AudioNoInterrupts();
          MIDI.setHandleNoteOff(ChordOrganOnNoteOff);
          MIDI.setHandleNoteOn(ChordOrganOnNoteOn);
          AudioInterrupts();
          chordOrganenvelope1.noteOn();
          break;

          case 2:
          chordOrganenvelope1.noteOff();
          kelpieOff();
          AudioNoInterrupts();
          usbMIDI.setHandleNoteOn(braidsHandleNoteOn);
          AudioInterrupts();
          toggle_braids(0,1,3);
          break;
        }
      }
    }
    if(firstTime){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(synthName[newRight1]);
      firstTime = synthParam;
    }
  }else{
    firstTime = synthParam;
  }
}

boolean openSDCard() {
    int crashCountdown = 0;
    if (!(SD.begin(chipSelect))) {
        while (!(SD.begin(chipSelect))) {
            delay(20);
            crashCountdown++;
            if (crashCountdown > 4) {
                return false;
            }
        }
    }
    return true;
}

void setup(){
  // Configure the ADCs
  analogReadResolution(7);
  analogReadAveraging(4);
  analogReference(EXTERNAL);

  // Configure the DACs
  analogWriteResolution(12);
  DACS1.analogReference(INTERNAL);
  AudioMemory(600);

  mainMix.gain(0, 1);
  mainAMP.gain(5);

  Serial.println("Initializing SD card...");

  boolean hasSD = openSDCard();

  if (!hasSD) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.begin(9600);
  Serial.print("Begin");

  setup_lcd();
  setup_progressbar();

  setup_hardware_controls();

  Serial.print(ANALOG_CONTROL_PINS);

  lcd.setCursor(0,0);
  lcd.print("SuperSynth");
  delay(2000);

  init_banks();

  MIDI.begin();

  kelpie_setup();
  setup_chordOrgan(hasSD);
  setup_braids();
  toggle_braids(0,1,0);
}

void loop(){
  MIDI.read();
  runSamplePlayer();
  selectSynth();
  switch (synthSelect) {
    case 0:
    kelpie_run();
    break;

    case 1:
    chordOrgan_run();
    break;

    case 2:
    run_braids();
    break;
  }
}
