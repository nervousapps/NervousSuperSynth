#ifndef Synths_h
#define Synths_h

boolean synthParam = false;
boolean sampleParam = false;
boolean firstSampleParam = true;

elapsedMillis DisplayParamMsec = 0;

// Routes for mono outs
// ChordOrgan -> mainMix1 -> 0
// DS909 -> mainMix1 -> 1
// FMSynth -> mainMix1 -> 2
// Kelpie -> mainMix1 -> 3
// Psych03 -> mainMix2 -> 0
// Tsynth -> mainMix2 -> 1
// AudioBraids -> mainMix2 -> 2

AudioMixer4                 mainMix1;
AudioMixer4                 mainMix2;

AudioConnection             synthPatchCord0(mainMix1, 0, mainMix3, 0);
AudioConnection             synthPatchCord1(mainMix2, 0, mainMix3, 1);

// #include "kelpie/kelpiemaster.h"
#include "chordOrgan/ChordOrgan.h"
// #include "tsynth/TSynth.h"
#include "audioBraids/AudioSynthBraids.h"
// #include "psyc03/Psyc03Main.h"
// #include "ds909/DS909MAIN.h"
// #include "fmsynth/SynthMain.h"



bool debug = false;

#define synthNumber 7

int synthSelect = 1;
String synthName[synthNumber] = {"Kelpie :()", "ChordOrgan", "Tsynth", "Braids", "Psych03", "DS909 :(p)", "FMSynth"};

void selectSynth(byte inputIndex, long value){
  if(firstSampleParam){
    firstSampleParam = !firstSampleParam;
    stopAudioBraids();
    stopchordOrgan();
    // stopTsynth();
    // kelpieOff();
    // stopPsyc03();
    // stopDS909();
    // stopFMSynth();
  }
  if(!synthParam){
    synthSelect = value;
    device->updateLine(1, synthName[synthSelect]);
  }
}

void confirmSynth(byte inputIndex){
  AudioNoInterrupts();
  synthParam = !synthParam;
  lcd.cursor();
  switch (synthSelect) {
    case 0:
    // setupKelpie();
    // kelpieOn();
    break;

    case 1:
    setupChordOrgan();
    break;

    case 2:
    // setupTsynth();
    break;

    case 3:
    setupAudioBraids();
    break;

    case 4:
    // setupPsyc03();
    break;

    case 5:
    // setupDS909();
    break;

    case 6:
    // setupFMSynth();
    break;
  }
  // setSamplePlayerHandlers();
  AudioInterrupts();
}

void NothingnoteOn(byte channel, byte note, byte velocity){
  return;
}

void NothingnoteOff(byte channel, byte note, byte velocity){
  return;
}

void returnToMenu(byte inputIndex){
  device->updateEncodeursMaxValue(0, 1-synthNumber);
  device->setHandleEncoderChange(0, selectSynth);
  device->setHandlePress(0, confirmSynth);
  device->setHandleDoublePress(0, nullptr);
  for(byte i =0; i<3; i++){
    device->setHandleCVChange(i, nullptr);
  }
  synthParam = false;
  device->updateEncodeursValue(0, synthSelect);
  firstSampleParam = true;
  selectSynth(0, synthSelect);
}

void setup_synths(){
  // Set handlers
  device->updateEncodeursValue(0, 0);
  device->setHandleEncoderChange(0, selectSynth);
  device->setHandlePress(0, confirmSynth);
  device->setHandleDoublePress(0, nullptr);
  device->updateEncodeursMaxValue(0, 1-synthNumber);
  for (int i=0;i<MnumControls;i++){
    device->setHandleMuxControlChange(i, nullptr);
  }
  for(byte i =0; i<3; i++){
    device->setHandleCVChange(i, nullptr);
  }
  for (int i=0;i<ANALOG_CONTROL_PINS;i++){
    device->setHandleSwitchChange(i, nullptr);
  }
  device->setHandleLongPress(0, returnToMenu);

  MIDI.setHandleNoteOn(NothingnoteOn);
  MIDI.setHandleNoteOff(NothingnoteOff);

  // Power off all synths
  selectSynth(0, synthSelect);
  returnToMenu(0);
}

void run_synths(){
  if(synthParam){
    switch (synthSelect) {
      case 0:
      // runKelpie();
      break;

      case 1:
      runChordOrgan();
      break;

      case 2:
      // runTsynth();
      break;

      case 3:
      // AudioSynthBraids
      break;

      case 4:
      // runPsyc03();
      break;

      case 5:
      // runDS909();
      break;

      case 6:
      // runFMSynth();
      break;
    }
  }
}

#endif
