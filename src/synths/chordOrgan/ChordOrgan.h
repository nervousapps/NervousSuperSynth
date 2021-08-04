#include "Arduino.h"

#include <AudioPrivate.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
// #include <EEPROM.h>

#include "Settings.h"
#include "Tuning.h"
#include "Waves.h"
// #include "LedControl.h"
#include "AudioEngine.h"
#include "AnalogInput.h"
#include "Interface.h"
// #include "Trig.h"

//#define REQUIRE_SD_CARD

//#define DEBUG_STARTUP

// Debug Flags
//#define CHECK_CPU

// REBOOT CODES
// #define RESTART_ADDR       0xE000ED0C
// #define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
// #define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

elapsedMillis cpuCheckTimer;

// Current waveform index
int waveform = 0;
int waveformPage = 0;
int waveformPages = 4;

AudioEngine audioEngine;
Settings settings("CHORDORG.TXT");
Tuning tuning("TUNING.SCL");
Interface interface;
// Trig trig;

AudioConnection ChordOrganpatchCord1(audioEngine.envelope1, 0, mainMix1, 0);

void selectWaveform(int waveform) {
  waveformPage = waveform >> 2;
}

void nextWaveform() {
  // waveform++;
  // waveform = waveform % (4 * waveformPages);
  selectWaveform(waveform);
  audioEngine.update(waveform, interface.rootNotePot);
  device->updateLine(1, audioEngine.line);
}

// void reBoot(int delayTime){
//   if (delayTime > 0)
//   delay (delayTime);
//   WRITE_RESTART(0x5FA0004);
// }

void checkCPU() {
  if (cpuCheckTimer > 100) {
    int maxCPU = AudioProcessorUsageMax();
    Serial.print("MaxCPU=");
    Serial.println(maxCPU);
    cpuCheckTimer = 0;
  }
}

void ChordOrganhandlechordCVInput(byte inputIndex, unsigned int value, int diffToPrevious){
  unsigned int mapped_value = map(
    value,
    0,
    1024,
    0,
    8192);
  DisplayParamMsec = 0;
  device->updateLine(1, "chordCVvalue : " + String(mapped_value));
  interface.chordCVvalue = mapped_value;
}

void ChordOrganhandlechordPotInput(byte inputIndex, unsigned int value, int diffToPrevious){
  unsigned int mapped_value = map(
    value,
    0,
    127,
    0,
    8192);
  DisplayParamMsec = 0;
  device->updateLine(1, "chordPotvalue : " + String(mapped_value));
  interface.chordPotvalue = mapped_value;
}

void ChordOrgannoteOn(byte channel, byte note, byte velocity){
  unsigned int value = map(
    note,
    0,
    127,
    0,
    8192);
  interface.rootCVvalue = value;
  audioEngine.envelope1.noteOn();
}

void ChordOrgannoteOff(byte channel, byte note, byte velocity){
  audioEngine.envelope1.noteOff();
}

void ChordOrganhandlerootPotInput(byte inputIndex, unsigned int value, int diffToPrevious){
  unsigned int mapped_value = map(
    value,
    0,
    127,
    0,
    8192);
  DisplayParamMsec = 0;
  device->updateLine(1, "rootPotvalue : " + String(mapped_value));
  interface.rootPotvalue = mapped_value;
}

void ChordOrganhandlerootCVInput(byte inputIndex, unsigned int value, int diffToPrevious){
  unsigned int mapped_value = map(
    value,
    0,
    1024,
    0,
    8192);
  DisplayParamMsec = 0;
  device->updateLine(1, "rootCVvalue : " + String(mapped_value));
  interface.rootCVvalue = mapped_value;
}

void ChordOrganhandleAttack(byte inputIndex, unsigned int value, int diffToPrevious){
  long mapped_value = map(
    value,
    0,
    127,
    10.5,
    20);
  DisplayParamMsec = 0;
  device->updateLine(1, "Attack : " + String(mapped_value));
  audioEngine.envelope1.attack(mapped_value);
}

void ChordOrganhandleDecay(byte inputIndex, unsigned int value, int diffToPrevious){
  long mapped_value = map(
    value,
    0,
    127,
    10.5,
    11880);
  DisplayParamMsec = 0;
  device->updateLine(1, "Decay : " + String(mapped_value));
  audioEngine.envelope1.decay(mapped_value);
}

void ChordOrganhandleSustain(byte inputIndex, unsigned int value, int diffToPrevious){
  long mapped_value = map(
    value,
    0,
    127,
    10.5,
    11880);
  DisplayParamMsec = 0;
  device->updateLine(1, "Sustain : " + String(mapped_value));
  audioEngine.envelope1.sustain(mapped_value);
}

void ChordOrganhandleRelease(byte inputIndex, unsigned int value, int diffToPrevious){
  long mapped_value = map(
    value,
    0,
    127,
    10.5,
    11880/2);
  DisplayParamMsec = 0;
  device->updateLine(1, "Release : " + String(mapped_value));
  audioEngine.envelope1.release(mapped_value);
}

void ChordOrganhandleGlide(byte inputIndex, unsigned int value, int diffToPrevious){
  DisplayParamMsec = 0;
  if(value > 0){
    settings.glide = true;
    device->updateLine(1, "Glide : ON");
  }else{
    settings.glide = false;
    device->updateLine(1, "Glide : OFF");
  }
}

void ChordOrganhandleStacked(byte inputIndex, unsigned int value, int diffToPrevious){
  DisplayParamMsec = 0;
  if(value > 0){
    settings.stacked = true;
    audioEngine.stackedVoices = settings.stacked;
    device->updateLine(1, "Stacked : ON");
  }else{
    settings.stacked = false;
    audioEngine.stackedVoices = settings.stacked;
    device->updateLine(1, "Stacked : OFF");
  }
}

void ChordOrganhandlePress(byte inputIndex){
  // interface.buttonState = BUTTON_SHORT_PRESS;
  nextWaveform();
}

void ChordOrganhandleDoublePress(byte inputIndex){
  interface.buttonState = BUTTON_LONG_PRESS;
}

void ChordOrgancheckEncoder(byte inputIndex, long value)
{
  waveform = value;
  device->updateLine(1, "Waveform : " + String(waveform));
}

void setupChordOrgan(){
  #ifdef DEBUG_STARTUP
  while( !Serial );
  Serial.println("Starting");
  #endif // DEBUG_STARTUP

  // SD CARD SETTINGS FOR MODULE
  // SPI.setMOSI(7);
  // SPI.setSCK(14);

  // ledControl.init();
  // trig.init();

  #ifdef REQUIRE_SD_CARD
  openSDCard();
  settings.init(true, *device);
  #else
  settings.init(openSDCard());
  #endif


  // Read waveform settings from EEPROM
  // waveform = EEPROM.read(1234);
  // if (waveform < 0) waveform = 0;

  waveformPages = settings.extraWaves ? 3 : 1;
  if(settings.extraWaves) {
    waveformPage = waveform >> 2;
  } else if(waveform > 3) {
    // If we read a custom waveform index from EEPROM
    // but they are not enabled in the config then change back to sine
    waveform = 0;
  }

  interface.init(&settings);
  tuning.init();
  audioEngine.init(&settings, tuning.createNoteMap(), waveform);

  device->setHandleCVChange(0, ChordOrganhandlechordCVInput);
  device->setHandleCVChange(1, ChordOrganhandlerootCVInput);
  device->setHandleMuxControlChange(SLIDE1, ChordOrganhandlechordPotInput);
  device->setHandleMuxControlChange(SLIDE2, ChordOrganhandlerootPotInput);
  device->setHandleMuxControlChange(SLIDE3, ChordOrganhandleAttack);
  device->setHandleMuxControlChange(SLIDE4, ChordOrganhandleDecay);
  device->setHandleMuxControlChange(SLIDE5, ChordOrganhandleSustain);
  device->setHandleMuxControlChange(SLIDE6, ChordOrganhandleRelease);
  device->setHandleSwitchChange(0, ChordOrganhandleGlide);
  device->setHandleSwitchChange(1, ChordOrganhandleStacked);
  device->setHandlePress(0, ChordOrganhandlePress);
  device->setHandleDoublePress(0, ChordOrganhandleDoublePress);
  device->setHandleEncoderChange(0, ChordOrgancheckEncoder);
  device->updateEncodeursMaxValue(0, -11);
  MIDI.setHandleNoteOn(ChordOrgannoteOn);
  MIDI.setHandleNoteOff(ChordOrgannoteOff);

  ChordOrganpatchCord1.connect();
  device->updateLine(1, "Waveform : " + String(waveform));
}

void stopchordOrgan(){
  audioEngine.stop();
  ChordOrganpatchCord1.disconnect();
}

void runChordOrgan(){
  if(DisplayParamMsec > 400 && DisplayParamMsec < 500){
    device->updateLine(1, audioEngine.line);
  }

  uint16_t state = interface.update();

  // trig.update();

  int notesUpdate = state & (ROOT_NOTE_UPDATE | CHORD_INDEX_CHANGED);
  int buttonShortPress = state & BUTTON_SHORT_PRESS;

  // if(state & BUTTON_VERY_LONG_PRESS) {
  //   // show all LEDs
  //   ledControl.multi(0xF);
  //   reBoot(50);
  // }

  if(state & BUTTON_LONG_PRESS) {
    audioEngine.stackedVoices = !audioEngine.stackedVoices;
    notesUpdate = true;
  }

  if (notesUpdate) {
    audioEngine.updateNotes(settings.notes[interface.chordIndex], interface.rootNoteCV);

    // Only glide if CV is quantised
    if(settings.glide && interface.quantiseRootCV) {
      audioEngine.startGlide();
    }
  }

  if (buttonShortPress){
    nextWaveform();
  }

  if (buttonShortPress || notesUpdate)  {
    // trig.out(true);
  }

  if(state || audioEngine.gliding) {
    audioEngine.update(waveform, interface.rootNotePot);
  }

  // ledControl.bankAndSingle(waveformPage, waveform);

  #ifdef CHECK_CPU
  checkCPU();
  #endif // CHECK_CPU
}
