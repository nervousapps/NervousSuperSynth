#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <AudioPrivate.h>
#include "hardware/pins.h"
#include <MIDI.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include "hardware/HardwareControls.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

AudioMixer4          mainMix;
AudioAmplifier       mainAMP;
// AudioOutputAnalogStereo     DACS1;
AudioOutputAnalog     DACS1;
AudioOutputPT8211           pt8211_1;
AudioConnection             mainpatchcord0(mainMix, 0, mainAMP, 0);
AudioConnection             mainpatchcord2(mainAMP, 0, DACS1, 0);
// AudioConnection             mainpatchcord3(mainAMP, 0, DACS1, 1);

boolean synthParam = false;
boolean sampleParam = false;
boolean hasSD = false;

bool displayChange = true;
bool debug = false;

#define synthNumber 4

int synthSelect = -9;
char synthName[synthNumber][16] = {"Kelpie", "ChordOrgan", "Tsynth", "Braids"};

const int chipSelect = BUILTIN_SDCARD;

#include "sampleplayer/SamplePlayer.h"
#include "kelpie/kelpiemaster.h"
#include "chordOrgan/ChordOrgan.h"
#include "tsynth/TSynth.h"
#include "braids/braids.h"

#include "display/Display.h"

void selectSynth(){
  long newRight1;
  if(!synthParam){
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
      positionRight1 = newRight1;
      displayChange = true;
    }
    if(digital_encsw[0].update()){
      if(digital_encsw[0].fallingEdge()){
        AudioNoInterrupts();
        synthSelect = newRight1;
        synthParam = !synthParam;
        AudioInterrupts();
        switch (synthSelect) {
          case 0:
          AudioNoInterrupts();
          braids_off();
          chordOrganOff();
          Tsynth_off();
          kelpieOn();
          MIDI.setHandleNoteOff(KelpieOnNoteOff);
          MIDI.setHandleNoteOn(KelpieOnNoteOn);
          AudioInterrupts();
          break;

          case 1:
          AudioNoInterrupts();
          braids_off();
          kelpieOff();
          Tsynth_off();
          chordOrganOn();
          MIDI.setHandleNoteOff(ChordOrganOnNoteOff);
          MIDI.setHandleNoteOn(ChordOrganOnNoteOn);
          AudioInterrupts();
          chordOrganenvelope1.noteOn();
          break;

          case 2:
          AudioNoInterrupts();
          braids_off();
          chordOrganOff();
          kelpieOff();
          Tsynth_setup();
          MIDI.setHandleNoteOff(TsynthNoteOff);
          MIDI.setHandleNoteOn(TsynthNoteOn);
          AudioInterrupts();
          break;

          case 3:
          chordOrganOff();
          Tsynth_off();
          kelpieOff();
          AudioNoInterrupts();
          MIDI.setHandleNoteOn(braidsHandleNoteOn);
          AudioInterrupts();
          braids_on();
          break;
        }
        displayChange = true;
      }
    }
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
  // Catch debug mode
  if(digital_encsw[0].update()){
    if(digital_encsw[0].fallingEdge()){
      debug = true;
      // Init serial
      Serial.begin(9600);
      Serial.print("Begin");
    }
  }
  debug = true;
  Serial.begin(9600);
  Serial.print("Begin");

  // Configure the ADCs
  analogReadResolution(7);
  analogReadAveraging(4);
  analogReference(EXTERNAL);

  // Configure the DACs
  analogWriteResolution(12);
  DACS1.analogReference(INTERNAL);
  AudioMemory(200);

  // Set main mixer volume
  mainMix.gain(0, 0.25);
  mainMix.gain(1, 0.25);
  mainMix.gain(2, 0.25);
  mainMix.gain(3, 0.25);
  mainAMP.gain(50);

  // Init SD card
  if(debug) Serial.println("Initializing SD card...");
  hasSD = openSDCard();
  if (!hasSD) {
    if(debug) Serial.println("initialization failed!");
  }

  // Init LCD
  setup_lcd();
  setup_progressbar();
  lcd.setCursor(0,0);

  // Init hardware controls
  setup_hardware_controls();
  if(debug) Serial.print(ANALOG_CONTROL_PINS);

  // Init sampleplayer banks
  init_banks();

  // Init MIDI
  MIDI.begin();

  // Init Kelpie and power off
  kelpie_setup();
  kelpieOff();

  // Init Tsynth and power off
  Tsynth_setup();
  Tsynth_off();

  // Init chordOrgan and power off
  setup_chordOrgan(hasSD);
  chordOrganOff();

  // Init Braids and power off
  setup_braids();
  braids_off();

  // Init LCD
  lcd.setCursor(0,0);

  // Starting animation
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
  MIDI.read();
  runSamplePlayer();
  selectSynth();
  if(synthParam){
    switch (synthSelect) {
      case 0:
      kelpie_run();
      break;

      case 1:
      chordOrgan_run();
      break;

      case 2:
      Tsynth_run();
      break;

      case 3:
      run_braids();
      break;
    }
  }
  printInfos();

  if(debug){
    cpuLoadSleep();
    if(has_result){
      Serial.println(cpu_load);
    }
    CPUMonitor();
  }
}
