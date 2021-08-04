#include <Arduino.h>
#include <Wire.h>
#include <AudioPrivate.h>
#include <MIDI.h>
#include <SD.h>
#include <SPI.h>
#include <NervousSuperMother.h>
#include "main_settings.h"
#include "main_utils.h"

#if DEBUG
elapsedMillis DisplayDebug = 0;
#endif

// Create a default midi instance => MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// Motherboard
NervousSuperMother * device = NervousSuperMother::getInstance();


AudioControlSGTL5000        sgtl5000_1;
AudioOutputI2S              output_i2s;
AudioInputI2S               input_i2s;
AudioOutputPT8211_2         pt8211_1;

// Routes for mono outs
// ChordOrgan -> mainMix1 -> 0
// DS909 -> mainMix1 -> 1
// FMSynth -> mainMix1 -> 2
// Kelpie -> mainMix1 -> 3
// Psych03 -> mainMix2 -> 0
// Tsynth -> mainMix2 -> 1
// AudioBraids -> mainMix2 -> 2

// mainMix 1 -> mainMix3 -> 0
// mainMix 2 -> mainMix3 -> 1

AudioMixer4                 mainMix3;

AudioMixer4                 mainMixI2s_L;
AudioMixer4                 mainMixI2s_R;
AudioMixer4                 mainMix8211_L;
AudioMixer4                 mainMix8211_R;
AudioAmplifier              mainAmpI2s_L;
AudioAmplifier              mainAmpI2s_R;
AudioAmplifier              mainAmp8211_L;
AudioAmplifier              mainAmp8211_R;

AudioConnection             mainpatchcord0(mainMix3, 0, mainMixI2s_L, 0);
AudioConnection             mainpatchcord1(mainMix3, 0, mainMixI2s_R, 0);
AudioConnection             mainpatchcord2(mainMixI2s_L, 0, mainAmpI2s_L, 0);
AudioConnection             mainpatchcord3(mainMixI2s_R, 0, mainAmpI2s_R, 0);
AudioConnection             mainpatchcord4(mainAmpI2s_L, 0, output_i2s, 0);
AudioConnection             mainpatchcord5(mainAmpI2s_R, 0, output_i2s, 1);

AudioConnection             mainpatchcord00(mainMix3, 0, mainMix8211_L, 0);
AudioConnection             mainpatchcord11(mainMix3, 0, mainMix8211_R, 0);
AudioConnection             mainpatchcord22(mainMix8211_L, 0, mainAmp8211_L, 0);
AudioConnection             mainpatchcord33(mainMix8211_R, 0, mainAmp8211_R, 0);
AudioConnection             mainpatchCord44(mainAmp8211_L, 0, pt8211_1, 0);
AudioConnection             mainpatchCord55(mainAmp8211_R, 0, pt8211_1, 1);

#ifdef SYNTHS
#include "synths/synths.h"
#endif

#ifdef SAMPLEPLAYER
#include "sampleplayer/SamplePlayer.h"
#endif

#ifdef EFFECTSPROCESSOR
#include "effectsprocessor/EffectsProcessor.h"
#endif


void setVol(float value){
  float rate = (value / 64.0);

  mainAmp8211_L.gain(rate/6.0);
  mainAmp8211_R.gain(rate/6.0);

  mainAmpI2s_L.gain(rate);
  mainAmpI2s_R.gain(rate);
  device->updateLine(2, "Vol : " + String(rate));
  // draw_progressbar(value/10);
}

void setup(){
#if DEBUG
  Serial.begin(9600);
  Serial.print("!    SuperSynth    !");
#endif

  // Set TX of Serial1 to 53 instead of 1 as encoder 1 uses that pin.
  Serial1.setTX(53);

  AudioNoInterrupts();
  // Configure the ADCs
  analogReadResolution(10);
  analogReadAveraging(16);
  analogReference(0);

  // Configure the DACs
  // analogWriteResolution(12);

  // Init audio
  AudioMemory(1500);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineOutLevel(0.8);
  sgtl5000_1.dacVolume(1.0);
  sgtl5000_1.muteHeadphone();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.audioPreProcessorEnable();
  sgtl5000_1.audioPostProcessorEnable();
  sgtl5000_1.surroundSoundEnable();
  sgtl5000_1.enhanceBassEnable();

  // Set main mixers/amplificator volume
  // Little trick here as I did not succeed to get separate outputs for the two DACs
  // But working with the same mixer out.
  mainMixI2s_L.gain(0,0.5);  // Synths
  mainMixI2s_R.gain(0,0.5);
  mainMixI2s_L.gain(1,0);  // EffectProcessor
  mainMixI2s_R.gain(1,0);
  mainMixI2s_L.gain(2,0.5);  // SamplePlayer
  mainMixI2s_R.gain(2,0.5);
  mainMixI2s_L.gain(3,0);  // None
  mainMixI2s_R.gain(3,0);

  mainMix8211_L.gain(0,0);  // Synths
  mainMix8211_R.gain(0,0);
  mainMix8211_L.gain(1,0.5);  // EffectProcessor
  mainMix8211_R.gain(1,0.5);
  mainMix8211_L.gain(2,0.5);  // SamplePlayer
  mainMix8211_R.gain(2,0.5);
  mainMix8211_L.gain(3,0);  // None
  mainMix8211_R.gain(3,0);

  mainAmpI2s_L.gain(2);
  mainAmpI2s_R.gain(2);
  mainAmp8211_L.gain(0.2);
  mainAmp8211_R.gain(0.2);

  // Init SD card
  init_sd_card();

  // Init MIDI
  MIDI.begin(MIDI_CHANNEL);

  // Init device NervousSuperMother
  byte controls[7] = {0,1,2,3,4,5,6};
  device->init(controls);

  device->setHandleVolChange(setVol);

#ifdef SYNTHS
  setup_synths();
#endif

#ifdef SAMPLEPLAYER
  setup_sampleplayer();
#endif

#ifdef EFFECTSPROCESSOR
  setup_effect_processor();
#endif

  // Starting animation
  lcd.setCursor(0,0);
  lcd.print("!    SuperSynth    !");
  for(int i=0; i<100; i++){
    draw_progressbar(i);
    delay(2);
  }

  AudioInterrupts();
}

void loop(){
  MIDI.read();
  device->update();

#ifdef SYNTHS
  run_synths();
#endif

#ifdef SAMPLEPLAYER
  // SAMPLEPLAYER
#endif

#ifdef EFFECTSPROCESSOR
  // EFFECTSPROCESSOR
#endif

#if DEBUG
  if(DEBUG && DisplayDebug > 1000){
    cpuLoadSleep();
    if(has_result){
      Serial.println(cpu_load);
    }
    CPUMonitor();
    DisplayDebug = 0;
  }
#endif
}
