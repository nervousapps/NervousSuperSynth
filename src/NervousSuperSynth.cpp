#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <AudioPrivate.h>
#include <MIDI.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <TeensyThreads.h>
#include <NervousSuperMother.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// Motherboard
NervousSuperMother * device = NervousSuperMother::getInstance();

AudioMixer4                 mainMix;
AudioMixer4                 mainMix2;
AudioMixer4                 mainMix3;
AudioAmplifier              mainAMP;

// AudioOutputAnalogStereo     DACS1;
AudioOutputAnalog           DACS1;
AudioOutputPT8211           pt8211_1;
AudioConnection             mainpatchcord0(mainMix, 0, mainMix2, 0);
AudioConnection             mainpatchcord1(mainMix2, 0, mainMix3, 0);
AudioConnection             mainpatchcord2(mainMix3, 0, mainAMP, 0);
AudioConnection             mainpatchcord3(mainAMP, 0, DACS1, 0);
// AudioConnection             mainpatchcord3(mainAMP, 0, DACS1, 1);

boolean synthParam = false;
boolean sampleParam = false;
boolean firstSampleParam = true;
boolean hasSD = false;

elapsedMillis DisplayParamMsec = 0;
// elapsedMillis DisplayDebug = 0;

bool debug = false;

#define synthNumber 6

int synthSelect = 0;
char synthName[synthNumber][16] = {"Kelpie", "ChordOrgan", "Tsynth", "Braids", "Psych03", "DS909"};

const int chipSelect = BUILTIN_SDCARD;

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

#include "sampleplayer/SamplePlayer.h"
#include "kelpie/kelpiemaster.h"
#include "chordOrgan/ChordOrgan.h"
#include "tsynth/TSynth.h"
#include "braids/braids.h"
#include "psyc03/Psyc03Main.h"
#include "ds909/DS909MAIN.h"

void selectSynth(byte inputIndex, long value){
  if(firstSampleParam){
    firstSampleParam = !firstSampleParam;
    braids_off();
    stopchordOrgan();
    Tsynth_off();
    kelpieOff();
    stopPsyc03();
    stopDS909();
  }
  if(!synthParam){
    synthSelect = value;
    device->updateEncodeursMaxValue(0, synthNumber-1);
    device->updateLine(1, String(synthName[synthSelect]));
  }
}

void confirmSynth(byte inputIndex){
  AudioNoInterrupts();
  synthParam = !synthParam;
  switch (synthSelect) {
    case 0:
    setupKelpie();
    kelpieOn();
    break;

    case 1:
    setupChordOrgan();
    break;

    case 2:
    setupTsynth();
    break;

    case 3:
    braids_on();
    break;

    case 4:
    setupPsyc03();
    break;

    case 5:
    setupDS909();
    break;
  }
  AudioInterrupts();
}

void NothingnoteOn(byte channel, byte note, byte velocity){
  return;
}

void NothingnoteOff(byte channel, byte note, byte velocity){
  return;
}

void returnToMenu(byte inputIndex){
  MIDI.setHandleNoteOn(NothingnoteOn);
  MIDI.setHandleNoteOff(NothingnoteOff);
  device->updateEncodeursMaxValue(0, synthNumber-1);
  device->setHandleEncoderChange(0, selectSynth);
  device->setHandlePress(0, confirmSynth);
  for (int i=0;i<ANALOG_CONTROL_PINS;i++){
    device->setHandlePotentiometerChange(i, nullptr);
  }
  synthParam = false;
  device->updateEncodeursValue(0, synthSelect);
  firstSampleParam = true;
  selectSynth(0, synthSelect);
}

void setup(){
  // Catch debug mode
  // if(digital_encsw[0].update()){
  //   if(digital_encsw[0].fallingEdge()){
  //     debug = true;
  //     // Init serial
  //     Serial.begin(9600);
  //     Serial.print("Begin");
  //   }
  // }
  // debug = true;
  // Serial.begin(9600);
  // Serial.print("!    SuperSynth    !");

  // Configure the ADCs
  analogReadResolution(7);
  analogReadAveraging(16);
  analogReference(EXTERNAL);

  // Configure the DACs
  analogWriteResolution(12);
  DACS1.analogReference(INTERNAL);
  AudioMemory(500);

  // Set main mixer volume
  mainMix.gain(0, 0.25);
  mainMix.gain(1, 0.25);
  mainMix.gain(2, 0.25);
  mainMix.gain(3, 0.25);
  mainAMP.gain(10);

  // Init SD card
  if(debug) Serial.println("Initializing SD card...");
  hasSD = openSDCard();
  if (!hasSD) {
    if(debug) Serial.println("initialization failed!");
  }

  // Init hardware controls
  setup_hardware_controls();
  if(debug) Serial.print(ANALOG_CONTROL_PINS);

  // Init MIDI
  MIDI.begin();

  // Init device NervousSuperMother
  byte controls[6] = {0,1,2,3,4,5};
  device->init(controls);

  // Set the handlers
  initSamplePlayer();

  // Power off all synths
  selectSynth(0, synthSelect);

  // Set handlers
  device->updateEncodeursValue(0, 0);
  device->setHandleEncoderChange(0, selectSynth);
  device->setHandlePress(0, confirmSynth);
  device->updateEncodeursMaxValue(0, synthNumber-1);
  for (int i=0;i<ANALOG_CONTROL_PINS;i++){
    device->setHandlePotentiometerChange(i, nullptr);
  }
  device->setHandleDoublePress(0, returnToMenu);

  // Starting animation
  lcd.setCursor(0,0);
  lcd.print("!    SuperSynth    !");
  for(int i=0; i<100; i++){
    draw_progressbar(i);
    delay(2);
  }
}

volatile uint32_t cpu_load = 0;
volatile bool has_result = false;

void cpuLoadSleep()
{
    uint32_t st;
    static uint32_t wt = 0;
    static uint32_t busy_time = 0;
    static uint32_t sleep_time = 0;

    busy_time += ARM_DWT_CYCCNT - wt;
    st = ARM_DWT_CYCCNT;
    __disable_irq();
    __asm volatile ("wfi \n");
    sleep_time += ARM_DWT_CYCCNT - st;
    wt = ARM_DWT_CYCCNT;
    __enable_irq();
    if ((busy_time + sleep_time) > F_CPU) {
        cpu_load = busy_time / ((busy_time + sleep_time) / 1000);
        busy_time = 0;
        sleep_time = 0;
        has_result = true;
    }
}

void CPUMonitor() {
  Serial.print("CPU:");
  Serial.print(AudioProcessorUsage());
  Serial.print("  MEM:");
  Serial.println(AudioMemoryUsage());
  Serial.print("CPU MAX:");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("  MEM MAX:");
  Serial.println(AudioMemoryUsageMax());
}

void loop(){
  device->update();
  MIDI.read();
  if(synthParam){
    switch (synthSelect) {
      case 0:
      runKelpie();
      break;

      case 1:
      runChordOrgan();
      break;

      case 2:
      runTsynth();
      break;

      case 3:
      runBraids();
      break;

      case 4:
      runPsyc03();
      break;

      case 5:
      runDS909();
      break;
    }
  }

  if(debug){  //&& DisplayDebug > 1000){
    cpuLoadSleep();
    if(has_result){
      Serial.println(cpu_load);
    }
    CPUMonitor();
    // DisplayDebug = 0;
  }
}
