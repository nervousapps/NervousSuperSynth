#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
#include "hardware/pins.h"
#include "display/Display.h"
#include "hardware/HardwareControls.h"
#include "sampleplayer/SamplePlayer.h"
#include "kelpie/kelpiemaster.h"


void setup(){
  // Configure the ADCs
  analogReadResolution(9);
  analogReadAveraging(4);
  analogReference(EXTERNAL);

  // Configure the DACs
  analogWriteResolution(16);
  DACS1.analogReference(INTERNAL);
  AudioMemory(800);

  // Set mixers gain
  mix1.gain(0, 3);
  mix1.gain(1, 3);
  mix2.gain(0, 3);
  mix2.gain(1, 3);
  mix2.gain(2, 3);
  mix2.gain(3, 3);
  mix3.gain(0, 3);
  mix3.gain(1, 3);

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
}

void loop(){
  control_sampleplayer();
  sampleplay();
  kelpie_run();
}
