#ifndef Kelpie_Master_h
#define Kelpie_Master_h

#include "Arduino.h"
#include <Audio.h>
#include "audioConnections.h"
#include "KelpieHelpers.h"

elapsedMillis fps;
elapsedMillis kelpieParam;

elapsedMillis kelpiesynthParamMsec = 0;
char kelpieEncoderParam[3][16] = {"Waveform1", "Waveform2", "Poly"};
char kelpirWaveforms[4][16] = {"SQUARE", "SAWTOOTH", "SINE", "ARBITR"};
byte waveform1state = 0;
byte waveform2state = 0;

byte kelpiedata[SLIDERS_PINS] = { 0 };
byte kelpiedataLag[SLIDERS_PINS] = { 0 };

boolean prevButtonsState[4] = {false, false, false, false}; // initial state on boot
boolean *buttonsState;
byte changedKnobIndex;
byte changedButtonIndex;

SynthVoice VOICE_1 = {0, 0.0, 0, false, V1_A, V1_B, V1_N, V1_MIX, V1_AMP, V1_ENV, V1_FILT_ENV, V1_FILT};
SynthVoice VOICE_2 = {0, 0.0, 0, false, V2_A, V2_B, V2_N, V2_MIX, V2_AMP, V2_ENV, V2_FILT_ENV, V2_FILT};
SynthVoice VOICE_3 = {0, 0.0, 0, false, V3_A, V3_B, V3_N, V3_MIX, V3_AMP, V3_ENV, V3_FILT_ENV, V3_FILT};
SynthVoice VOICE_4 = {0, 0.0, 0, false, V4_A, V4_B, V4_N, V4_MIX, V4_AMP, V4_ENV, V4_FILT_ENV, V4_FILT};
SynthVoice VOICE_5 = {0, 0.0, 0, false, V5_A, V5_B, V5_N, V5_MIX, V5_AMP, V5_ENV, V5_FILT_ENV, V5_FILT};
SynthVoice VOICE_6 = {0, 0.0, 0, false, V6_A, V6_B, V6_N, V6_MIX, V6_AMP, V6_ENV, V6_FILT_ENV, V6_FILT};
SynthVoice VOICE_7 = {0, 0.0, 0, false, V7_A, V7_B, V7_N, V7_MIX, V7_AMP, V7_ENV, V7_FILT_ENV, V7_FILT};
SynthVoice VOICE_8 = {0, 0.0, 0, false, V8_A, V8_B, V8_N, V8_MIX, V8_AMP, V8_ENV, V8_FILT_ENV, V8_FILT};

const byte numPolyVoices = 8; //
SynthVoice polyVoices[numPolyVoices] = {
    VOICE_1,
    VOICE_2,
    VOICE_3,
    VOICE_4,
    VOICE_5,
    VOICE_6,
    VOICE_7,
    VOICE_8};

const byte MONOBUFFERSIZE = 8; // how many voices we allow to be stored in the MONO buffer
byte monoBuffer[MONOBUFFERSIZE];

SynthState globalState = {
    // default synth state on startup
    WAVEFORM_SINE, // WAVEFORM1
    WAVEFORM_SAWTOOTH, // WAVEFORM2
    true,             // IS_POLY
    false,             // shift
    1.0,               // OSC1_VOL
    1.0,               // OSC2_VOL
    1.0,               // NOISE_VOL
    0.0,               // OSC_CONSTANT
    0.5,               // PWM
    1.0,               // DETUNE
    1.0,               // PITCH_BEND
    0.0,               // LFO_FREQ
    0.0,               // LFO_FILTER_GAIN
    0.0,               // LFO_AMP_GAIN
    0.0,               // AMP_ATTACK
    0.0,               // AMP_DECAY
    1.0,               // AMP_SUSTAIN
    500,               // AMP_RELEASE ms
    0.0,               // FILTER_ATTACK
    0.0,               // FILTER_DECAY
    1.0,               // FILTER_SUSTAIN
    500,               // FILTER_RELEASE
    10000,             // FILTER_FREQ
    1.1,               // FILTER_Q
    1.0,               // FILTER_OCTAVE
    5.0,               // MASTER_VOL
    1.0,               // POLY_GAIN_MULTIPLIER
    0.9,               // PREFILTER_GAIN
    0,                 // PREV_NOTE
    0                  // CURRENT NOTE
};

void get_parameters(){
  if(!sampleParam){
    for (int i=0;i<SLIDERS_PINS;i++){
      // update the ResponsiveAnalogRead object every loop
      analog_slide[i].update();
      if (analog_slide[i].hasChanged()) {
        kelpiedata[i] = analog_slide[i].getValue();
        if (kelpiedata[i] > kelpiedataLag[i]+2 || kelpiedata[i] < kelpiedataLag[i]-2) {
          if(kelpiedata[i] >= 115){
            kelpiedata[i] = 127;
          }
          if(kelpiedata[i] <= 10){
            kelpiedata[i] = 0;
          }
          if (kelpiedata[i] != kelpiedataLag[i]){
            kelpiedataLag[i] = kelpiedata[i];
            float normalizedKnobVal = (kelpiedataLag[i] * DIV127);
            switch (i)
            {
                case 0: // MASTER VOLUME
                  globalState.MASTER_VOL = normalizedKnobVal * MAX_MASTER_GAIN;
                  MASTER_GAIN.gain(globalState.MASTER_VOL * globalState.POLY_GAIN_MULTIPLIER);
                break;
                case 1: // OSC BALANCE
                  globalState.OSC1_VOL = normalizedKnobVal;
                  globalState.OSC2_VOL = 1 - (normalizedKnobVal);
                  globalState.OSC_CONSTANT = calculateOscConstant(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL);
                  setWaveformLevels(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL, globalState.OSC_CONSTANT);
                break;
                case 2: // OSCILLATOR PWM
                  globalState.PWM = 0.1 + 0.4 * (1 - normalizedKnobVal);
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].waveformA.pulseWidth(globalState.PWM);
                    polyVoices[i].waveformB.pulseWidth(globalState.PWM);
                  }
                break;
                case 3: // NOISE VOLUME
                  globalState.NOISE_VOL = normalizedKnobVal;
                  globalState.OSC_CONSTANT = calculateOscConstant(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL);
                  setWaveformLevels(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL, globalState.OSC_CONSTANT);
                break;
                case 4: // OSC DETUNE
                  globalState.DETUNE = calculateDetuneValue(normalizedKnobVal);
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].waveformB.frequency(polyVoices[i].noteFreq * globalState.DETUNE * globalState.PITCH_BEND);
                  }
                break;
                case 5: // FILTER FREQUENCY
                  globalState.FILTER_FREQ = FILTER_CUTOFF_MAX * pow(normalizedKnobVal, 3);
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].filter.frequency(globalState.FILTER_FREQ);
                  }
                break;
                case 6: // FILTER RESONANCE
                  globalState.FILTER_Q = (FILTER_Q_MAX * normalizedKnobVal) + 1.1;
                  globalState.PREFILTER_GAIN = 1 / globalState.FILTER_Q;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].filter.resonance(globalState.FILTER_Q);
                  }
                break;
                case 7: // FILTER DEPTH
                  globalState.FILTER_OCTAVE = FILTER_OCTAVE_DEPTH * normalizedKnobVal;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].filter.octaveControl(globalState.FILTER_OCTAVE);
                  }
                break;
                case 8: // LFO RATE
                  globalState.LFO_FREQ = LFO_FREQ_MAX * pow(normalizedKnobVal, 5);
                  LFO.frequency(globalState.LFO_FREQ);
                break;
                case 9: // LFO DESTINATION FILTER
                  globalState.LFO_FILTER_GAIN = (normalizedKnobVal);
                  LFO_MIXER_FILTER.gain(1, globalState.LFO_FILTER_GAIN);
                break;
                case 10: // LFO DESTINATION AMP
                  globalState.LFO_AMP_GAIN = (normalizedKnobVal);
                  LFO_MIXER_AMP.gain(1, globalState.LFO_AMP_GAIN);
                break;
                case 11: // AMP ATTACK
                globalState.AMP_ATTACK = AMP_ATTACK_MAX * (normalizedKnobVal);
                if (globalState.AMP_ATTACK < 15) //
                {
                  globalState.AMP_ATTACK = 0;
                }
                for (byte i = 0; i < numPolyVoices; i++)
                {
                  polyVoices[i].ampEnv.attack(globalState.AMP_ATTACK);
                }
                break;
                case 12: // AMP DECAY
                  globalState.AMP_DECAY = AMP_DECAY_MAX * normalizedKnobVal;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].ampEnv.decay(globalState.AMP_DECAY);
                  }
                break;
                case 13: // AMP SUSTAIN
                  globalState.AMP_SUSTAIN = normalizedKnobVal;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].ampEnv.sustain(globalState.AMP_SUSTAIN);
                  }
                break;
                case 14: // AMP RELEASE
                  globalState.AMP_RELEASE = AMP_RELEASE_MAX * normalizedKnobVal;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].ampEnv.release(globalState.AMP_RELEASE);
                  }
                break;
                case 15: // FILTER ATTACK
                      globalState.FILTER_ATTACK = FILTER_ATTACK_MAX * normalizedKnobVal;
                if (globalState.FILTER_ATTACK < 15) //
                {
                  globalState.FILTER_ATTACK = 0;
                }
                for (byte i = 0; i < numPolyVoices; i++)
                {
                  polyVoices[i].filterEnv.attack(globalState.FILTER_ATTACK);
                }
                break;
                case 16: // FILTER DECAY
                  globalState.FILTER_DECAY = FILTER_DECAY_MAX * normalizedKnobVal;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].filterEnv.decay(globalState.FILTER_DECAY);
                  }
                break;
                case 17: // FILTER SUSTAIN
                  globalState.FILTER_SUSTAIN = normalizedKnobVal;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].filterEnv.sustain(globalState.FILTER_SUSTAIN);
                  }
                break;
                case 18: // FILTER RELEASE
                  globalState.FILTER_RELEASE = FILTER_RELEASE_MAX * normalizedKnobVal;
                  for (byte i = 0; i < numPolyVoices; i++)
                  {
                    polyVoices[i].filterEnv.release(globalState.FILTER_RELEASE);
                  }
                break;
              }
          }
        }
      }
    }
  }
}

void kelpie_get_encoders_parameters(){
  if(synthParam){
    long newRight1;
    // Get rotary encoder1 value
    newRight1 = knobRight1.read()/2;
    if (newRight1 != positionRight1) {
      if (newRight1 >= 3){
        knobRight1.write(0);
        newRight1 = 0;
      }
      if (newRight1 < 0){
        newRight1 = 2;
        knobRight1.write(newRight1*2);
      }
      positionRight1 = newRight1;
    }
    if(digital_encsw[0].update()){
      if(digital_encsw[0].fallingEdge()){
        if(kelpiesynthParamMsec <= 300){
          synthParam = false;
        }else{
          switch(newRight1){
            case 0:
              switch(waveform1state){
                case 0:
                globalState.WAVEFORM1 = WAVEFORM_SQUARE;
                waveform1state = 1;
                break;
                case 1:
                globalState.WAVEFORM1 = WAVEFORM_SAWTOOTH;
                waveform1state = 2;
                break;
                case 2:
                globalState.WAVEFORM1 = WAVEFORM_SINE;
                waveform1state = 3;
                break;
                case 3:
                globalState.WAVEFORM1 = WAVEFORM_ARBITRARY;
                waveform1state = 0;
                break;
              }
              for (byte i = 0; i < numPolyVoices; i++)
              {
                polyVoices[i].waveformA.begin(globalState.WAVEFORM1);
              }
              break;
            case 1:
              switch(waveform2state){
                case 0:
                globalState.WAVEFORM2 = WAVEFORM_SQUARE;
                waveform2state = 1;
                break;
                case 1:
                globalState.WAVEFORM2 = WAVEFORM_SAWTOOTH;
                waveform2state = 2;
                break;
                case 2:
                globalState.WAVEFORM2 = WAVEFORM_SINE;
                waveform2state = 3;
                break;
                case 3:
                globalState.WAVEFORM2 = WAVEFORM_ARBITRARY;
                waveform2state = 0;
                break;
              }
              for (byte i = 0; i < numPolyVoices; i++)
              {
                polyVoices[i].waveformB.begin(globalState.WAVEFORM2);
              }
              break;
            case 2:
              globalState.IS_POLY = !globalState.IS_POLY;
              break;
          }
        }
        kelpiesynthParamMsec = 0;
      }
    }
    lcd.setCursor(0, 0);
    lcd.print(kelpieEncoderParam[newRight1]);
    lcd.print(" : ");
    switch(newRight1){
      case 0:
      lcd.print(kelpirWaveforms[waveform1state]);
      lcd.print("       ");
      break;

      case 1:
      lcd.print(kelpirWaveforms[waveform2state]);
      lcd.print("       ");
      break;

      case 2:
      if(globalState.IS_POLY){
        lcd.print("TRUE            ");
      }else{
        lcd.print("FALSE           ");
      }
      break;
    }
  }
}

void kelpieOn(){
  for (int i=0;i<SLIDERS_PINS;i++){
    // update the ResponsiveAnalogRead object every loop
    analog_slide[i].update();
    kelpiedata[i] = analog_slide[i].getValue();
    kelpiedataLag[i] = kelpiedata[i];
    float normalizedKnobVal = (kelpiedataLag[i] * DIV127);
    switch (i)
    {
      case 0: // MASTER VOLUME
        globalState.MASTER_VOL = normalizedKnobVal * MAX_MASTER_GAIN;
        MASTER_GAIN.gain(globalState.MASTER_VOL * globalState.POLY_GAIN_MULTIPLIER);
      break;
      case 1: // OSC BALANCE
        globalState.OSC1_VOL = normalizedKnobVal;
        globalState.OSC2_VOL = 1 - (normalizedKnobVal);
        globalState.OSC_CONSTANT = calculateOscConstant(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL);
        setWaveformLevels(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL, globalState.OSC_CONSTANT);
      break;
      case 2: // OSCILLATOR PWM
        globalState.PWM = 0.1 + 0.4 * (1 - normalizedKnobVal);
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].waveformA.pulseWidth(globalState.PWM);
          polyVoices[i].waveformB.pulseWidth(globalState.PWM);
        }
      break;
      case 3: // NOISE VOLUME
        globalState.NOISE_VOL = normalizedKnobVal;
        globalState.OSC_CONSTANT = calculateOscConstant(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL);
        setWaveformLevels(globalState.OSC1_VOL, globalState.OSC2_VOL, globalState.NOISE_VOL, globalState.OSC_CONSTANT);
      break;
      case 4: // OSC DETUNE
        globalState.DETUNE = calculateDetuneValue(normalizedKnobVal);
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].waveformB.frequency(polyVoices[i].noteFreq * globalState.DETUNE * globalState.PITCH_BEND);
        }
      break;
      case 5: // FILTER FREQUENCY
        globalState.FILTER_FREQ = FILTER_CUTOFF_MAX * pow(normalizedKnobVal, 3);
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].filter.frequency(globalState.FILTER_FREQ);
        }
      break;
      case 6: // FILTER RESONANCE
        globalState.FILTER_Q = (FILTER_Q_MAX * normalizedKnobVal) + 1.1;
        globalState.PREFILTER_GAIN = 1 / globalState.FILTER_Q;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].filter.resonance(globalState.FILTER_Q);
        }
      break;
      case 7: // FILTER DEPTH
        globalState.FILTER_OCTAVE = FILTER_OCTAVE_DEPTH * normalizedKnobVal;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].filter.octaveControl(globalState.FILTER_OCTAVE);
        }
      break;
      case 8: // LFO RATE
        globalState.LFO_FREQ = LFO_FREQ_MAX * pow(normalizedKnobVal, 5);
        LFO.frequency(globalState.LFO_FREQ);
      break;
      case 9: // LFO DESTINATION FILTER
        globalState.LFO_FILTER_GAIN = (normalizedKnobVal);
        LFO_MIXER_FILTER.gain(1, globalState.LFO_FILTER_GAIN);
      break;
      case 10: // LFO DESTINATION AMP
        globalState.LFO_AMP_GAIN = (normalizedKnobVal);
        LFO_MIXER_AMP.gain(1, globalState.LFO_AMP_GAIN);
      break;
      case 11: // AMP ATTACK
      globalState.AMP_ATTACK = AMP_ATTACK_MAX * (normalizedKnobVal);
      if (globalState.AMP_ATTACK < 15) //
      {
        globalState.AMP_ATTACK = 0;
      }
      for (byte i = 0; i < numPolyVoices; i++)
      {
        polyVoices[i].ampEnv.attack(globalState.AMP_ATTACK);
      }
      break;
      case 12: // AMP DECAY
        globalState.AMP_DECAY = AMP_DECAY_MAX * normalizedKnobVal;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].ampEnv.decay(globalState.AMP_DECAY);
        }
      break;
      case 13: // AMP SUSTAIN
        globalState.AMP_SUSTAIN = normalizedKnobVal;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].ampEnv.sustain(globalState.AMP_SUSTAIN);
        }
      break;
      case 14: // AMP RELEASE
        globalState.AMP_RELEASE = AMP_RELEASE_MAX * normalizedKnobVal;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].ampEnv.release(globalState.AMP_RELEASE);
        }
      break;
      case 15: // FILTER ATTACK
            globalState.FILTER_ATTACK = FILTER_ATTACK_MAX * normalizedKnobVal;
      if (globalState.FILTER_ATTACK < 15) //
      {
        globalState.FILTER_ATTACK = 0;
      }
      for (byte i = 0; i < numPolyVoices; i++)
      {
        polyVoices[i].filterEnv.attack(globalState.FILTER_ATTACK);
      }
      break;
      case 16: // FILTER DECAY
        globalState.FILTER_DECAY = FILTER_DECAY_MAX * normalizedKnobVal;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].filterEnv.decay(globalState.FILTER_DECAY);
        }
      break;
      case 17: // FILTER SUSTAIN
        globalState.FILTER_SUSTAIN = normalizedKnobVal;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].filterEnv.sustain(globalState.FILTER_SUSTAIN);
        }
      break;
      case 18: // FILTER RELEASE
        globalState.FILTER_RELEASE = FILTER_RELEASE_MAX * normalizedKnobVal;
        for (byte i = 0; i < numPolyVoices; i++)
        {
          polyVoices[i].filterEnv.release(globalState.FILTER_RELEASE);
        }
      break;
    }
  }
}

void KelpieOnNoteOn(byte channel, byte note, byte velocity) {
  if (note > 23 && note < 108)
  {
    LFO.phase(0); // retrigger LFO on keypress
    float noteGain = pow(float(velocity) * DIV127, VELOCITY_CURVE);

    if (globalState.IS_POLY == true) // depending on mode send to buffer
    {
      keyBuffPoly(note, noteGain, true);
    }
    else
    {
      keyBuffMono(note, noteGain, true);
    }
  }
}

void KelpieOnNoteOff(byte channel, byte note, byte velocity) {
  if (note > 23 && note < 108)
  {
    if (globalState.IS_POLY == true) // depending on mode send to buffer
    {
      keyBuffPoly(note, velocity, false);
    }
    else
    {
      keyBuffMono(note, velocity, false);
    }
  }
}

void kelpie_setup(){
  AudioNoInterrupts();
  kelpieOn();
  for (byte i = 0; i < numPolyVoices; i++)
  {
    polyVoices[i].waveformA.begin(globalState.WAVEFORM1);
    polyVoices[i].waveformA.amplitude(0.33);
    polyVoices[i].waveformA.frequency(82.41);
    polyVoices[i].waveformA.pulseWidth(0.15);

    polyVoices[i].waveformB.begin(globalState.WAVEFORM2);
    polyVoices[i].waveformB.amplitude(0.33);
    polyVoices[i].waveformB.frequency(82.41);
    polyVoices[i].waveformB.pulseWidth(0.15);

    polyVoices[i].noise.amplitude(0.33);

    polyVoices[i].waveformMixer.gain(0, 1.0);
    polyVoices[i].waveformMixer.gain(1, 1.0);
    polyVoices[i].waveformMixer.gain(2, 1.0);

    polyVoices[i].waveformAmplifier.gain(0.0);

    polyVoices[i].ampEnv.attack(globalState.AMP_ATTACK);
    polyVoices[i].ampEnv.decay(globalState.AMP_DECAY);
    polyVoices[i].ampEnv.sustain(globalState.AMP_SUSTAIN);
    polyVoices[i].ampEnv.release(globalState.AMP_RELEASE);

    polyVoices[i].filter.frequency(globalState.FILTER_FREQ);
    polyVoices[i].filter.resonance(globalState.FILTER_Q);
    polyVoices[i].filter.octaveControl(2.0);
  }
  AudioInterrupts();

  DC_OFFSET.amplitude(1.0);
  LFO.amplitude(1.0);
  LFO.frequency(2.0);
  LFO.phase(90);

  LFO_MIXER_FILTER.gain(0, 1); // THIS IS THE AMP THAT ADJUSTS HOW MUCH OF THE LFO IS FED INTO THE FILTER
  LFO_MIXER_FILTER.gain(1, 0);

  for (byte i = 0; i < 4; i++)
  {
    V14_MIX.gain(i, 0.25);
    V58_MIX.gain(i, 0.25);
  }

  ALL_VOICE_MIX.gain(0, 2);
  ALL_VOICE_MIX.gain(1, 2);

  usbMIDI.setHandleNoteOff(KelpieOnNoteOff);
  usbMIDI.setHandleNoteOn(KelpieOnNoteOn);
}

void kelpie_run(){
  if(kelpieParam > 50){
    get_parameters();
    kelpie_get_encoders_parameters();
    kelpieParam = 0;
  }

  if (fps > 24)
  {
    if (PEAK.available())
    {
      fps = 0;
      int monoPeak = PEAK.read() * 100.0;
      Serial.print("|");
      for (int cnt = 0; cnt < 100; cnt++)
      if (cnt < monoPeak)
      {
        Serial.print(">");
      } else if (cnt >= monoPeak && cnt < 99)
      {
        Serial.print(" ");
      } else if (cnt > 99)
      {
        Serial.println("|");
      }
      Serial.print(monoPeak);
      Serial.println();
    }
  }
  usbMIDI.read();
}


#endif
