// HELPER FUNCTIONS USED ACROSS VARIOUS CLASSES AND IN MAIN CODE

#ifndef Kelpie_Helpers_h
#define Kelpie_Helpers_h

#include <Arduino.h>
#include <Audio.h>
// #include <HardwareInputs.h>
#include "keyMappings.h"
#include "SynthVoice.h"
#include "SynthState.h"
#include "constants.h"

extern const byte numPolyVoices;
extern SynthVoice polyVoices[];
extern SynthVoice monoVoices[];
extern SynthState globalState;
extern boolean prevButtonsState[];
extern AudioAmplifier MASTER_GAIN;
extern AudioSynthWaveformSine LFO;
extern AudioMixer4 LFO_MIXER_FILTER;
extern AudioMixer4 LFO_MIXER_AMP;
extern const byte MONOBUFFERSIZE;
extern byte monoBuffer[];

enum playModes {
  PLAY_NOTE,
  UPDATE_NOTE,
  STOP_NOTE
};

void activateVoice(byte index, byte note, float frequency, float gain)
{
  polyVoices[index].note = note;
  polyVoices[index].noteFreq = frequency;
  polyVoices[index].waveformA.frequency(frequency * globalState.PITCH_BEND);
  polyVoices[index].waveformA.phase(0);
  polyVoices[index].waveformB.frequency(frequency * globalState.PITCH_BEND * globalState.DETUNE);
  polyVoices[index].waveformB.phase(0);
  polyVoices[index].waveformAmplifier.gain(gain * globalState.PREFILTER_GAIN);
  polyVoices[index].ampEnv.noteOn();
  polyVoices[index].filterEnv.noteOn();
}

void deactivateVoice(byte index)
{
  polyVoices[index].ampEnv.noteOff();
  polyVoices[index].filterEnv.noteOff();
  polyVoices[index].note = -1;
}

void playNoteMono(byte playMode, byte note, float noteGain)
{
  globalState.PREV_NOTE = globalState.CURRENT_NOTE;
  globalState.CURRENT_NOTE = note;
  if (globalState.CURRENT_NOTE == globalState.PREV_NOTE) {
    globalState.PREV_NOTE = 0; // this is a quick fix
    globalState.CURRENT_NOTE = 0;
  }
  float baseNoteFreq = noteFreqs[note];
  AudioNoInterrupts();
  switch (playMode)
  {
  case PLAY_NOTE: // PLAYNOTE
    for (byte i = 0; i < numPolyVoices; i++)
    {
      activateVoice(i, note, baseNoteFreq, noteGain);
    }
    break;
  case UPDATE_NOTE: // UPDATE NOTE
    for (byte i = 0; i < numPolyVoices; i++)
    {
      polyVoices[i].note = note;
      polyVoices[i].noteFreq = baseNoteFreq;
      polyVoices[i].waveformA.frequency(baseNoteFreq);// * globalState.PITCH_BEND);
      polyVoices[i].waveformB.frequency(baseNoteFreq * globalState.PITCH_BEND * globalState.DETUNE);
    }
    break;
  case STOP_NOTE: // STOP NOTE
    for (byte i = 0; i < numPolyVoices; i++)
    {
      deactivateVoice(i);
    }
    break;
  }
  AudioInterrupts();
};

void bufferShift(byte indexToRemove, byte currentSizeOfBuffer)
{
  for (byte i = indexToRemove + 1; i < currentSizeOfBuffer; i++)
  {
    monoBuffer[i - 1] = monoBuffer[i];
  }
  monoBuffer[currentSizeOfBuffer - 1] = 0;
}

void keyBuffMono(byte note, float noteGain, boolean playNote)
{
  static byte currentNote = 0; // this might make more sense to start with -1 to keep indexing legible
  if (playNote)
  {
    if (currentNote == MONOBUFFERSIZE) // if we exceed buffer size, newest note goes on end, remove first note and SHIFT all notes down 1
    {
      bufferShift(0, currentNote);
      currentNote = MONOBUFFERSIZE - 1;
    }
    monoBuffer[currentNote] = note;
    playNoteMono(PLAY_NOTE, note, noteGain);
    currentNote++;
  }
  else // key is released
  {
    byte foundNoteIndex = MONOBUFFERSIZE; // default to index larger than buffer size
    for (byte i = 0; i <= (currentNote); i++)
    {

      if (note == monoBuffer[i])
      {
        foundNoteIndex = i;
        monoBuffer[i] = 0;
        // note has to be stopped
        bufferShift(foundNoteIndex, currentNote);
        currentNote--;
        if (currentNote == 0)
        {
          playNoteMono(STOP_NOTE, note, noteGain);
        } else {
          playNoteMono(UPDATE_NOTE, monoBuffer[currentNote - 1], noteGain); // this is causing issues with the RELEASE phase of the AMP ENV
        }
      }
    }
  }
}

void keyBuffPoly(byte note, float noteGain, boolean playNote)
{
  if (playNote) // on keypress
  {
    float baseNoteFreq = noteFreqs[note];
    for (byte i = 0; i < numPolyVoices; i++)
    {
      if (polyVoices[i].ampEnv.isActive() == false)
      {
        activateVoice(i, note, baseNoteFreq, noteGain);
        break;
      }
    }
  }
  else // key released
  {
    for (byte i = 0; i < numPolyVoices; i++)
    {
      if (polyVoices[i].note == note)
      {
        deactivateVoice(i);
      }
    }
  }
}

// this function is responsible to balancing OSC1, OSC2, and NOISE for each voice. This is necessary because KNOB 1 balances OSC1 and 2, while Knob 6 adds and subtracts noise presense from audio
float calculateOscConstant(float osc1Vol, float osc2Vol, float noiseVol)
{
  float numerator = (1 - noiseVol);
  float denominator = (osc1Vol + osc2Vol + noiseVol);
  float constant = numerator / denominator;
  return constant;
}

void setWaveformLevels(float osc1Vol, float osc2Vol, float noiseVol, float oscConstant)
{
  for (byte i = 0; i < numPolyVoices; i++)
  {
    polyVoices[i].waveformA.amplitude(osc1Vol * oscConstant);
    polyVoices[i].waveformB.amplitude(osc2Vol * oscConstant);
    polyVoices[i].noise.amplitude(noiseVol - (noiseVol * oscConstant));
  }
}

float calculateDetuneValue(int knobReading)
{
  int knobInverse = 1023 - knobReading;
  float mappedKnob = 0.0;

  // piecewise function that separates tuning knob into 3 sections
  // mid section is shallower to achieve finer tuning
  // lower and upper limits will push the tuning 1 octave higher or lower
  if (knobInverse >= 0 && knobInverse <= (362))
  {
    mappedKnob = knobInverse * 1.24; // 1.24 is calculated by dividing 1023/823
  }
  else if (knobInverse > (362) && knobInverse < (662))
  {
    // this line is calculated by interpolating the points between lower and upper sections
    mappedKnob = ((knobInverse - 362) * 0.413) + 449;
  }
  else if (knobInverse >= (662) && knobInverse <= 1023)
  {
    // this is a line with the same slope as the lower bound, but offset in X direction
    mappedKnob = (knobInverse - 200) * 1.24;
  }
  // converts mapped reading to float val between 0 and 1.0
  float decKnobVal = mappedKnob * DIV1023;
  // maps prev value to value between 1/2 and 2, this is is half or 2x the base frequency (octaves)
  return pow(2, 2 * (decKnobVal - 0.5));
}


#endif
