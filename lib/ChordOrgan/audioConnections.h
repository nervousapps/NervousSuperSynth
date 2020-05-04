#ifndef ChordOrgan_Audio_Connections_h
#define ChordOrgan_Audio_Connections_h

#include "Arduino.h"
#include <Audio.h>

extern AudioSynthWaveform       chordOrganwaveform1;      //xy=215,232
extern AudioSynthWaveform       chordOrganwaveform2;      //xy=243,295
extern AudioSynthWaveform       chordOrganwaveform3;      //xy=273,354
extern AudioSynthWaveform       chordOrganwaveform4;      //xy=292,394
extern AudioSynthWaveform       chordOrganwaveform5;      //xy=215,232
extern AudioSynthWaveform       chordOrganwaveform6;      //xy=243,295
extern AudioSynthWaveform       chordOrganwaveform7;      //xy=273,354
extern AudioSynthWaveform       chordOrganwaveform8;      //xy=292,394
extern AudioMixer4              chordOrganmixer1;         //xy=424,117
extern AudioMixer4              chordOrganmixer2;         //xy=424,181
extern AudioMixer4              chordOrganmixer3;         //xy=571,84
extern AudioEffectEnvelope      chordOrganenvelope1;      //xy=652,281
// AudioOutputAnalog        dac1;           //xy=784,129
extern AudioConnection          chordOrganpatchCord1;
extern AudioConnection          chordOrganpatchCord2;
extern AudioConnection          chordOrganpatchCord7;
extern AudioConnection          chordOrganpatchCord8;
extern AudioConnection          chordOrganpatchCord3;
extern AudioConnection          chordOrganpatchCord4;
extern AudioConnection          chordOrganpatchCord5;
extern AudioConnection          chordOrganpatchCord6;
extern AudioConnection          chordOrganpatchCord9;
extern AudioConnection          chordOrganpatchCord10;
extern AudioConnection          chordOrganpatchCord11;
// AudioConnection          chordOrganpatchCord12(chordOrganenvelope1, 0, mainMix, 1);
// GUItool: end automatically generated code
// Pointers to waveforms

#endif
