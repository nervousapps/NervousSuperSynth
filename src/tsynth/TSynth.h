/*
   MIT License

  Copyright (c) 2020 ElectroTechnique

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ElectroTechnique TSynth - Firmware Rev 1.28

  Includes code by:
    Dave Benn - Handling MUXs, a few other bits and original inspiration  https://www.notesandvolts.com/2019/01/teensy-synth-part-10-hardware.html
    Alexander Davis - Stereo ensemble chorus effect https://github.com/quarterturn/teensy3-ensemble-chorus
    Gustavo Silveira - Band limited wavetables https://forum.pjrc.com/threads/41905-Band-limited-Sawtooth-wavetable-C-generator-for-quot-Arbitrary-Waveform-quot-(and-its-use)
    Holger Wirtz - Modified library integration and special thanks https://www.parasitstudio.de/

  Arduino IDE
  Tools Settings:
  Board: "Teensy3.6"
  USB Type: "Serial + MIDI + Audio"
  CPU Speed: "180MHz"
  Optimize: "Faster"

  +++ Compiling with overclocking can lead to problems, test carefully +++
  Performance Tests   CPU  Mem
  180Mhz Faster       81.6 44
  180Mhz Fastest      77.8 44
  180Mhz Fastest+PC   79.0 44
  180Mhz Fastest+LTO  76.7 44
  240MHz Fastest+LTO  55.9 44

  Additional libraries:
    Agileware CircularBuffer, Adafruit_GFX (available in Arduino libraries manager)
*/

#include <AudioPrivate.h> //Using local version to override Teensyduino version
#include "MidiCC.h"
#include "AudioPatching.h"
#include "Constants.h"
#include "Parameters.h"
#include "PatchMgr.h"
#include "HWControls.h"
#include "sawtoothWave.h"
#include "squareWave.h"
#include "Velocity.h"

#define PARAMETER 0 //The main page for displaying the current patch and control (parameter) changes
#define SAVE 1 //Save patch page
#define REINITIALISE 2 // Reinitialise message
#define PATCHNAMING 3 // Patch naming page
#define DELETE 4 //Delete patch page
#define SETTINGS 5 //Settings page
#define SETTINGSVALUE 6 //Settings page
#define DELETEMSG 7 //Delete patch message page
#define RECALL 8 //Patches list
#define PATCH 9 // Show current patch bypassing PARAMETER

String TsynthFirstParamName[7] = {
  "PATCHES", //The main page for displaying the current patch and control (parameter) changes
  // "RECALL", //Patches list
  "SAVE", //Save patch page
  "REINITIALISE", // Reinitialise message
  // "PATCH", // Show current patch bypassing PARAMETER
  "PATCHNAMING", // Patch naming page
  "DELETE", //Delete patch page
  // "DELETEMSG", //Delete patch message page
  "SETTINGS", //Settings page
  // "SETTINGSVALUE", //Settings page
};

unsigned int state = PARAMETER;

#define WAVEFORM_SAWTOOTH_WT 101
#define WAVEFORM_SQUARE_WT 102
#define WAVEFORM_PARABOLIC 103
#define WAVEFORM_HARMONIC 104

struct VoiceAndNote {
  int note;
  long timeOn;
  boolean voiceOn;
};

struct VoiceAndNote voices[NO_OF_VOICES] = {
  { -1, 0, false},
  { -1, 0, false},
  { -1, 0, false},
  { -1, 0, false},
  { -1, 0, false},
  { -1, 0, false}
};

#include "Settings.h"

boolean cardStatus = false;
boolean firstPatchLoaded = false;

//USB HOST MIDI Class Compliant
// USBHost myusb;
// USBHub hub1(myusb);
// USBHub hub2(myusb);
// MIDIDevice midi1(myusb);

//MIDI 5 Pin DIN
// MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI); //RX - Pin 31

int prevNote = 48;//This is for glide to use previous note to glide from
float previousMillis = millis(); //For MIDI Clk Sync

int count = 0;//For MIDI Clk Sync
int patchNo = 1;//Current patch no
int voiceToReturn = -1; //Initialise
long earliestTime = millis(); //For voice allocation - initialise to now

void showCurrentParameterPage(String parameter, String value);
int getVoiceNo(int note);
void updateVoice1();
void updateVoice2();
void updateVoice3();
void updateVoice4();
void updateVoice5();
void updateVoice6();
int getLFOWaveform(int value);
String getWaveformStr(int value);
void loadArbWaveformA(const int16_t * wavedata);
void loadArbWaveformB(const int16_t * wavedata);
float getLFOTempoRate(int value);
int getWaveformA(int value);
int getWaveformB(int value);
int getPitch(int value);
void setPwmMixerALFO(float value);
void setPwmMixerBLFO(float value);
void setPwmMixerAPW(float value);
void setPwmMixerBPW(float value);
void setPwmMixerAFEnv(float value);
void setPwmMixerBFEnv(float value);
void updateUnison();
void updateVolume(float vol);
void updateGlide();
void updateWaveformA();
void updateWaveformB();
void updatePitchA();
void updatePitchB();
void updateDetune();
void updatesAllVoices();
void updatePWMSource();
void updatePWMRate();
void updatePWMAmount();
void updatePWA();
void updatePWB();
void updateOscLevelA();
void updateOscLevelB();
void updateNoiseLevel();
void updateFilterFreq();
void updateFilterRes();
void updateFilterMixer();
void updateFilterEnv();
void updatePitchEnv();
void updateKeyTracking();
void updateOscLFOAmt();
void updateModWheel();
void updatePitchLFORate();
void updatePitchLFOWaveform();
void updatePitchLFOMidiClkSync();
void updateFilterLfoRate();
void updateFilterLfoAmt();
void updateFilterLFOWaveform();
void updatePitchLFORetrig();
void updateFilterLFORetrig();
void updateFilterLFOMidiClkSync();
void updateFilterAttack();
void updateFilterDecay();
void updateFilterSustain();
void updateFilterRelease();
void updateAttack();
void updateDecay();
void updateSustain();
void updateRelease();
void updateOscFX();
void updateFXAmt();
void updateFXMix();
void updatePatchname();
void myPitchBend(byte channel, int bend);
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);
void allNotesOff();
void myControlChange(byte channel, byte control, byte value);
void myProgramChange(byte channel, byte program);
void myMIDIClockStart();
void myMIDIClockStop();
void myMIDIClock();
void closeEnvelopes();
void recallPatch(int patchNo);
void setCurrentPatchData(String data[]);
String getCurrentPatchData();
void checkVolumePot();
void checkSwitches();
void TsynthcheckDoublePress(byte inputIndex);
void TsynthcheckPress(byte inputIndex);
void TsynthcheckEncoder(byte inputIndex, long value);


void showCurrentParameterPage(String parameter, String value){
  device->updateLine(1, parameter + " : " + value);
}

int getVoiceNo(int note)
{
  voiceToReturn = -1;      //Initialise
  earliestTime = millis(); //Initialise to now
  if (note == -1)
  {
    //NoteOn() - Get the oldest free voice (recent voices may be still on release stage)
    for (int i = 0; i < NO_OF_VOICES; i++)
    {
      if (voices[i].voiceOn == false)
      {
        if (voices[i].timeOn < earliestTime)
        {
          earliestTime = voices[i].timeOn;
          voiceToReturn = i;
        }
      }
    }
    if (voiceToReturn == -1)
    {
      //No free voices, need to steal oldest sounding voice
      earliestTime = millis(); //Reinitialise
      for (int i = 0; i < NO_OF_VOICES; i++)
      {
        if (voices[i].timeOn < earliestTime)
        {
          earliestTime = voices[i].timeOn;
          voiceToReturn = i;
        }
      }
    }
    return voiceToReturn + 1;
  }
  else
  {
    //NoteOff() - Get voice number from note
    for (int i = 0; i < NO_OF_VOICES; i++)
    {
      if (voices[i].note == note && voices[i].voiceOn)
      {
        return i + 1;
      }
    }
  }
  //Shouldn't get here, return voice 1
  return 1;
}

void updateVoice1()
{
  if (oscWaveformA == WAVEFORM_SAWTOOTH_WT) waveformMod1a.arbitraryWaveform(sawtoothWavetable[(voices[0].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SAWTOOTH_WT) waveformMod1b.arbitraryWaveform(sawtoothWavetable[(voices[0].note + oscPitchB) / 3 + 1], AWFREQ);
  if (oscWaveformA == WAVEFORM_SQUARE_WT) waveformMod1a.arbitraryWaveform(squareWavetable[(voices[0].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SQUARE_WT) waveformMod1b.arbitraryWaveform(squareWavetable[(voices[0].note + oscPitchB) / 3 + 1], AWFREQ);

  waveformMod1a.frequency(NOTEFREQS[voices[0].note + oscPitchA]);
  if (unison == 1)
  {
    waveformMod1b.frequency(NOTEFREQS[voices[0].note + oscPitchB] * (detune + ((1 - detune) * 0.09)));
  }
  else
  {
    waveformMod1b.frequency(NOTEFREQS[voices[0].note + oscPitchB] * detune);
  }
}

void updateVoice2()
{
  if (oscWaveformA == WAVEFORM_SAWTOOTH_WT) waveformMod2a.arbitraryWaveform(sawtoothWavetable[(voices[1].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SAWTOOTH_WT) waveformMod2b.arbitraryWaveform(sawtoothWavetable[(voices[1].note + oscPitchB) / 3 + 1], AWFREQ);
  if (oscWaveformA == WAVEFORM_SQUARE_WT) waveformMod2a.arbitraryWaveform(squareWavetable[(voices[1].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SQUARE_WT) waveformMod2b.arbitraryWaveform(squareWavetable[(voices[1].note + oscPitchB) / 3 + 1], AWFREQ);
  if (unison == 1)
  {
    waveformMod2a.frequency(NOTEFREQS[voices[1].note + oscPitchA] * (detune + ((1 - detune) * 0.18)));
    waveformMod2b.frequency(NOTEFREQS[voices[1].note + oscPitchB] * (detune + ((1 - detune) * 0.27)));
  }
  else
  {
    waveformMod2a.frequency(NOTEFREQS[voices[1].note + oscPitchA]);
    waveformMod2b.frequency(NOTEFREQS[voices[1].note + oscPitchB] * detune);
  }
}

void updateVoice3()
{
  if (oscWaveformA == WAVEFORM_SAWTOOTH_WT) waveformMod3a.arbitraryWaveform(sawtoothWavetable[(voices[2].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SAWTOOTH_WT) waveformMod3b.arbitraryWaveform(sawtoothWavetable[(voices[2].note + oscPitchB) / 3 + 1], AWFREQ);
  if (oscWaveformA == WAVEFORM_SQUARE_WT) waveformMod3a.arbitraryWaveform(squareWavetable[(voices[2].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SQUARE_WT) waveformMod3b.arbitraryWaveform(squareWavetable[(voices[2].note + oscPitchB) / 3 + 1], AWFREQ);
  if (unison == 1)
  {
    waveformMod3a.frequency(NOTEFREQS[voices[2].note + oscPitchA] * (detune + ((1 - detune) * 0.36)));
    waveformMod3b.frequency(NOTEFREQS[voices[2].note + oscPitchB] * (detune + ((1 - detune) * 0.46)));
  }
  else
  {
    waveformMod3a.frequency(NOTEFREQS[voices[2].note + oscPitchA]);
    waveformMod3b.frequency(NOTEFREQS[voices[2].note + oscPitchB] * detune);
  }
}
void updateVoice4()
{
  if (oscWaveformA == WAVEFORM_SAWTOOTH_WT) waveformMod4a.arbitraryWaveform(sawtoothWavetable[(voices[3].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SAWTOOTH_WT) waveformMod4b.arbitraryWaveform(sawtoothWavetable[(voices[3].note + oscPitchB) / 3 + 1], AWFREQ);
  if (oscWaveformA == WAVEFORM_SQUARE_WT) waveformMod4a.arbitraryWaveform(squareWavetable[(voices[3].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SQUARE_WT) waveformMod4b.arbitraryWaveform(squareWavetable[(voices[3].note + oscPitchB) / 3 + 1], AWFREQ);
  if (unison == 1)
  {
    waveformMod4a.frequency(NOTEFREQS[voices[3].note + oscPitchA] * (detune + ((1 - detune) * 0.55)));
    waveformMod4b.frequency(NOTEFREQS[voices[3].note + oscPitchB] * (detune + ((1 - detune) * 0.64)));
  }
  else
  {
    waveformMod4a.frequency(NOTEFREQS[voices[3].note + oscPitchA]);
    waveformMod4b.frequency(NOTEFREQS[voices[3].note + oscPitchB] * detune);
  }
}

void updateVoice5()
{
  if (oscWaveformA == WAVEFORM_SAWTOOTH_WT) waveformMod5a.arbitraryWaveform(sawtoothWavetable[(voices[4].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SAWTOOTH_WT) waveformMod5b.arbitraryWaveform(sawtoothWavetable[(voices[4].note + oscPitchB) / 3 + 1], AWFREQ);
  if (oscWaveformA == WAVEFORM_SQUARE_WT) waveformMod5a.arbitraryWaveform(squareWavetable[(voices[4].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SQUARE_WT) waveformMod5b.arbitraryWaveform(squareWavetable[(voices[4].note + oscPitchB) / 3 + 1], AWFREQ);
  if (unison == 1)
  {
    waveformMod5a.frequency(NOTEFREQS[voices[4].note + oscPitchA] * (detune + ((1 - detune) * 0.73)));
    waveformMod5b.frequency(NOTEFREQS[voices[4].note + oscPitchB] * (detune + ((1 - detune) * 0.82)));
  }
  else
  {
    waveformMod5a.frequency(NOTEFREQS[voices[4].note + oscPitchA]);
    waveformMod5b.frequency(NOTEFREQS[voices[4].note + oscPitchB] * detune);
  }
}

void updateVoice6()
{
  if (oscWaveformA == WAVEFORM_SAWTOOTH_WT) waveformMod6a.arbitraryWaveform(sawtoothWavetable[(voices[5].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SAWTOOTH_WT) waveformMod6b.arbitraryWaveform(sawtoothWavetable[(voices[5].note + oscPitchB) / 3 + 1], AWFREQ);
  if (oscWaveformA == WAVEFORM_SQUARE_WT) waveformMod6a.arbitraryWaveform(squareWavetable[(voices[5].note + oscPitchA) / 3 + 1], AWFREQ);
  if (oscWaveformB == WAVEFORM_SQUARE_WT) waveformMod6b.arbitraryWaveform(squareWavetable[(voices[5].note + oscPitchB) / 3 + 1], AWFREQ);
  if (unison == 1)
  {
    waveformMod6a.frequency(NOTEFREQS[voices[5].note + oscPitchA] * (detune + ((1 - detune) * 0.90)));
  }
  else
  {
    waveformMod6a.frequency(NOTEFREQS[voices[5].note + oscPitchA]);
  }
  waveformMod6b.frequency(NOTEFREQS[voices[5].note + oscPitchB] * detune);
}

int getLFOWaveform(int value)
{
  if (value >= 0 && value < 8)
  {
    return WAVEFORM_SINE;
  }
  else if (value >= 8 && value < 30)
  {
    return WAVEFORM_TRIANGLE;
  }
  else if (value >= 30 && value < 63)
  {
    return WAVEFORM_SAWTOOTH_REVERSE;
  }
  else if (value >= 63 && value < 92)
  {
    return WAVEFORM_SAWTOOTH;
  }
  else if (value >= 92 && value < 111)
  {
    return WAVEFORM_SQUARE;
  }
  else
  {
    return WAVEFORM_SAMPLE_HOLD;
  }
}

String getWaveformStr(int value)
{
  switch (value)
  {
    case WAVEFORM_SILENT:
      return "Off";
    case WAVEFORM_SAMPLE_HOLD:
      return "Sample & Hold";
    case WAVEFORM_SINE:
      return "Sine";
    case WAVEFORM_SQUARE_WT:
    case WAVEFORM_SQUARE:
      return "Square";
    case WAVEFORM_TRIANGLE:
      return "Triangle";
    case WAVEFORM_SAWTOOTH_WT:
    case WAVEFORM_SAWTOOTH:
      return "Sawtooth";
    case WAVEFORM_SAWTOOTH_REVERSE:
      return "Ramp";
    case WAVEFORM_PULSE:
      return "Var. Pulse";
    case WAVEFORM_TRIANGLE_VARIABLE:
      return "Var. Triangle";
    case WAVEFORM_PARABOLIC:
      return "Parabolic";
    case WAVEFORM_HARMONIC:
      return "Harmonic";
    default:
      return "ERR_WAVE";
  }
}

void loadArbWaveformA(const int16_t * wavedata) {
  waveformMod1a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod2a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod3a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod4a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod5a.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod6a.arbitraryWaveform(wavedata, AWFREQ);
}

void loadArbWaveformB(const int16_t * wavedata) {
  waveformMod1b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod2b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod3b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod4b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod5b.arbitraryWaveform(wavedata, AWFREQ);
  waveformMod6b.arbitraryWaveform(wavedata, AWFREQ);
}

float getLFOTempoRate(int value)
{
  lfoTempoValue = LFOTEMPO[value];
  return lfoSyncFreq * LFOTEMPO[value];
}

int getWaveformA(int value)
{
  if (value >= 0 && value < 7)
  {
    //This will turn the osc off
    return WAVEFORM_SILENT;
  }
  else if (value >= 7 && value < 23)
  {
    return WAVEFORM_TRIANGLE;
  }
  else if (value >= 23 && value < 40)
  {
    return WAVEFORM_SQUARE_WT;
  }
  else if (value >= 40 && value < 60)
  {
    return WAVEFORM_SAWTOOTH_WT;
  }
  else if (value >= 60 && value < 80)
  {
    return WAVEFORM_PULSE;
  }
  else if (value >= 80 && value < 100)
  {
    return WAVEFORM_TRIANGLE_VARIABLE;
  }
  else if (value >= 100 && value < 120)
  {
    return WAVEFORM_PARABOLIC;
  }
  else
  {
    return WAVEFORM_HARMONIC;
  }
}

int getWaveformB(int value)
{
  if (value >= 0 && value < 7)
  {
    //This will turn the osc off
    return WAVEFORM_SILENT;
  }
  else if (value >= 7 && value < 23)
  {
    return WAVEFORM_SAMPLE_HOLD;
  }
  else if (value >= 23 && value < 40)
  {
    return WAVEFORM_SQUARE_WT;
  }
  else if (value >= 40 && value < 60)
  {
    return WAVEFORM_SAWTOOTH_WT;
  }
  else if (value >= 60 && value < 80)
  {
    return WAVEFORM_PULSE;
  }
  else if (value >= 80 && value < 100)
  {
    return WAVEFORM_TRIANGLE_VARIABLE;
  }
  else if (value >= 100 && value < 120)
  {
    return WAVEFORM_PARABOLIC;
  }
  else
  {
    return WAVEFORM_HARMONIC;
  }
}

int getPitch(int value)
{
  return PITCH[value];
}

void setPwmMixerALFO(float value)
{
  pwMixer1a.gain(0, value);
  pwMixer2a.gain(0, value);
  pwMixer3a.gain(0, value);
  pwMixer4a.gain(0, value);
  pwMixer5a.gain(0, value);
  pwMixer6a.gain(0, value);
  showCurrentParameterPage("1. PWM LFO", String(value));
}

void setPwmMixerBLFO(float value)
{
  pwMixer1b.gain(0, value);
  pwMixer2b.gain(0, value);
  pwMixer3b.gain(0, value);
  pwMixer4b.gain(0, value);
  pwMixer5b.gain(0, value);
  pwMixer6b.gain(0, value);
  showCurrentParameterPage("2. PWM LFO", String(value));
}

void setPwmMixerAPW(float value)
{
  pwMixer1a.gain(1, value);
  pwMixer2a.gain(1, value);
  pwMixer3a.gain(1, value);
  pwMixer4a.gain(1, value);
  pwMixer5a.gain(1, value);
  pwMixer6a.gain(1, value);
}

void setPwmMixerBPW(float value)
{
  pwMixer1b.gain(1, value);
  pwMixer2b.gain(1, value);
  pwMixer3b.gain(1, value);
  pwMixer4b.gain(1, value);
  pwMixer5b.gain(1, value);
  pwMixer6b.gain(1, value);
}

void setPwmMixerAFEnv(float value)
{
  pwMixer1a.gain(2, value);
  pwMixer2a.gain(2, value);
  pwMixer3a.gain(2, value);
  pwMixer4a.gain(2, value);
  pwMixer5a.gain(2, value);
  pwMixer6a.gain(2, value);
  showCurrentParameterPage("1. PWM F Env", String(value));
}

void setPwmMixerBFEnv(float value)
{
  pwMixer1b.gain(2, value);
  pwMixer2b.gain(2, value);
  pwMixer3b.gain(2, value);
  pwMixer4b.gain(2, value);
  pwMixer5b.gain(2, value);
  pwMixer6b.gain(2, value);
  showCurrentParameterPage("2. PWM F Env", String(value));
}

void updateUnison()
{
  if (unison == 0)
  {
    allNotesOff();//Avoid hanging notes
    showCurrentParameterPage("Unison", "Off");
    // digitalWriteFast(UNISON_LED, LOW);  // LED off
  }
  else
  {
    showCurrentParameterPage("Unison", "On");
    // digitalWriteFast(UNISON_LED, HIGH);  // LED on
  }
}

void updateVolume(float vol)
{
  showCurrentParameterPage("Volume", vol);
}

void updateGlide() {
  if (glideSpeed * GLIDEFACTOR < 1000) {
    showCurrentParameterPage("Glide", String(int(glideSpeed * GLIDEFACTOR)) + " ms");
  }
  else {
    showCurrentParameterPage("Glide", String((glideSpeed * GLIDEFACTOR) / 1000) + " s");
  }
}

void updateWaveformA()
{
  int newWaveform = oscWaveformA;//To allow Arbitrary waveforms
  if (oscWaveformA == WAVEFORM_PARABOLIC) {
    loadArbWaveformA(PARABOLIC_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }
  if (oscWaveformA == WAVEFORM_HARMONIC) {
    loadArbWaveformA(HARMONIC_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }
  if (oscWaveformA == WAVEFORM_SAWTOOTH_WT || oscWaveformA == WAVEFORM_SQUARE_WT) {
    //Wavetable is loaded on each new note
    updatesAllVoices();
    newWaveform = WAVEFORM_ARBITRARY;
  }
  waveformMod1a.begin(newWaveform);
  waveformMod2a.begin(newWaveform);
  waveformMod3a.begin(newWaveform);
  waveformMod4a.begin(newWaveform);
  waveformMod5a.begin(newWaveform);
  waveformMod6a.begin(newWaveform);
  showCurrentParameterPage("1. Waveform", getWaveformStr(oscWaveformA));
}

void updateWaveformB()
{
  int newWaveform = oscWaveformB;//To allow Arbitrary waveforms
  if (oscWaveformB == WAVEFORM_PARABOLIC) {
    loadArbWaveformB(PARABOLIC_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }
  if (oscWaveformB == WAVEFORM_HARMONIC) {
    loadArbWaveformB(PPG_WAVE);
    newWaveform = WAVEFORM_ARBITRARY;
  }
  if (oscWaveformB == WAVEFORM_SAWTOOTH_WT || oscWaveformB == WAVEFORM_SQUARE_WT) {
    //Wavetable is loaded on each new note
    updatesAllVoices();
    newWaveform = WAVEFORM_ARBITRARY;
  }
  waveformMod1b.begin(newWaveform);
  waveformMod2b.begin(newWaveform);
  waveformMod3b.begin(newWaveform);
  waveformMod4b.begin(newWaveform);
  waveformMod5b.begin(newWaveform);
  waveformMod6b.begin(newWaveform);
  showCurrentParameterPage("2. Waveform", getWaveformStr(oscWaveformB));
}

void updatePitchA() {
  updatesAllVoices();
  showCurrentParameterPage("1. Semitones", (oscPitchA > 0 ? "+" : "") + String(oscPitchA));
}

void updatePitchB() {
  updatesAllVoices();
  showCurrentParameterPage("2. Semitones", (oscPitchB > 0 ? "+" : "") + String(oscPitchB));
}

void updateDetune() {
  updatesAllVoices();
  showCurrentParameterPage("Detune", String((1 - detune) * 100) + " %");
}

void updatesAllVoices() {
  updateVoice1();
  updateVoice2();
  updateVoice3();
  updateVoice4();
  updateVoice5();
  updateVoice6();
}

void updatePWMSource() {
  if (pwmSource == PWMSOURCELFO) {
    setPwmMixerAFEnv(0);
    setPwmMixerBFEnv(0);
    if (pwmRate > -5) {
      setPwmMixerALFO(pwmAmtA);
      setPwmMixerBLFO(pwmAmtB);
    }
    showCurrentParameterPage("PWM Source", "LFO"); //Only shown when updated via MIDI
  } else {
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    if (pwmRate > -5) {
      setPwmMixerAFEnv(pwmAmtA);
      setPwmMixerBFEnv(pwmAmtB);
    }
    showCurrentParameterPage("PWM Source", "Filter Env");
  }
}

void updatePWMRate()
{
  pwmLfo.frequency(pwmRate);
  if (pwmRate == -10) {
    //Set to fixed PW mode
    setPwmMixerALFO(0);//LFO Source off
    setPwmMixerBLFO(0);
    setPwmMixerAFEnv(0);//Filter Env Source off
    setPwmMixerBFEnv(0);
    setPwmMixerAPW(1);//Manually adjustable pulse width on
    setPwmMixerBPW(1);
    showCurrentParameterPage("PW Mode", "On");
  } else if (pwmRate == -5) {
    //Set to Filter Env Mod source
    pwmSource = PWMSOURCEFENV;
    updatePWMSource();
    //Filter env mod - pwmRate does nothing
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    setPwmMixerAPW(0);
    setPwmMixerBPW(0);
    setPwmMixerAFEnv(pwmAmtA);
    setPwmMixerBFEnv(pwmAmtB);
    showCurrentParameterPage("PWM Source", "Filter Env");
  } else {
    pwmSource = PWMSOURCELFO;
    updatePWMSource();
    setPwmMixerAPW(0);
    setPwmMixerBPW(0);
    showCurrentParameterPage("PWM Rate", String(pwmRate) + " Hz");
  }
}

void updatePWMAmount()
{
  //MIDI only - sets both osc
  pwA = 0;
  pwB = 0;
  setPwmMixerALFO(pwmAmtA);
  setPwmMixerBLFO(pwmAmtB);
  showCurrentParameterPage("PWM Amt", String(pwmAmtA) + " " + String(pwmAmtB));
}

void updatePWA()
{
  if (pwmRate == -10) {
    //if PWM amount is around zero, fixed PW is enabled
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    setPwmMixerAFEnv(0);
    setPwmMixerBFEnv(0);
    setPwmMixerAPW(1);
    setPwmMixerBPW(1);
    if (oscWaveformA == WAVEFORM_TRIANGLE_VARIABLE)
    {
      showCurrentParameterPage("1. PW Amt", pwA);
    }
    else
    {
      showCurrentParameterPage("1. PW Amt", pwA);
    }
  }
  else
  {
    setPwmMixerAPW(0);
    setPwmMixerBPW(0);
    if (pwmSource == PWMSOURCELFO)
    {
      //PW alters PWM LFO amount for waveform A
      setPwmMixerALFO(pwmAmtA);
      showCurrentParameterPage("1. PWM Amt", "LFO " + String(pwmAmtA));
    }
    else
    {
      //PW alters PWM Filter Env amount for waveform A
      setPwmMixerAFEnv(pwmAmtA);
      showCurrentParameterPage("1. PWM Amt", "F. Env " + String(pwmAmtA));
    }
  }
  float pwA_Adj = pwA;//Prevent silence when pw = +/-1 on pulse
  if (pwA > 0.98) pwA_Adj = 0.98f;
  if (pwA < -0.98) pwA_Adj = -0.98f;
  pwa.amplitude(pwA_Adj);
}

void updatePWB()
{
  if (pwmRate == -10) {
    //if PWM amount is around zero, fixed PW is enabled
    setPwmMixerALFO(0);
    setPwmMixerBLFO(0);
    setPwmMixerAFEnv(0);
    setPwmMixerBFEnv(0);
    setPwmMixerAPW(1);
    setPwmMixerBPW(1);
    if (oscWaveformB == WAVEFORM_TRIANGLE_VARIABLE) {
      showCurrentParameterPage("2. PW Amt", pwB);
    } else {
      showCurrentParameterPage("2. PW Amt", pwB);
    }
  } else {
    setPwmMixerAPW(0);
    setPwmMixerBPW(0);
    if (pwmSource == PWMSOURCELFO)
    {
      //PW alters PWM LFO amount for waveform B
      setPwmMixerBLFO(pwmAmtB);
      showCurrentParameterPage("2. PWM Amt", "LFO " + String(pwmAmtB));
    } else {
      //PW alters PWM Filter Env amount for waveform B
      setPwmMixerBFEnv(pwmAmtB);
      showCurrentParameterPage("2. PWM Amt", "F. Env " + String(pwmAmtB));
    }
  }
  float pwB_Adj = pwB;//Prevent silence when pw = +/-1 on pulse
  if (pwB > 0.98) pwB_Adj = 0.98f;
  if (pwB < -0.98) pwB_Adj = -0.98f;
  pwb.amplitude(pwB_Adj);
}

void updateOscLevelA()
{
  waveformMixer1.gain(0, oscALevel);
  waveformMixer2.gain(0, oscALevel);
  waveformMixer3.gain(0, oscALevel);
  waveformMixer4.gain(0, oscALevel);
  waveformMixer5.gain(0, oscALevel);
  waveformMixer6.gain(0, oscALevel);
  if (oscFX == 1)
  {
    waveformMixer1.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer2.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer3.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer4.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer5.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer6.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
  }
  showCurrentParameterPage("Osc Levels", "   " + String(oscALevel) + " - " + String(oscBLevel));
}

void updateOscLevelB()
{
  waveformMixer1.gain(1, oscBLevel);
  waveformMixer2.gain(1, oscBLevel);
  waveformMixer3.gain(1, oscBLevel);
  waveformMixer4.gain(1, oscBLevel);
  waveformMixer5.gain(1, oscBLevel);
  waveformMixer6.gain(1, oscBLevel);
  if (oscFX == 1)
  {
    waveformMixer1.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer2.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer3.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer4.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer5.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
    waveformMixer6.gain(3, (oscALevel + oscBLevel) / 2.0f); //Osc FX
  }
  showCurrentParameterPage("Osc Levels", "   " + String(oscALevel) + " - " + String(oscBLevel));
}

void updateNoiseLevel()
{
  if (noiseLevel > 0)
  {
    pink.amplitude(noiseLevel);
    white.amplitude(0.0f);
    showCurrentParameterPage("Noise Level", "Pink " + String(noiseLevel));
  }
  else if (noiseLevel < 0)
  {
    pink.amplitude(0.0f);
    white.amplitude(abs(noiseLevel));
    showCurrentParameterPage("Noise Level", "White " + String(abs(noiseLevel)));
  }
  else
  {
    pink.amplitude(noiseLevel);
    white.amplitude(noiseLevel);
    showCurrentParameterPage("Noise Level", "Off");
  }
}

void updateFilterFreq()
{
  filter1.frequency(filterFreq);
  filter2.frequency(filterFreq);
  filter3.frequency(filterFreq);
  filter4.frequency(filterFreq);
  filter5.frequency(filterFreq);
  filter6.frequency(filterFreq);

  //Altering filterOctave to give more cutoff width for deeper bass, but sharper cuttoff at higher frequncies
  if (filterFreq <= 2000) {
    filterOctave = 4.0f + ((2000.0f - filterFreq) / 710.0f);//More bass
  } else if (filterFreq > 2000 && filterFreq <= 3500) {
    filterOctave = 3.0f + ((3500.0f - filterFreq) / 1500.0f);//Sharper cutoff
  } else if (filterFreq > 3500 && filterFreq <= 7000) {
    filterOctave = 2.0f + ((7000.0f - filterFreq) / 4000.0f);//Sharper cutoff
  } else {
    filterOctave = 1.0f + ((12000.0f - filterFreq) / 5100.0f);//Sharper cutoff
  }

  filter1.octaveControl(filterOctave);
  filter2.octaveControl(filterOctave);
  filter3.octaveControl(filterOctave);
  filter4.octaveControl(filterOctave);
  filter5.octaveControl(filterOctave);
  filter6.octaveControl(filterOctave);

  showCurrentParameterPage("Cutoff", String(int(filterFreq)) + " Hz");
}

void updateFilterRes()
{
  filter1.resonance(filterRes);
  filter2.resonance(filterRes);
  filter3.resonance(filterRes);
  filter4.resonance(filterRes);
  filter5.resonance(filterRes);
  filter6.resonance(filterRes);
  showCurrentParameterPage("Resonance", filterRes);
}

void updateFilterMixer()
{
  float LP = 1.0f;
  float BP = 0;
  float HP = 0;
  String filterStr;
  if (filterMix == LINEAR_FILTERMIXER[127])
  {
    //BP mode
    LP = 0;
    BP = 1.0f;
    HP = 0;
    filterStr = "Band Pass";
  }
  else
  {
    //LP-HP mix mode - a notch filter
    LP = 1.0f - filterMix;
    BP = 0;
    HP = filterMix;
    if (filterMix == LINEAR_FILTERMIXER[0])
    {
      filterStr = "Low Pass";
    }
    else if (filterMix == LINEAR_FILTERMIXER[125])
    {
      filterStr = "High Pass";
    }
    else
    {
      filterStr = "LP " + String(100 - filterMixStr) + " - " + String(filterMixStr) + " HP";
    }
  }
  filterMixer1.gain(0, LP);
  filterMixer1.gain(1, BP);
  filterMixer1.gain(2, HP);
  filterMixer2.gain(0, LP);
  filterMixer2.gain(1, BP);
  filterMixer2.gain(2, HP);
  filterMixer3.gain(0, LP);
  filterMixer3.gain(1, BP);
  filterMixer3.gain(2, HP);
  filterMixer4.gain(0, LP);
  filterMixer4.gain(1, BP);
  filterMixer4.gain(2, HP);
  filterMixer5.gain(0, LP);
  filterMixer5.gain(1, BP);
  filterMixer5.gain(2, HP);
  filterMixer6.gain(0, LP);
  filterMixer6.gain(1, BP);
  filterMixer6.gain(2, HP);
  showCurrentParameterPage("Filter Type", filterStr);
}

void updateFilterEnv()
{
  filterModMixer1.gain(0, filterEnv);
  filterModMixer2.gain(0, filterEnv);
  filterModMixer3.gain(0, filterEnv);
  filterModMixer4.gain(0, filterEnv);
  filterModMixer5.gain(0, filterEnv);
  filterModMixer6.gain(0, filterEnv);
  showCurrentParameterPage("Filter Env.", String(filterEnv));
}

void updatePitchEnv()
{
  oscModMixer1a.gain(1, pitchEnv);
  oscModMixer1b.gain(1, pitchEnv);
  oscModMixer2a.gain(1, pitchEnv);
  oscModMixer2b.gain(1, pitchEnv);
  oscModMixer3a.gain(1, pitchEnv);
  oscModMixer3b.gain(1, pitchEnv);
  oscModMixer4a.gain(1, pitchEnv);
  oscModMixer4b.gain(1, pitchEnv);
  oscModMixer5a.gain(1, pitchEnv);
  oscModMixer5b.gain(1, pitchEnv);
  oscModMixer6a.gain(1, pitchEnv);
  oscModMixer6b.gain(1, pitchEnv);
  showCurrentParameterPage("Pitch Env Amt", String(pitchEnv));
}

void updateKeyTracking()
{
  filterModMixer1.gain(2, keytrackingAmount);
  filterModMixer2.gain(2, keytrackingAmount);
  filterModMixer3.gain(2, keytrackingAmount);
  filterModMixer4.gain(2, keytrackingAmount);
  filterModMixer5.gain(2, keytrackingAmount);
  filterModMixer6.gain(2, keytrackingAmount);
  showCurrentParameterPage("Key Tracking", String(keytrackingAmount));
}

void updateOscLFOAmt()
{
  pitchLfo.amplitude(oscLfoAmt + modWhAmt);
  char buf[10];
  showCurrentParameterPage("LFO Amount", dtostrf(oscLfoAmt, 4, 3, buf));
}

void updateModWheel()
{
  pitchLfo.amplitude(oscLfoAmt + modWhAmt);
}

void updatePitchLFORate()
{
  pitchLfo.frequency(oscLfoRate);
  showCurrentParameterPage("LFO Rate", String(oscLfoRate) + " Hz");
}

void updatePitchLFOWaveform()
{
  pitchLfo.begin(oscLFOWaveform);
  showCurrentParameterPage("Pitch LFO", getWaveformStr(oscLFOWaveform));
}

//MIDI CC only
void updatePitchLFOMidiClkSync()
{
  showCurrentParameterPage("P. LFO Sync", oscLFOMidiClkSync == 1 ? "On" : "Off");
}

void updateFilterLfoRate()
{
  filterLfo.frequency(filterLfoRate);
  if (filterLFOMidiClkSync)
  {
    showCurrentParameterPage("LFO Time Div", filterLFOTimeDivStr);
  }
  else
  {
    showCurrentParameterPage("F. LFO Rate", String(filterLfoRate) + " Hz");
  }
}

void updateFilterLfoAmt()
{
  filterLfo.amplitude(filterLfoAmt);
  showCurrentParameterPage("F. LFO Amt", String(filterLfoAmt));
}

void updateFilterLFOWaveform()
{
  filterLfo.begin(filterLfoWaveform);
  showCurrentParameterPage("Filter LFO", getWaveformStr(filterLfoWaveform));
}

void updatePitchLFORetrig()
{
  showCurrentParameterPage("P. LFO Retrig", oscLfoRetrig == 1 ? "On" : "Off");
}

void updateFilterLFORetrig()
{
  showCurrentParameterPage("F. LFO Retrig", filterLfoRetrig == 1 ? "On" : "Off");
  // digitalWriteFast(RETRIG_LED, filterLfoRetrig == 1 ? HIGH : LOW);  // LED
}

void updateFilterLFOMidiClkSync()
{
  showCurrentParameterPage("Tempo Sync", filterLFOMidiClkSync == 1 ? "On" : "Off");
  // digitalWriteFast(TEMPO_LED, filterLFOMidiClkSync == 1 ? HIGH : LOW);  // LED
}

void updateFilterAttack()
{
  filterEnvelope1.attack(filterAttack);
  filterEnvelope2.attack(filterAttack);
  filterEnvelope3.attack(filterAttack);
  filterEnvelope4.attack(filterAttack);
  filterEnvelope5.attack(filterAttack);
  filterEnvelope6.attack(filterAttack);
  if (filterAttack < 1000)
  {
    showCurrentParameterPage("Filter Attack", String(int(filterAttack)) + " ms");
  }
  else
  {
    showCurrentParameterPage("Filter Attack", String(filterAttack * 0.001) + " s");
  }
}

void updateFilterDecay()
{
  filterEnvelope1.decay(filterDecay);
  filterEnvelope2.decay(filterDecay);
  filterEnvelope3.decay(filterDecay);
  filterEnvelope4.decay(filterDecay);
  filterEnvelope5.decay(filterDecay);
  filterEnvelope6.decay(filterDecay);
  if (filterDecay < 1000)
  {
    showCurrentParameterPage("Filter Decay", String(int(filterDecay)) + " ms");
  }
  else
  {
    showCurrentParameterPage("Filter Decay", String(filterDecay * 0.001) + " s");
  }
}

void updateFilterSustain()
{
  filterEnvelope1.sustain(filterSustain);
  filterEnvelope2.sustain(filterSustain);
  filterEnvelope3.sustain(filterSustain);
  filterEnvelope4.sustain(filterSustain);
  filterEnvelope5.sustain(filterSustain);
  filterEnvelope6.sustain(filterSustain);
  showCurrentParameterPage("Filter Sustain", String(filterSustain));
}

void updateFilterRelease()
{
  filterEnvelope1.release(filterRelease);
  filterEnvelope2.release(filterRelease);
  filterEnvelope3.release(filterRelease);
  filterEnvelope4.release(filterRelease);
  filterEnvelope5.release(filterRelease);
  filterEnvelope6.release(filterRelease);
  if (filterRelease < 1000)
  {
    showCurrentParameterPage("Filter Release", String(int(filterRelease)) + " ms");
  }
  else
  {
    showCurrentParameterPage("Filter Release", String(filterRelease * 0.001) + " s");
  }
}

void updateAttack()
{
  ampEnvelope1.attack(ampAttack);
  ampEnvelope2.attack(ampAttack);
  ampEnvelope3.attack(ampAttack);
  ampEnvelope4.attack(ampAttack);
  ampEnvelope5.attack(ampAttack);
  ampEnvelope6.attack(ampAttack);
  if (ampAttack < 1000)
  {
    showCurrentParameterPage("Attack", String(int(ampAttack)) + " ms");
  }
  else
  {
    showCurrentParameterPage("Attack", String(ampAttack * 0.001) + " s");
  }
}

void updateDecay()
{
  ampEnvelope1.decay(ampDecay);
  ampEnvelope2.decay(ampDecay);
  ampEnvelope3.decay(ampDecay);
  ampEnvelope4.decay(ampDecay);
  ampEnvelope5.decay(ampDecay);
  ampEnvelope6.decay(ampDecay);
  if (ampDecay < 1000)
  {
    showCurrentParameterPage("Decay", String(int(ampDecay)) + " ms");
  }
  else
  {
    showCurrentParameterPage("Decay", String(ampDecay * 0.001) + " s");
  }
}

void updateSustain()
{
  ampEnvelope1.sustain(ampSustain);
  ampEnvelope2.sustain(ampSustain);
  ampEnvelope3.sustain(ampSustain);
  ampEnvelope4.sustain(ampSustain);
  ampEnvelope5.sustain(ampSustain);
  ampEnvelope6.sustain(ampSustain);
  showCurrentParameterPage("Sustain", String(ampSustain));
}

void updateRelease()
{
  ampEnvelope1.release(ampRelease);
  ampEnvelope2.release(ampRelease);
  ampEnvelope3.release(ampRelease);
  ampEnvelope4.release(ampRelease);
  ampEnvelope5.release(ampRelease);
  ampEnvelope6.release(ampRelease);
  if (ampRelease < 1000)
  {
    showCurrentParameterPage("Release", String(int(ampRelease)) + " ms");
  }
  else
  {
    showCurrentParameterPage("Release", String(ampRelease * 0.001) + " s");
  }
}

void updateOscFX()
{
  if (oscFX == 1)
  {
    oscFX1.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX2.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX3.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX4.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX5.setCombineMode(AudioEffectDigitalCombine::XOR);
    oscFX6.setCombineMode(AudioEffectDigitalCombine::XOR);
    waveformMixer1.gain(3, (oscALevel + oscBLevel) / 2.0); //Osc FX
    waveformMixer2.gain(3, (oscALevel + oscBLevel) / 2.0); //Osc FX
    waveformMixer3.gain(3, (oscALevel + oscBLevel) / 2.0); //Osc FX
    waveformMixer4.gain(3, (oscALevel + oscBLevel) / 2.0); //Osc FX
    waveformMixer5.gain(3, (oscALevel + oscBLevel) / 2.0); //Osc FX
    waveformMixer6.gain(3, (oscALevel + oscBLevel) / 2.0); //Osc FX
    showCurrentParameterPage("Osc FX", "On");
    // riteFast(OSC_FX_LED, HIGH);  // LED on
  }
  else
  {
    oscFX1.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX2.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX3.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX4.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX5.setCombineMode(AudioEffectDigitalCombine::OFF);
    oscFX6.setCombineMode(AudioEffectDigitalCombine::OFF);
    waveformMixer1.gain(3, 0); //Osc FX
    waveformMixer2.gain(3, 0); //Osc FX
    waveformMixer3.gain(3, 0); //Osc FX
    waveformMixer4.gain(3, 0); //Osc FX
    waveformMixer5.gain(3, 0); //Osc FX
    waveformMixer6.gain(3, 0); //Osc FX
    showCurrentParameterPage("Osc FX", "Off");
    // digitalWriteFast(OSC_FX_LED, LOW);  // LED off
  }
}

void updateFXAmt()
{
  ensemble.lfoRate(fxAmt);
  showCurrentParameterPage("Effect Amt", String(fxAmt) + " Hz");
}

void updateFXMix()
{
  effectMixerL.gain(0, 1.0 - fxMix); //Dry
  effectMixerL.gain(1, fxMix);       //Wet
  effectMixerR.gain(0, 1.0 - fxMix); //Dry
  effectMixerR.gain(1, fxMix);       //Wet
  showCurrentParameterPage("Effect Mix", String(fxMix));
}

void updatePatchname()
{
  showCurrentParameterPage(String(patchNo), patchName);
}

void myPitchBend(byte channel, int bend)
{
  pitchBend.amplitude(bend * 0.5 * pitchBendRange * DIV12 * DIV8192); //)0.5 to give 1oct max - spread of mod is 2oct
}

void myNoteOn(byte channel, byte note, byte velocity)
{
  //Check for out of range notes
  if (note + oscPitchA < 0 || note + oscPitchA > 127 || note + oscPitchB < 0 || note + oscPitchB > 127)
    return;

  if (oscLfoRetrig == 1)
  {
    pitchLfo.sync();
  }
  if (filterLfoRetrig == 1)
  {
    filterLfo.sync();
  }

  if (unison == 0)
  {
    switch (getVoiceNo(-1))
    {
      case 1:
        keytracking1.amplitude(note * DIV127 * keytrackingAmount);
        voices[0].note = note;
        voices[0].timeOn = millis();
        voiceMixer1.gain(0, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
        updateVoice1();
        filterEnvelope1.noteOn();
        ampEnvelope1.noteOn();
        voices[0].voiceOn = true;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide1.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
          glide1.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        prevNote = note;
        break;
      case 2:
        keytracking2.amplitude(note * DIV127 * keytrackingAmount);
        voices[1].note = note;
        voices[1].timeOn = millis();
        voiceMixer1.gain(1, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
        updateVoice2();
        filterEnvelope2.noteOn();
        ampEnvelope2.noteOn();
        voices[1].voiceOn = true;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide2.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
          glide2.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        prevNote = note;
        break;
      case 3:
        keytracking3.amplitude(note * DIV127 * keytrackingAmount);
        voices[2].note = note;
        voices[2].timeOn = millis();
        voiceMixer1.gain(2, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
        updateVoice3();
        filterEnvelope3.noteOn();
        ampEnvelope3.noteOn();
        voices[2].voiceOn = true;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide3.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
          glide3.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        prevNote = note;
        break;
      case 4:
        keytracking4.amplitude(note * DIV127 * keytrackingAmount);
        voices[3].note = note;
        voices[3].timeOn = millis();
        voiceMixer2.gain(0, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
        updateVoice4();
        filterEnvelope4.noteOn();
        ampEnvelope4.noteOn();
        voices[3].voiceOn = true;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide4.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
          glide4.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        prevNote = note;
        break;
      case 5:
        keytracking5.amplitude(note * DIV127 * keytrackingAmount);
        voices[4].note = note;
        voices[4].timeOn = millis();
        voiceMixer2.gain(1, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
        updateVoice5();
        filterEnvelope5.noteOn();
        ampEnvelope5.noteOn();
        voices[4].voiceOn = true;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide5.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
          glide5.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        prevNote = note;
        break;
      case 6:
        keytracking6.amplitude(note * DIV127 * keytrackingAmount);
        voices[5].note = note;
        voices[5].timeOn = millis();
        voiceMixer2.gain(2, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
        updateVoice6();
        filterEnvelope6.noteOn();
        ampEnvelope6.noteOn();
        voices[5].voiceOn = true;
        if (glideSpeed > 0 && note != prevNote)
        {
          glide6.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
          glide6.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
        }
        prevNote = note;
        break;
    }
  }
  else
  {
    //UNISON MODE
    keytracking1.amplitude(note * DIV127 * keytrackingAmount);
    keytracking2.amplitude(note * DIV127 * keytrackingAmount);
    keytracking3.amplitude(note * DIV127 * keytrackingAmount);
    keytracking4.amplitude(note * DIV127 * keytrackingAmount);
    keytracking5.amplitude(note * DIV127 * keytrackingAmount);
    keytracking6.amplitude(note * DIV127 * keytrackingAmount);
    voices[0].note = note;
    voices[0].timeOn = millis();
    voiceMixer1.gain(0, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
    updateVoice1();
    voices[1].note = note;
    voices[1].timeOn = millis();
    voiceMixer1.gain(1, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
    updateVoice2();
    voices[2].note = note;
    voices[2].timeOn = millis();
    voiceMixer1.gain(2, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
    updateVoice3();
    voices[3].note = note;
    voices[3].timeOn = millis();
    voiceMixer2.gain(0, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
    updateVoice4();
    voices[4].note = note;
    voices[4].timeOn = millis();
    voiceMixer2.gain(1, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
    updateVoice5();
    voices[5].note = note;
    voices[5].timeOn = millis();
    voiceMixer2.gain(2, VELOCITY[velocitySens][velocity] * VOICEMIXERLEVEL);
    updateVoice6();

    filterEnvelope1.noteOn();
    filterEnvelope2.noteOn();
    filterEnvelope3.noteOn();
    filterEnvelope4.noteOn();
    filterEnvelope5.noteOn();
    filterEnvelope6.noteOn();

    ampEnvelope1.noteOn();
    ampEnvelope2.noteOn();
    ampEnvelope3.noteOn();
    ampEnvelope4.noteOn();
    ampEnvelope5.noteOn();
    ampEnvelope6.noteOn();

    voices[0].voiceOn = true;
    voices[1].voiceOn = true;
    voices[2].voiceOn = true;
    voices[3].voiceOn = true;
    voices[4].voiceOn = true;
    voices[5].voiceOn = true;

    if (glideSpeed > 0 && note != prevNote)
    {
      glide1.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
      glide1.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide2.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
      glide2.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide3.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
      glide3.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide4.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
      glide4.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide5.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
      glide5.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
      glide6.amplitude((prevNote - note) * DIV24);   //Set glide to previous note frequency (limited to 1 octave max)
      glide6.amplitude(0, glideSpeed * GLIDEFACTOR); //Glide to current note
    }
    prevNote = note;
  }
}

void myNoteOff(byte channel, byte note, byte velocity)
{
  if (unison == 0)
  {
    switch (getVoiceNo(note))
    {
      case 1:
        filterEnvelope1.noteOff();
        ampEnvelope1.noteOff();
        voices[0].voiceOn = false;
        break;
      case 2:
        filterEnvelope2.noteOff();
        ampEnvelope2.noteOff();
        voices[1].voiceOn = false;
        break;
      case 3:
        filterEnvelope3.noteOff();
        ampEnvelope3.noteOff();
        voices[2].voiceOn = false;
        break;
      case 4:
        filterEnvelope4.noteOff();
        ampEnvelope4.noteOff();
        voices[3].voiceOn = false;
        break;
      case 5:
        filterEnvelope5.noteOff();
        ampEnvelope5.noteOff();
        voices[4].voiceOn = false;
        break;
      case 6:
        filterEnvelope6.noteOff();
        ampEnvelope6.noteOff();
        voices[5].voiceOn = false;
        break;
    }
  }
  else
  {
    //UNISON MODE
    //If statement prevents the previous different note
    //ending the current note when released
    if (voices[0].note == note)allNotesOff();
  }
}

void allNotesOff()
{
  filterEnvelope1.noteOff();
  ampEnvelope1.noteOff();
  filterEnvelope2.noteOff();
  ampEnvelope2.noteOff();
  filterEnvelope3.noteOff();
  ampEnvelope3.noteOff();
  filterEnvelope4.noteOff();
  ampEnvelope4.noteOff();
  filterEnvelope5.noteOff();
  ampEnvelope5.noteOff();
  filterEnvelope6.noteOff();
  ampEnvelope6.noteOff();

  voices[0].voiceOn = false;
  voices[1].voiceOn = false;
  voices[2].voiceOn = false;
  voices[3].voiceOn = false;
  voices[4].voiceOn = false;
  voices[5].voiceOn = false;
}

void TsynthHandlePotards(byte inputIndex, unsigned int value, int diffToPrevious){
  myControlChange(midiChannel, CCarray[int(inputIndex)], value);
}

void myControlChange(byte channel, byte control, byte value)
{
  DisplayParamMsec = 0;
  //Serial.println("MIDI: " + String(control) + " : " + String(value));
  switch (control)
  {
    case CCvolume:
      volumeMixer.gain(0, LINEAR[value]);
      updateVolume(LINEAR[value]);
      break;

    case CCunison:
      value > 0 ? unison = 1 : unison = 0;
      updateUnison();
      break;

    case CCglide:
      glideSpeed = POWER[value];
      updateGlide();
      break;

    case CCpitchenv:
      pitchEnv = LINEARCENTREZERO[value] * OSCMODMIXERMAX;
      updatePitchEnv();
      break;

    case CCoscwaveformA:
      if (oscWaveformA == getWaveformA(value))return;
      oscWaveformA = getWaveformA(value);
      updateWaveformA();
      break;

    case CCoscwaveformB:
      if (oscWaveformB == getWaveformB(value))return;
      oscWaveformB = getWaveformB(value);
      updateWaveformB();
      break;

    case CCpitchA:
      oscPitchA = getPitch(value);
      updatePitchA();
      break;

    case CCpitchB:
      oscPitchB = getPitch(value);
      updatePitchB();
      break;

    case CCdetune:
      detune = 1 - (MAXDETUNE * LINEAR[value]);
      updateDetune();
      break;

    case CCpwmSource:
      value > 0 ? pwmSource = PWMSOURCEFENV : pwmSource = PWMSOURCELFO;
      updatePWMSource();
      break;

    case CCpwmRate:
      //Uses combination of PWMRate, PWa and PWb
      pwmRate = PWMRATE[value];
      updatePWMRate();
      break;

    case CCpwmAmt:
      //NO FRONT PANEL CONTROL - MIDI CC ONLY
      //Total PWM amount for both oscillators
      pwmAmtA = LINEAR[value];
      pwmAmtB = LINEAR[value];
      updatePWMAmount();
      break;

    case CCpwA:
      pwA = LINEARCENTREZERO[value]; //Bipolar
      pwmAmtA = LINEAR[value];
      updatePWA();
      break;

    case CCpwB:
      pwB = LINEARCENTREZERO[value]; //Bipolar
      pwmAmtB = LINEAR[value];
      updatePWB();
      break;

    case CCoscLevelA:
      oscALevel = LINEAR[value];
      updateOscLevelA();
      break;

    case CCoscLevelB:
      oscBLevel = LINEAR[value];
      updateOscLevelB();
      break;

    case CCnoiseLevel:
      noiseLevel = LINEARCENTREZERO[value];
      updateNoiseLevel();
      break;

    case CCfilterfreq:
      //Pick up
      //MIDI is 7 bit and needs to choose alternate filterfreqs(8 bit)
      if (!pickUpActive && pickUp && (filterfreqPrevValue <  FILTERFREQS256[(value - TOLERANCE) * 2] || filterfreqPrevValue >  FILTERFREQS256[(value - TOLERANCE) * 2])) return; //PICK-UP
      filterFreq = FILTERFREQS256[value * 2];
      updateFilterFreq();
      filterfreqPrevValue = filterFreq;//PICK-UP
      break;

    case CCfilterres:
      //Pick up
      if (!pickUpActive && pickUp && (resonancePrevValue <  ((13.9f * POWER[value - TOLERANCE]) + 1.1f) || resonancePrevValue >  ((13.9f * POWER[value + TOLERANCE]) + 1.1f))) return; //PICK-UP
      filterRes = (13.9f * POWER[value]) + 1.1f; //If <1.1 there is noise at high cutoff freq
      updateFilterRes();
      resonancePrevValue = filterRes;//PICK-UP
      break;

    case CCfiltermixer:
      //Pick up
      if (!pickUpActive && pickUp && (filterMixPrevValue <  LINEAR_FILTERMIXER[value - TOLERANCE] || filterMixPrevValue >  LINEAR_FILTERMIXER[value + TOLERANCE])) return; //PICK-UP
      filterMix = LINEAR_FILTERMIXER[value];
      filterMixStr = LINEAR_FILTERMIXERSTR[value];
      updateFilterMixer();
      filterMixPrevValue = filterMix;//PICK-UP
      break;

    case CCfilterenv:
      filterEnv = LINEARCENTREZERO[value] * FILTERMODMIXERMAX; //Bipolar
      updateFilterEnv();
      break;

    case CCkeytracking:
      keytrackingAmount = KEYTRACKINGAMT[value];
      updateKeyTracking();
      break;

    case CCmodwheel:
      modWhAmt = POWER[value] * modWheelDepth; //Variable LFO amount from mod wheel - Settings Option
      updateModWheel();
      break;

    case CCosclfoamt:
      //Pick up
      if (!pickUpActive && pickUp && (oscLfoAmtPrevValue <  POWER[value - TOLERANCE] || oscLfoAmtPrevValue >  POWER[value + TOLERANCE])) return; //PICK-UP
      oscLfoAmt = POWER[value];
      updateOscLFOAmt();
      oscLfoAmtPrevValue = oscLfoAmt;//PICK-UP
      break;

    case CCoscLfoRate:
      //Pick up
      if (!pickUpActive && pickUp && (oscLfoRatePrevValue <  LFOMAXRATE * POWER[value - TOLERANCE] || oscLfoRatePrevValue > LFOMAXRATE * POWER[value + TOLERANCE])) return; //PICK-UP
      if (oscLFOMidiClkSync == 1) {
        oscLfoRate = getLFOTempoRate(value);
        oscLFOTimeDivStr = LFOTEMPOSTR[value];
      }
      else {
        oscLfoRate = LFOMAXRATE * POWER[value];
      }
      updatePitchLFORate();
      oscLfoRatePrevValue = oscLfoRate;//PICK-UP
      break;

    case CCoscLfoWaveform:
      oscLFOWaveform = getLFOWaveform(value);
      updatePitchLFOWaveform();
      break;

    case CCosclforetrig:
      value > 0 ? oscLfoRetrig = 1 : oscLfoRetrig = 0;
      updatePitchLFORetrig();
      break;

    case CCfilterLFOMidiClkSync:
      value > 0 ? filterLFOMidiClkSync = 1 : filterLFOMidiClkSync = 0;
      updateFilterLFOMidiClkSync();
      break;

    case CCfilterlforate:
      //Pick up
      if (!pickUpActive && pickUp && (filterLfoRatePrevValue <  LFOMAXRATE * POWER[value - TOLERANCE] || filterLfoRatePrevValue > LFOMAXRATE * POWER[value + TOLERANCE])) return; //PICK-UP
      if (filterLFOMidiClkSync == 1) {
        filterLfoRate = getLFOTempoRate(value);
        filterLFOTimeDivStr = LFOTEMPOSTR[value];
      } else {
        filterLfoRate = LFOMAXRATE * POWER[value];
      }
      updateFilterLfoRate();
      filterLfoRatePrevValue = filterLfoRate;//PICK-UP
      break;

    case CCfilterlfoamt:
      //Pick up
      if (!pickUpActive && pickUp && (filterLfoAmtPrevValue <  LINEAR[value - TOLERANCE] * FILTERMODMIXERMAX || filterLfoAmtPrevValue >  LINEAR[value + TOLERANCE] * FILTERMODMIXERMAX)) return; //PICK-UP
      filterLfoAmt = LINEAR[value] * FILTERMODMIXERMAX;
      updateFilterLfoAmt();
      filterLfoAmtPrevValue = filterLfoAmt;//PICK-UP
      break;

    case CCfilterlfowaveform:
      filterLfoWaveform = getLFOWaveform(value);
      updateFilterLFOWaveform();
      break;

    case CCfilterlforetrig:
      value > 0 ? filterLfoRetrig = 1 : filterLfoRetrig = 0;
      updateFilterLFORetrig();
      break;

    //MIDI Only
    case CCoscLFOMidiClkSync:
      value > 0 ? oscLFOMidiClkSync = 1 : oscLFOMidiClkSync = 0;
      updatePitchLFOMidiClkSync();
      break;

    case CCfilterattack:
      filterAttack = ENVTIMES[value];
      updateFilterAttack();
      break;

    case CCfilterdecay:
      filterDecay = ENVTIMES[value];
      updateFilterDecay();
      break;

    case CCfiltersustain:
      filterSustain = LINEAR[value];
      updateFilterSustain();
      break;

    case CCfilterrelease:
      filterRelease = ENVTIMES[value];
      updateFilterRelease();
      break;

    case CCampattack:
      ampAttack = ENVTIMES[value];
      updateAttack();
      break;

    case CCampdecay:
      ampDecay = ENVTIMES[value];
      updateDecay();
      break;

    case CCampsustain:
      ampSustain = LINEAR[value];
      updateSustain();
      break;

    case CCamprelease:
      ampRelease = ENVTIMES[value];
      updateRelease();
      break;

    case CCringmod:
      value > 0 ? oscFX = 1 : oscFX = 0;
      updateOscFX();
      break;

    case CCfxamt:
      //Pick up
      if (!pickUpActive && pickUp && (fxAmtPrevValue <  ENSEMBLE_LFO[value - TOLERANCE] || fxAmtPrevValue >  ENSEMBLE_LFO[value + TOLERANCE])) return; //PICK-UP
      fxAmt = ENSEMBLE_LFO[value];
      updateFXAmt();
      fxAmtPrevValue = fxAmt;//PICK-UP
      break;

    case CCfxmix:
      //Pick up
      if (!pickUpActive && pickUp && (fxMixPrevValue <  LINEAR[value - TOLERANCE] || fxMixPrevValue >  LINEAR[value + TOLERANCE])) return; //PICK-UP
      fxMix = LINEAR[value];
      updateFXMix();
      fxMixPrevValue = fxMix;//PICK-UP
      break;

    case CCallnotesoff:
      allNotesOff();
      break;
  }
}

void myProgramChange(byte channel, byte program)
{
  state = PATCH;
  patchNo = program + 1;
  recallPatch(patchNo);
  Serial.print("MIDI Pgm Change:");
  Serial.println(patchNo);
  state = PARAMETER;
}

void myMIDIClockStart()
{
  // MIDIClkSignal = true;
  //Resync LFOs when MIDI Clock starts.
  //When there's a jump to a different
  //part of a track, such as in a DAW, the DAW must have same
  //rhythmic quantisation as Tempo Div.
  if (oscLFOMidiClkSync == 1)
  {
    pitchLfo.sync();
  }
  if (filterLFOMidiClkSync == 1)
  {
    filterLfo.sync();
  }
}

void myMIDIClockStop()
{
  // MIDIClkSignal = false;
}

void myMIDIClock()
{
  //This recalculates the LFO frequencies if the tempo changes (MIDI cLock is 24ppq)
  // if ((oscLFOMidiClkSync == 1 || filterLFOMidiClkSync == 1) && count > 23)
  // {
  //   MIDIClkSignal = !MIDIClkSignal;
  //   float timeNow = millis();
  //   midiClkTimeInterval = (timeNow - previousMillis);
  //   lfoSyncFreq = 1000.0 / midiClkTimeInterval;
  //   previousMillis = timeNow;
  //   if (oscLFOMidiClkSync == 1)pitchLfo.frequency(lfoSyncFreq * lfoTempoValue); //MIDI CC only
  //   if (filterLFOMidiClkSync == 1)filterLfo.frequency(lfoSyncFreq * lfoTempoValue);
  //   count = 0;
  // }
  // if (count < 24) count++; //prevent eventual overflow
}

void closeEnvelopes() {
  filterEnvelope1.close();
  filterEnvelope2.close();
  filterEnvelope3.close();
  filterEnvelope4.close();
  filterEnvelope5.close();
  filterEnvelope6.close();
  ampEnvelope1.close();
  ampEnvelope2.close();
  ampEnvelope3.close();
  ampEnvelope4.close();
  ampEnvelope5.close();
  ampEnvelope6.close();
}

void recallPatch(int patchNo)
{
  allNotesOff();
  closeEnvelopes();
  File patchFile = SD.open(("TSPATCH/" + String(patchNo)).c_str());
  if (!patchFile)
  {
    Serial.println("File not found");
  }
  else
  {
    String data[NO_OF_PARAMS]; //Array of data read in
    recallPatchData(patchFile, data);
    setCurrentPatchData(data);
    patchFile.close();
  }
}

void setCurrentPatchData(String data[])
{
  patchName = data[0];
  oscALevel = data[1].toFloat();
  oscBLevel = data[2].toFloat();
  noiseLevel = data[3].toFloat();
  unison = data[4].toInt();
  oscFX = data[5].toInt();
  detune = data[6].toFloat();
  lfoSyncFreq = data[7].toInt();
  midiClkTimeInterval = data[8].toInt();
  lfoTempoValue = data[9].toFloat();
  keytrackingAmount = data[10].toFloat();
  glideSpeed = data[11].toFloat();
  oscPitchA = data[12].toFloat();
  oscPitchB = data[13].toFloat();
  oscWaveformA = data[14].toInt();
  oscWaveformB = data[15].toInt();
  pwmSource = data[16].toInt();
  pwmAmtA = data[17].toFloat();
  pwmAmtB = data[18].toFloat();
  pwmRate = data[19].toFloat();
  pwA = data[20].toFloat();
  pwB = data[21].toFloat();
  filterRes = data[22].toFloat();
  resonancePrevValue = filterRes;//Pick-up
  filterFreq = data[23].toInt();
  filterfreqPrevValue = filterFreq; //Pick-up
  filterMix = data[24].toFloat();
  filterMixPrevValue = filterMix; //Pick-up
  filterEnv = data[25].toFloat();
  oscLfoAmt = data[26].toFloat();
  oscLfoAmtPrevValue = oscLfoAmt;//PICK-UP
  oscLfoRate = data[27].toFloat();
  oscLfoRatePrevValue = oscLfoRate;//PICK-UP
  oscLFOWaveform = data[28].toFloat();
  oscLfoRetrig = data[29].toInt();
  oscLFOMidiClkSync = data[30].toFloat(); //MIDI CC Only
  filterLfoRate = data[31].toFloat();
  filterLfoRatePrevValue = filterLfoRate;//PICK-UP
  filterLfoRetrig = data[32].toInt();
  filterLFOMidiClkSync = data[33].toFloat();
  filterLfoAmt = data[34].toFloat();
  filterLfoAmtPrevValue = filterLfoAmt;//PICK-UP
  filterLfoWaveform = data[35].toFloat();
  filterAttack = data[36].toFloat();
  filterDecay = data[37].toFloat();
  filterSustain = data[38].toFloat();
  filterRelease = data[39].toFloat();
  ampAttack = data[40].toFloat();
  ampDecay = data[41].toFloat();
  ampSustain = data[42].toFloat();
  ampRelease = data[43].toFloat();
  fxAmt = data[44].toFloat();
  fxAmtPrevValue = fxAmt;//PICK-UP
  fxMix = data[45].toFloat();
  fxMixPrevValue = fxMix;//PICK-UP
  pitchEnv = data[46].toFloat();
  velocitySens = data[47].toFloat();

  updatePatchname();
  updateUnison();
  updateWaveformA();
  updateWaveformB();
  updatePitchA();
  updatePitchB();
  updateDetune();
  updatePWMSource();
  updatePWMAmount();
  updatePWA();
  updatePWB();
  updatePWMRate();
  updateOscLevelA();
  updateOscLevelB();
  updateNoiseLevel();
  updateFilterFreq();
  updateFilterRes();
  updateFilterMixer();
  updateFilterEnv();
  updateKeyTracking();
  updateOscLFOAmt();
  updatePitchLFORate();
  updatePitchLFOWaveform();
  updatePitchLFOMidiClkSync();
  updateFilterLfoRate();
  updateFilterLfoAmt();
  updateFilterLFOWaveform();
  updateFilterLFOMidiClkSync();
  updateFilterLFORetrig();
  updateFilterAttack();
  updateFilterDecay();
  updateFilterSustain();
  updateFilterRelease();
  updateAttack();
  updateDecay();
  updateSustain();
  updateRelease();
  updateOscFX();
  updateFXAmt();
  updateFXMix();
  updatePitchEnv();
  Serial.print("Set Patch: ");
  Serial.println(patchName);
}

String getCurrentPatchData()
{
  return patchName + "," + String(oscALevel) + "," + String(oscBLevel) + "," + String(noiseLevel) + "," + String(unison) + "," + String(oscFX) + "," + String(detune) + "," + String(lfoSyncFreq) + "," + String(midiClkTimeInterval) + "," + String(lfoTempoValue) + "," + String(keytrackingAmount) + "," + String(glideSpeed) + "," + String(oscPitchA) + "," + String(oscPitchB) + "," + String(oscWaveformA) + "," + String(oscWaveformB) + "," +
         String(pwmSource) + "," + String(pwmAmtA) + "," + String(pwmAmtB) + "," + String(pwmRate) + "," + String(pwA) + "," + String(pwB) + "," + String(filterRes) + "," + String(filterFreq) + "," + String(filterMix) + "," + String(filterEnv) + "," + String(oscLfoAmt) + "," + String(oscLfoRate) + "," + String(oscLFOWaveform) + "," + String(oscLfoRetrig) + "," + String(oscLFOMidiClkSync) + "," + String(filterLfoRate) + "," +
         filterLfoRetrig + "," + filterLFOMidiClkSync + "," + filterLfoAmt + "," + filterLfoWaveform + "," + filterAttack + "," + filterDecay + "," + filterSustain + "," + filterRelease + "," + ampAttack + "," + ampDecay + "," + ampSustain + "," + ampRelease + "," +
         String(fxAmt) + "," + String(fxMix) + "," + String(pitchEnv) + "," + String(velocitySens);
}

// void checkVolumePot()
// {
//   volumeRead = adc->adc0->analogRead(VOLUME_POT);
//   if (volumeRead > (volumePrevious + QUANTISE_FACTOR) || volumeRead < (volumePrevious - QUANTISE_FACTOR))
//   {
//     volumePrevious = volumeRead;
//     volumeRead = (volumeRead >> 5); //Change range to 0-127
//     myControlChange(midiChannel, CCvolume, volumeRead);
//   }
// }

// void checkSwitches()
// {
//   unisonSwitch.update();
//   if (unisonSwitch.fallingEdge())
//   {
//     unison = !unison;
//     myControlChange(midiChannel, CCunison, unison);
//   }
//
//   oscFXSwitch.update();
//   if (oscFXSwitch.fallingEdge())
//   {
//     oscFX = !oscFX;
//     myControlChange(midiChannel, CCringmod, oscFX);
//   }
//
//   filterLFORetrigSwitch.update();
//   if (filterLFORetrigSwitch.fallingEdge())
//   {
//     filterLfoRetrig = !filterLfoRetrig;
//     myControlChange(midiChannel, CCfilterlforetrig, filterLfoRetrig);
//   }
//
//   tempoSwitch.update();
//   if (tempoSwitch.fallingEdge())
//   {
//     filterLFOMidiClkSync = !filterLFOMidiClkSync;
//     myControlChange(midiChannel, CCfilterLFOMidiClkSync, filterLFOMidiClkSync);
//   }
//
//   saveButton.update();
//   if (saveButton.read() == LOW && saveButton.duration() > HOLD_DURATION)
//   {
//     switch (state)
//     {
//       case PARAMETER:
//       case PATCH:
//         state = DELETE;
//         saveButton.write(HIGH); //Come out of this state
//         del = true;             //Hack
//         break;
//     }
//   }
//   else if (saveButton.risingEdge())
//   {
//     if (!del)
//     {
//       switch (state)
//       {
//         case PARAMETER:
//           if (patches.size() < PATCHES_LIMIT)
//           {
//             resetPatchesOrdering(); //Reset order of patches from first patch
//             patches.push({patches.size() + 1, INITPATCHNAME});
//             state = SAVE;
//           }
//           break;
//         case SAVE:
//           //Save as new patch with INITIALPATCH name or overwrite existing keeping name - bypassing patch renaming
//           patchName = patches.last().patchName;
//           state = PATCH;
//           savePatch(String(patches.last().patchNo).c_str(), getCurrentPatchData());
//           showCurrentParameterPage(patches.last().patchNo, patches.last().patchName);
//           patchNo = patches.last().patchNo;
//           loadPatches(); //Get rid of pushed patch if it wasn't saved
//           setPatchesOrdering(patchNo);
//           renamedPatch = "";
//           state = PARAMETER;
//           break;
//         case PATCHNAMING:
//           if (renamedPatch.length() > 0) patchName = renamedPatch;//Prevent empty strings
//           state = PATCH;
//           savePatch(String(patches.last().patchNo).c_str(), getCurrentPatchData());
//           showCurrentParameterPage(patches.last().patchNo, patchName);
//           patchNo = patches.last().patchNo;
//           loadPatches(); //Get rid of pushed patch if it wasn't saved
//           setPatchesOrdering(patchNo);
//           renamedPatch = "";
//           state = PARAMETER;
//           break;
//       }
//     }
//     else
//     {
//       del = false;
//     }
//   }
//
//   settingsButton.update();
//   if (settingsButton.read() == LOW && settingsButton.duration() > HOLD_DURATION)
//   {
//     //If recall held, set current patch to match current hardware state
//     //Reinitialise all hardware values to force them to be re-read if different
//     state = REINITIALISE;
//     reinitialiseToPanel();
//     settingsButton.write(HIGH); //Come out of this state
//     reini = true;           //Hack
//   }
//   else if (settingsButton.risingEdge())
//   { //cannot be fallingEdge because holding button won't work
//     if (!reini)
//     {
//       switch (state)
//       {
//         case PARAMETER:
//           settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
//           showCurrentParameterPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex]);
//           state = SETTINGS;
//           break;
//         case SETTINGS:
//           settingsOptions.push(settingsOptions.shift());
//           settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
//           showCurrentParameterPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex]);
//         case SETTINGSVALUE:
//           //Same as pushing Recall - store current settings item and go back to options
//           settingsHandler(settingsOptions.first().value[settingsValueIndex], settingsOptions.first().handler);
//           showCurrentParameterPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex]);
//           state = SETTINGS;
//           break;
//       }
//     }
//     else
//     {
//       reini = false;
//     }
//   }
//
//   backButton.update();
//   if (backButton.read() == LOW && backButton.duration() > HOLD_DURATION)
//   {
//     //If Back button held, Panic - all notes off
//     allNotesOff();
//     closeEnvelopes();
//     backButton.write(HIGH); //Come out of this state
//     panic = true;           //Hack
//   }
//   else if (backButton.risingEdge())
//   { //cannot be fallingEdge because holding button won't work
//     if (!panic)
//     {
//       switch (state)
//       {
//         case RECALL:
//           setPatchesOrdering(patchNo);
//           state = PARAMETER;
//           break;
//         case SAVE:
//           renamedPatch = "";
//           state = PARAMETER;
//           loadPatches();//Remove patch that was to be saved
//           setPatchesOrdering(patchNo);
//           break;
//         case PATCHNAMING:
//           charIndex = 0;
//           renamedPatch = "";
//           state = SAVE;
//           break;
//         case DELETE:
//           setPatchesOrdering(patchNo);
//           state = PARAMETER;
//           break;
//         case SETTINGS:
//           state = PARAMETER;
//           break;
//         case SETTINGSVALUE:
//           settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
//           showCurrentParameterPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex]);
//           state = SETTINGS;
//           break;
//       }
//     }
//     else
//     {
//       panic = false;
//     }
//   }
//
//   //Encoder switch
//   recallButton.update();
//   if (recallButton.read() == LOW && recallButton.duration() > HOLD_DURATION)
//   {
//     //If Recall button held, return to current patch setting
//     //which clears any changes made
//     state = PATCH;
//     //Recall the current patch
//     patchNo = patches.first().patchNo;
//     recallPatch(patchNo);
//     state = PARAMETER;
//     recallButton.write(HIGH); //Come out of this state
//     recall = true;            //Hack
//   }
//   else if (recallButton.risingEdge())
//   {
//     if (!recall)
//     {
//       switch (state)
//       {
//         case PARAMETER:
//           state = RECALL;//show patch list
//           break;
//         case RECALL:
//           state = PATCH;
//           //Recall the current patch
//           patchNo = patches.first().patchNo;
//           recallPatch(patchNo);
//           state = PARAMETER;
//           break;
//         case SAVE:
//           showRenamingPage(patches.last().patchName);
//           patchName  = patches.last().patchName;
//           state = PATCHNAMING;
//           break;
//         case PATCHNAMING:
//           if (renamedPatch.length() < 12) //actually 12 chars
//           {
//             renamedPatch.concat(String(currentCharacter));
//             charIndex = 0;
//             currentCharacter = CHARACTERS[charIndex];
//             showRenamingPage(renamedPatch);
//           }
//           break;
//         case DELETE:
//           //Don't delete final patch
//           if (patches.size() > 1)
//           {
//             state = DELETEMSG;
//             patchNo = patches.first().patchNo;//PatchNo to delete from SD card
//             patches.shift();//Remove patch from circular buffer
//             deletePatch(String(patchNo).c_str());//Delete from SD card
//             loadPatches();//Repopulate circular buffer to start from lowest Patch No
//             renumberPatchesOnSD();
//             loadPatches();//Repopulate circular buffer again after delete
//             patchNo = patches.first().patchNo;//Go back to 1
//             recallPatch(patchNo);//Load first patch
//           }
//           state = PARAMETER;
//           break;
//         case SETTINGS:
//           //Choose this option and allow value choice
//           settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
//           showCurrentParameterPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex]);
//           state = SETTINGSVALUE;
//           break;
//         case SETTINGSVALUE:
//           //Store current settings item and go back to options
//           settingsHandler(settingsOptions.first().value[settingsValueIndex], settingsOptions.first().handler);
//           showCurrentParameterPage(settingsOptions.first().option, settingsOptions.first().value[settingsValueIndex]);
//           state = SETTINGS;
//           break;
//       }
//     }
//     else
//     {
//       recall = false;
//     }
//   }
// }

boolean TsynthFirstParameter = true;
String line;

void TsynthcheckDoublePress(byte inputIndex){
  DisplayParamMsec = 0;
  if(!TsynthFirstParameter)
    TsynthFirstParameter = !TsynthFirstParameter;
    device->updateEncodeursMaxValue(0, 1-6);
    line = TsynthFirstParamName[state];
    device->updateLine(1, line);
}

void TsynthcheckPress(byte inputIndex){
  DisplayParamMsec = 0;
  if(TsynthFirstParameter){
    TsynthFirstParameter = !TsynthFirstParameter;
    device->updateEncodeursMaxValue(0, -100);
    switch (state)
    {
      case PARAMETER:
        state = RECALL;//show patch list
        line = "Patch : " + String(patches.first().patchName);        break;
      case RECALL:
        state = PATCH;
        //Recall the current patch
        line = "Patch : " + String(patches.first().patchName);
        break;
      case SAVE:
        line = patches.last().patchName;
        break;
      case PATCHNAMING:
        if (renamedPatch.length() < 12) //actually 12 chars
        {
          line = renamedPatch;
        }
        break;
      case DELETE:
        //Don't delete final patch
        break;
      case SETTINGS:
        //Choose this option and allow value choice
        line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[settingsValueIndex]);
        break;
      case SETTINGSVALUE:
        //Store current settings item and go back to options
        line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[settingsValueIndex]);
        break;
    }
  }else{
    switch (state)
    {
      case PARAMETER:
        state = RECALL;//show patch list
        line = "Patch : " + String(patches.first().patchName);
        break;
      case RECALL:
        state = PATCH;
        //Recall the current patch
        patchNo = patches.first().patchNo;
        recallPatch(patchNo);
        line = "Patch : " + String(patches.first().patchName);
        state = RECALL;
        break;
      case SAVE:
        line = patches.last().patchName;
        patchName  = String(patches.last().patchName);
        state = PATCHNAMING;
        break;
      case PATCHNAMING:
        if (renamedPatch.length() < 12) //actually 12 chars
        {
          renamedPatch.concat(String(currentCharacter));
          charIndex = 0;
          currentCharacter = CHARACTERS[charIndex];
          line = renamedPatch;
        }
        break;
      case DELETE:
        //Don't delete final patch
        if (patches.size() > 1)
        {
          state = DELETEMSG;
          patchNo = patches.first().patchNo;//PatchNo to delete from SD card
          patches.shift();//Remove patch from circular buffer
          deletePatch(String(patchNo).c_str());//Delete from SD card
          loadPatches();//Repopulate circular buffer to start from lowest Patch No
          renumberPatchesOnSD();
          loadPatches();//Repopulate circular buffer again after delete
          patchNo = patches.first().patchNo;//Go back to 1
          recallPatch(patchNo);//Load first patch
        }
        state = PARAMETER;
        break;
      case SETTINGS:
        //Choose this option and allow value choice
        settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
        line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[settingsValueIndex]);
        state = SETTINGSVALUE;
        break;
      case SETTINGSVALUE:
        //Store current settings item and go back to options
        settingsHandler(settingsOptions.first().value[settingsValueIndex], settingsOptions.first().handler);
        line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[settingsValueIndex]);
        state = SETTINGS;
        break;
    }
  }
  device->updateLine(1, line);
}

void TsynthcheckEncoder(byte inputIndex, long value)
{
  //Encoder works with relative inc and dec values
  //Detent encoder goes up in 4 steps, hence +/-3
  long encRead = value;
  if(TsynthFirstParameter){
    state = value;
    line = TsynthFirstParamName[state];
  }else{
    if ((encCW && encRead > encPrevious) || (!encCW && encRead < encPrevious) )
    {
      switch (state)
      {
        case PARAMETER:
          state = PATCH;
          patches.push(patches.shift());
          patchNo = patches.first().patchNo;
          // recallPatch(patchNo);
          line = "Patch : " + String(patches.first().patchName);
          state = RECALL;
          break;
        case RECALL:
          patches.push(patches.shift());
          patchNo = patches.first().patchNo;
          line = "Patch : " + String(patches.first().patchName);
          break;
        case SAVE:
          patches.push(patches.shift());
          line = "SAVE : " + String(patches.first().patchName);
          break;
        case PATCHNAMING:
          if (charIndex == TOTALCHARS) charIndex = 0;//Wrap around
          currentCharacter = CHARACTERS[charIndex++];
          line = renamedPatch + " : " + currentCharacter;
          break;
        case DELETE:
          patches.push(patches.shift());
          line =  "DELETE";
          break;
        case SETTINGS:
          settingsOptions.push(settingsOptions.shift());
          settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
          line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[settingsValueIndex]);
          break;
        case SETTINGSVALUE:
          if (settingsOptions.first().value[settingsValueIndex + 1] != '\0'){
            line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[++settingsValueIndex]);
          }
          break;
      }
      encPrevious = encRead;
    }
    else if ((encCW && encRead < encPrevious) || (!encCW && encRead > encPrevious))
    {
      switch (state)
      {
        case PARAMETER:
          state = PATCH;
          patches.unshift(patches.pop());
          patchNo = patches.first().patchNo;
          // recallPatch(patchNo);
          line = "Patch : " + String(patches.first().patchName);
          state = PARAMETER;
          break;
        case RECALL:
          patches.unshift(patches.pop());
          patchNo = patches.first().patchNo;
          line = "Patch : " + String(patches.first().patchName);
          break;
        case SAVE:
          patches.unshift(patches.pop());
          line = "SAVE : " + String(patches.first().patchName);
          break;
        case PATCHNAMING:
          if (charIndex == -1)
            charIndex = TOTALCHARS - 1;
          currentCharacter = CHARACTERS[charIndex--];
          line = renamedPatch + " : " + currentCharacter;
          break;
        case DELETE:
          patches.unshift(patches.pop());
          break;
        case SETTINGS:
          settingsOptions.unshift(settingsOptions.pop());
          settingsValueIndex = getCurrentIndex(settingsOptions.first().currentIndex);
          line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[settingsValueIndex]);
          break;
        case SETTINGSVALUE:
          if (settingsValueIndex > 0){
            line = String(settingsOptions.first().option) + " : " + String(settingsOptions.first().value[--settingsValueIndex]);
          }
          break;
      }
    }
  }
  encPrevious = encRead;
  device->updateLine(1, line);
}

void setupTsynth()
{
  setUpSettings();

  TsynthFirstParameter = true;

  if (hasSD)
  {
    Serial.println("SD card is connected");
    //Get patch numbers and names from SD card
    loadPatches();
    if (patches.size() == 0)
    {
      //save an initialised patch to SD card
      savePatch("1", INITPATCH);
      loadPatches();
    }
  }
  else
  {
    Serial.println("SD card is not connected or unusable");
    // reinitialiseToPanel();
    showCurrentParameterPage("No SD", "conn'd / usable");
  }

  //Read MIDI Channel from EEPROM
  midiChannel = 1; //getMIDIChannel();
  Serial.println("MIDI Ch:" + String(midiChannel) + " (0 is Omni On)");

  device->updateEncodeursValue(0, 0);
  device->setHandleEncoderChange(0, TsynthcheckEncoder);
  device->setHandlePress(0, TsynthcheckPress);
  device->setHandleDoublePress(0, TsynthcheckDoublePress);
  device->updateEncodeursMaxValue(0, 1-6);
  for (int i=0;i<ANALOG_CONTROL_PINS;i++){
    device->setHandlePotentiometerChange(i, TsynthHandlePotards);
  }

  //MIDI 5 Pin DIN
  MIDI.setHandleNoteOn(myNoteOn);
  MIDI.setHandleNoteOff(myNoteOff);
  MIDI.setHandlePitchBend(myPitchBend);
  MIDI.setHandleControlChange(myControlChange);
  MIDI.setHandleProgramChange(myProgramChange);
  // MIDI.setHandleClock(myMIDIClock);
  // MIDI.setHandleStart(myMIDIClockStart);
  // MIDI.setHandleStop(myMIDIClockStop);
  Serial.println("MIDI In DIN Listening");

  Tsynth_AOstart();

  constant1Dc.amplitude(ONE);

  noiseMixer.gain(0, ONE);
  noiseMixer.gain(1, ONE);
  noiseMixer.gain(2, 0);
  noiseMixer.gain(3, 0);

  voiceMixer1.gain(0, VOICEMIXERLEVEL);
  voiceMixer1.gain(1, VOICEMIXERLEVEL);
  voiceMixer1.gain(2, VOICEMIXERLEVEL);
  voiceMixer1.gain(3, 0);

  voiceMixer2.gain(0, VOICEMIXERLEVEL);
  voiceMixer2.gain(1, VOICEMIXERLEVEL);
  voiceMixer2.gain(2, VOICEMIXERLEVEL);
  voiceMixer2.gain(3, 0);

  voiceMixerM.gain(0, 0.5f);
  voiceMixerM.gain(1, 0.5f);
  voiceMixerM.gain(2, 0);
  voiceMixerM.gain(3, 0);

  pwmLfo.amplitude(ONE);
  pwmLfo.begin(PWMWAVEFORM);

  waveformMod1a.frequency(440);
  waveformMod1a.amplitude(ONE);
  waveformMod1a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod1a.begin(oscWaveformA);

  waveformMod1b.frequency(440);
  waveformMod1b.amplitude(ONE);
  waveformMod1b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod1b.begin(oscWaveformB);

  waveformMod2a.frequency(440);
  waveformMod2a.amplitude(ONE);
  waveformMod2a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod2a.begin(oscWaveformA);

  waveformMod2b.frequency(440);
  waveformMod2b.amplitude(ONE);
  waveformMod2b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod2b.begin(oscWaveformB);

  waveformMod3a.frequency(440);
  waveformMod3a.amplitude(ONE);
  waveformMod3a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod3a.begin(oscWaveformA);

  waveformMod3b.frequency(440);
  waveformMod3b.amplitude(ONE);
  waveformMod3b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod3b.begin(oscWaveformB);

  waveformMod4a.frequency(440);
  waveformMod4a.amplitude(ONE);
  waveformMod4a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod4a.begin(oscWaveformA);

  waveformMod4b.frequency(440);
  waveformMod4b.amplitude(ONE);
  waveformMod4b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod4b.begin(oscWaveformB);

  waveformMod5a.frequency(440);
  waveformMod5a.amplitude(ONE);
  waveformMod5a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod5a.begin(oscWaveformA);

  waveformMod5b.frequency(440);
  waveformMod5b.amplitude(ONE);
  waveformMod5b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod5b.begin(oscWaveformB);

  waveformMod6a.frequency(440);
  waveformMod6a.amplitude(ONE);
  waveformMod6a.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod6a.begin(oscWaveformA);

  waveformMod6b.frequency(440);
  waveformMod6b.amplitude(ONE);
  waveformMod6b.frequencyModulation(PITCHLFOOCTAVERANGE);
  waveformMod6b.begin(oscWaveformB);

  //Arbitary waveform needs initialising to something
  loadArbWaveformA(PARABOLIC_WAVE);
  loadArbWaveformB(PARABOLIC_WAVE);

  waveformMixer1.gain(2, ONE); //Noise
  waveformMixer1.gain(3, 0);           //Osc FX
  waveformMixer2.gain(2, ONE); //Noise
  waveformMixer2.gain(3, 0);           //Osc FX
  waveformMixer3.gain(2, ONE); //Noise
  waveformMixer3.gain(3, 0);           //Osc FX
  waveformMixer4.gain(2, ONE); //Noise
  waveformMixer4.gain(3, 0);           //Osc FX
  waveformMixer5.gain(2, ONE); //Noise
  waveformMixer5.gain(3, 0);           //Osc FX
  waveformMixer6.gain(2, ONE); //Noise
  waveformMixer6.gain(3, 0);           //Osc FX

  filterModMixer1.gain(1, ONE); //LFO
  filterModMixer1.gain(3, 0);           //Not used
  filterModMixer2.gain(1, ONE); //LFO
  filterModMixer2.gain(3, 0);           //Not used
  filterModMixer3.gain(1, ONE); //LFO
  filterModMixer3.gain(3, 0);           //Not used
  filterModMixer4.gain(1, ONE); //LFO
  filterModMixer4.gain(3, 0);           //Not used
  filterModMixer5.gain(1, ONE); //LFO
  filterModMixer5.gain(3, 0);           //Not used
  filterModMixer6.gain(1, ONE); //LFO
  filterModMixer6.gain(3, 0);           //Not used

  filterMixer1.gain(3, 0); //Not used
  filterMixer2.gain(3, 0); //Not used
  filterMixer3.gain(3, 0); //Not used
  filterMixer4.gain(3, 0); //Not used
  filterMixer5.gain(3, 0); //Not used
  filterMixer6.gain(3, 0); //Not used

  //This removes dc offset (mostly from unison pulse waves) before the ensemble effect
  dcOffsetFilter.octaveControl(1.0);
  dcOffsetFilter.frequency(12.0);//Lower values will give clicks on note on/off

  ensemble.lfoRate(fxAmt);

  effectMixerL.gain(2, 0);
  effectMixerL.gain(3, 0);
  effectMixerR.gain(2, 0);
  effectMixerR.gain(3, 0);

  volumePrevious = RE_READ; //Force volume control to be read and set to current

  //Read Pitch Bend Range from EEPROM
  pitchBendRange = getPitchBendRange();

  //Read Mod Wheel Depth from EEPROM
  modWheelDepth = getModWheelDepth();

  //Read MIDI Out Channel from EEPROM
  // midiOutCh = getMIDIOutCh();

  //Read Encoder Direction from EEPROM
  // encCW = getEncoderDir();

  //Read bass enhance enable from EEPROM
  // if (getBassEnhanceEnable()) sgtl5000_1.enhanceBassEnable();

  //Read Pick-up enable from EEPROM - experimental feature
  // pickUp = getPickupEnable();

  //Read oscilloscope enable from EEPROM
  // enableScope(getScopeEnable());

  //Read VU enable from EEPROM
  // vuMeter = getVUEnable();
}

void stopTsynth(){
  Tsynth_AOstop();
}

void runTsynth()
{
  if(DisplayParamMsec > 400 && DisplayParamMsec < 500){
    device->updateLine(1, line);
  }
}
