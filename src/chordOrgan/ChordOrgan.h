#ifndef Chord_Organ_h
#define Chord_Organ_h

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
// #include <EEPROM.h>

#include "Settings.h"
#include "Waves.h"
#include "chordOrganAudioConnections.h"

// #define DEBUG_STARTUP
// #define DEBUG_MODE
// #define CHECK_CPU

#define CHORD_POT_PIN SLIDE1 // pin for Chord pot
#define CHORD_CV_PIN SLIDE2 // pin for Chord CV
#define ROOT_POT_PIN SLIDE3 // pin for Root Note pot
#define ROOT_CV_PIN SLIDE4 // pin for Root Note CV
#define RESET_BUTTON ENC1_SW // Reset button
// #define RESET_LED 11 // Reset LED indicator
// #define RESET_CV 9 // Reset pulse in / out
#define BANK_BUTTON ENC2_SW // Bank Button
// #define LED0 6
// #define LED1 5
// #define LED2 4
// #define LED3 3

// REBOOT CODES
// #define RESTART_ADDR       0xE000ED0C
// #define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
// #define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

#define ADC_BITS 13
#define ADC_MAX_VAL 8192
#define CHANGE_TOLERANCE 64

#define SINECOUNT 8
#define LOW_NOTE 36

// For arbitrary waveform, required but unused apparently.
#define MAX_FREQ 600

#define SHORT_PRESS_DURATION 10
#define LONG_PRESS_DURATION 1000

int chordCount = 16;

// Target frequency of each oscillator
float FREQ[SINECOUNT] = {
    55,110, 220, 440, 880,1760,3520,7040};

// Total distance between last note and new.
// NOT distance per time step.
float deltaFrequency[SINECOUNT] = {
    0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

// Keep track of current frequency of each oscillator
float currentFrequency[SINECOUNT]  = {
    55,110, 220, 440, 880,1760,3520,7040};

float AMP[SINECOUNT] = {
    0.9, 0.9, 0.9, 0.9,0.9, 0.9, 0.9, 0.9};

// Volume for a single voice for each chord size
float AMP_PER_VOICE[SINECOUNT] = {
  0.4,0.3,0.22,0.2,0.15,0.15,0.13,0.12};

// Store midi note number to frequency in a table
// Later can replace the table for custom tunings / scala support.
float MIDI_TO_FREQ[128];

int chordRaw;
int chordCV = 0;
int chordRawOld;
int chordQuant;
int chordQuantOld;
int chordPot = 0;
int rootCV = 0;
int rootPot = 0;

int rootPotOld;
int rootCVOld;

int rootQuant;
int rootQuantOld;

float rootMapCoeff;

// Root CV Pin readings below this level are clamped to LOW_NOTE
int rootClampLow;

// Flag for either chord or root note change
boolean changed = true;
boolean rootChanged = false;

// Bounce resetCV = Bounce( RESET_CV, 40 );
boolean resetButton = false;
boolean resetCVRose;

elapsedMillis resetHold;
elapsedMillis resetFlash;
int updateCount = 0;

elapsedMillis buttonTimer = 0;
elapsedMillis lockOut = 0;
boolean shortPress = false;
boolean longPress = false;
elapsedMillis pulseOutTimer = 0;
uint32_t flashTime = 10;
boolean flashing = false;

// WAVEFORM
// Default wave types
short wave_type[4] = {
    WAVEFORM_SINE,
    WAVEFORM_SQUARE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_PULSE,
};
// Current waveform index
int waveform = 0;

// Waveform LED
boolean flashingWave = false;
elapsedMillis waveformIndicatorTimer = 0;

int waveformPage = 0;
int waveformPages = 1;

// Custom wavetables
int16_t const* waveTables[8] {
    wave1,
    wave7,
    wave3,
    wave4,

    wave8,
    wave9,
    wave10,
    wave11
};

// Per-waveform amp level
// First 4 are default waves, last 8 are custom wavetables
float WAVEFORM_AMP[12] = {
  0.8,0.6,0.8,0.6,
  0.8,0.8,0.8,0.8,
  0.8,0.8,0.8,0.8,
};

// GLIDE
// Main flag for glide on / off
boolean glide = false;
// msecs glide time.
uint32_t glideTime = 50;
// keep reciprocal
float oneOverGlideTime = 0.02;
// Time since glide started
elapsedMillis glideTimer = 0;
elapsedMillis ChordsynthParamMsec = 0;
// Are we currently gliding notes
boolean gliding = false;

// Stack mode replicates first 4 voices into last 4 with tuning offset
boolean stacked = false;
float stackFreqScale = 1.001;

int noteRange = 38;

// Pointers to waveforms
AudioSynthWaveform* oscillator[8];


void setWaveformType(short waveformType) {
    for(int i=0;i<SINECOUNT;i++) {
        oscillator[i]->begin(1.0,FREQ[i],waveformType);
    }
}

void setupCustomWaveform(int waveselect) {
    waveselect = (waveselect - 4) % 8;

    const int16_t* wave = waveTables[waveselect];
    for(int i=0;i<SINECOUNT;i++) {
        oscillator[i]->arbitraryWaveform(wave, MAX_FREQ);
    }

    setWaveformType(WAVEFORM_ARBITRARY);
}

void updateAmpAndFreq() {
    int16_t* chord = settingsnotes[chordQuant];

    int noteNumber;
    int voiceCount = 0;
    int halfSinecount = SINECOUNT>>1;

    if(stacked) {
        for(int i=0;i < halfSinecount;i++) {
            if (chord[i] != 255) {
                noteNumber = rootQuant + chord[i];
                if(noteNumber < 0) noteNumber = 0;
                if(noteNumber > 127) noteNumber = 127;
                float newFreq = MIDI_TO_FREQ[noteNumber];

                FREQ[i] = newFreq;
                FREQ[i+halfSinecount] = newFreq * stackFreqScale;
                // Serial.println("Stack Freq");
                // Serial.println(FREQ[i]);
                // Serial.println(FREQ[i+halfSinecount]);

                deltaFrequency[i] = newFreq - currentFrequency[i];
                deltaFrequency[i+halfSinecount] = (newFreq * stackFreqScale) - currentFrequency[i];

                voiceCount += 2;
            }
        }
    } else {
        for(int i = 0; i< SINECOUNT; i++){
            if (chord[i] != 255) {
                noteNumber = rootQuant + chord[i];
                if(noteNumber < 0) noteNumber = 0;
                if(noteNumber > 127) noteNumber = 127;
                float newFreq = MIDI_TO_FREQ[noteNumber];

                // TODO : Allow option to choose between jump from current or new?
                //deltaFrequency[i] = newFreq - FREQ[i];
                deltaFrequency[i] = newFreq - currentFrequency[i];

                // Serial.print("Delta ");
                // Serial.print(i);
                // Serial.print(" ");
                // Serial.print(deltaFrequency[i]);
                // Serial.print(" ");
                // Serial.println(newFreq);

                FREQ[i] = newFreq;
                voiceCount++;
            }
        }

    }

    float ampPerVoice = AMP_PER_VOICE[voiceCount-1];
    float totalAmp = 0;

    if(stacked) {
        for (int i = 0; i < halfSinecount; i++){
            if (chord[i] != 255) {
                AMP[i] = ampPerVoice;
                AMP[i + halfSinecount] = ampPerVoice;
                totalAmp += ampPerVoice;
            }
            else{
                AMP[i] = 0.0;
            }
        }
    } else {
        for (int i = 0; i< SINECOUNT; i++){
            if (chord[i] != 255) {
                AMP[i] = ampPerVoice;
                totalAmp += ampPerVoice;
            }
            else{
                AMP[i] = 0.0;
            }
        }
    }
}

void selectWaveform(int waveform) {
    waveformPage = waveform >> 2;
    if(waveformPage > 0) {
        flashingWave = true;
        waveformIndicatorTimer = 0;
    }
    // ledWrite(waveform % 4);
    // EEPROM.write(1234, waveform);

    AudioNoInterrupts();
    if(waveformPage == 0) {
        setWaveformType(wave_type[waveform]);
    } else {
        setupCustomWaveform(waveform);
    }
    AudioInterrupts();
}


void updateWaveformLEDs() {
    // // Flash waveform LEDs for custom waves
    // if(waveformPage > 0) {
    //     uint32_t blinkTime = 100 + ((waveformPage - 1) * 300);
    //     if(waveformIndicatorTimer >= blinkTime) {
    //         waveformIndicatorTimer = 0;
    //         flashingWave = !flashingWave;
    //         if(flashingWave) {
    //             ledWrite(waveform % 4);
    //         } else {
    //             ledWrite(15);
    //         }
    //     }
    // }
}

void updateFrequencies() {

    if(gliding) {
        // TODO : Replace division with reciprocal multiply.
        float dt = 1.0 - (glideTimer * oneOverGlideTime);
        if(dt < 0.0) {
            dt = 0.0;
            gliding = false;
        }
        // Serial.print("dt ");
        // Serial.print(dt);
        // Serial.print(" ");
        // Serial.println(glideTimer);

        for(int i=0;i<SINECOUNT;i++) {
            currentFrequency[i] = FREQ[i] - (deltaFrequency[i] * dt);
            oscillator[i]->frequency(currentFrequency[i]);
        }
    } else {
        for(int i=0;i<SINECOUNT;i++) {
            oscillator[i]->frequency(FREQ[i]);
        }
    }
}

void updateAmps(){
    float waveAmp = WAVEFORM_AMP[waveform];
    chordOrganmixer1.gain(0,AMP[0] * waveAmp);
    chordOrganmixer1.gain(1,AMP[1] * waveAmp);
    chordOrganmixer1.gain(2,AMP[2] * waveAmp);
    chordOrganmixer1.gain(3,AMP[3] * waveAmp);
    chordOrganmixer2.gain(0,AMP[4] * waveAmp);
    chordOrganmixer2.gain(1,AMP[5] * waveAmp);
    chordOrganmixer2.gain(2,AMP[6] * waveAmp);
    chordOrganmixer2.gain(3,AMP[7] * waveAmp);
}

// WRITE A 4 DIGIT BINARY NUMBER TO LED0-LED3
void ledWrite(int n){
    // digitalWrite(LED3, HIGH && (n==0));
    // digitalWrite(LED2, HIGH && (n==1));
    // digitalWrite(LED1, HIGH && (n==2));
    // digitalWrite(LED0, HIGH && (n==3));
}

void checkInterface(){
    if(!sampleParam){
      // Read pots + CVs
      for (int i=0;i<SLIDERS_PINS;i++){
        analog_slide[i].update();
        if (analog_slide[i].hasChanged()) {
          int value = analog_slide[i].getValue();
          switch(i){
            case 0:
            chordPot = value*64;
            break;

            case 1:
            rootPot = value*64;
            break;

            case 2:
            rootCV = value*64;
            break;

            case 3:
            chordOrganenvelope1.attack(value*10);
            break;

            case 4:
            chordOrganenvelope1.decay(value*10);
            break;

            case 5:
            chordOrganenvelope1.sustain(value*10);
            break;

            case 6:
            chordOrganenvelope1.release(value*10);
            break;
          }
        }
      }
    }
    // int chordCV = analogRead(CHORD_CV_PIN);

    // Copy pots and CVs to new value
    chordRaw = chordPot + chordCV;
    chordRaw = constrain(chordRaw, 0, ADC_MAX_VAL - 1);

    rootPot = constrain(rootPot, 0, ADC_MAX_VAL - 1);
    rootCV = constrain(rootCV, 0, ADC_MAX_VAL - 1);

    rootChanged = false;
    // Apply hysteresis and filtering to prevent jittery quantization
    // Thanks to Matthias Puech for this code

    if ((chordRaw > chordRawOld + CHANGE_TOLERANCE) || (chordRaw < chordRawOld - CHANGE_TOLERANCE)){
        chordRawOld = chordRaw;
    }
    else {
        chordRawOld += (chordRaw - chordRawOld) >>5;
        chordRaw = chordRawOld;
    }

    // Do Pot and CV separately
    if ((rootPot > rootPotOld + CHANGE_TOLERANCE) || (rootPot < rootPotOld - CHANGE_TOLERANCE)){
        rootPotOld = rootPot;
        rootChanged = true;
    }
    else {
        rootPotOld += (rootPot - rootPotOld) >>5;
        rootPot = rootPotOld;
    }
    if ((rootCV > rootCVOld + CHANGE_TOLERANCE) || (rootCV < rootCVOld - CHANGE_TOLERANCE)){
        rootCVOld = rootCV;
        rootChanged = true;
    }
    else {
        rootCVOld += (rootCV - rootCVOld) >>5;
        rootCV = rootCVOld;
    }

    chordQuant = map(chordRaw, 0, ADC_MAX_VAL, 0, chordCount);

    if (chordQuant != chordQuantOld){
        changed = true;
        chordQuantOld = chordQuant;
    }

    // Map ADC reading to Note Numbers
    int rootCVQuant = LOW_NOTE;
    if(rootCV > rootClampLow) {
      rootCVQuant = ((rootCV - rootClampLow) * rootMapCoeff) + LOW_NOTE + 1;
    }
    // Use Pot as transpose for CV
    int rootPotQuant = map(rootPot,0,ADC_MAX_VAL,0,48);
    rootQuant = rootCVQuant + rootPotQuant;
    if (rootQuant != rootQuantOld){
        changed = true;
        rootQuantOld = rootQuant;
    }

#ifdef DEBUG_MODE
   if(rootChanged) {
        // printRootInfo(rootPot,rootCV);
   }
#endif

    //    resetSwitch.update();
    //    resetButton = resetSwitch.read();

    if (!flashing){
        // resetCV.update();
        // resetCVRose = resetCV.rose();
        // if (resetCVRose) resetFlash = 0;

        // digitalWrite(RESET_LED, (resetFlash<20));
    }
}

void chord_get_encoders_parameters(){
  if(synthParam){
    if(digital_encsw[0].update()){
      if(digital_encsw[0].fallingEdge()){
        if(ChordsynthParamMsec <= 300){
          synthParam = false;
          displayChange = true;
        }else{
          buttonTimer = 0;
        }
      }
      if(digital_encsw[0].risingEdge()){
        if (buttonTimer > SHORT_PRESS_DURATION && lockOut > 999 ){
            shortPress = true;
        }
        if (buttonTimer > LONG_PRESS_DURATION){
            longPress = true;
            lockOut = 0;
            buttonTimer = 0;
        }
      }
      ChordsynthParamMsec = 0;
    }
  }
}

void reBoot(int delayTime){
    // if (delayTime > 0)
    //     delay (delayTime);
    // WRITE_RESTART(0x5FA0004);
}

void printRootInfo(int rootPot, int rootCV) {
    Serial.print("Root ");
    Serial.print(rootPot);
    Serial.print(" ");
    Serial.print(rootCV);
    Serial.print(" ");
    Serial.println(rootQuant);
}

void printPlaying(){
    Serial.print("Chord: ");
    Serial.print(chordQuant);
    Serial.print(" Root: ");
    Serial.print(rootQuant);
    Serial.print(" ");
    for(int i = 0; i<SINECOUNT; i++){
        Serial.print(i);
        Serial.print(": ");
        Serial.print (FREQ[i]);
        Serial.print(" ");
        Serial.print(AMP[i]);
        Serial.print (" | ");
    }
    Serial.println("--");

}

float numToFreq(int input) {
    int number = input - 21; // set to midi note numbers = start with 21 at A0
    number = number - 48; // A0 is 48 steps below A4 = 440hz
    return 440*(pow (1.059463094359,number));
}

void ChordOrganOnNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note ON");
  if (note > 23 && note < 108)
  {
    chordCV = numToFreq(note);
    chordOrganenvelope1.noteOn();
  }
}

void ChordOrganOnNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Note OFF");
  if (note > 23 && note < 108)
  {
    chordOrganenvelope1.noteOff();
  }
}

void setup_chordOrgan(bool hasSD){
    // pinMode(BANK_BUTTON,INPUT);
    // pinMode(RESET_BUTTON, INPUT);
    // pinMode(RESET_CV, INPUT);
    // pinMode(RESET_LED, OUTPUT);
    // pinMode(LED0,OUTPUT);
    // pinMode(LED1,OUTPUT);
    // pinMode(LED2,OUTPUT);
    // pinMode(LED3,OUTPUT);
    // analogReadRes(ADC_BITS);

    oscillator[0] = &chordOrganwaveform1;
    oscillator[1] = &chordOrganwaveform2;
    oscillator[2] = &chordOrganwaveform3;
    oscillator[3] = &chordOrganwaveform4;
    oscillator[4] = &chordOrganwaveform5;
    oscillator[5] = &chordOrganwaveform6;
    oscillator[6] = &chordOrganwaveform7;
    oscillator[7] = &chordOrganwaveform8;

    for(int i=0;i<128;i++) {
        MIDI_TO_FREQ[i] = numToFreq(i);
    }

#ifdef DEBUG_STARTUP
  while( !Serial );

    Serial.println("Starting");
    // ledWrite(waveform);
#endif // DEBUG_STARTUP

    // SD CARD SETTINGS FOR MODULE
    // SPI.setMOSI(7);
    // SPI.setSCK(14);
    //
    // // Read waveform settings from EEPROM
    // waveform = EEPROM.read(1234);

#ifdef DEBUG_STARTUP
    Serial.print("Waveform from EEPROM ");
    Serial.println(waveform);
#endif

#ifdef DEBUG_STARTUP
    Serial.print("Has SD ");
    Serial.println(hasSD);
#endif
    // READ SETTINGS FROM SD CARD
    ChordOrganinit(hasSD);

    chordCount = settingsnumChords;
    waveformPages = settingsextraWaves ? 3 : 1;
    if(waveformPages > 1) {
        waveformPage = waveform >> 2;
    } else {
        // If we read a custom waveform index from EEPROM
        // but they are not enabled in the config then change back to sine
        waveform = 0;
    }

    glide = settingsglide;
    glideTime = settingsglideTime;
    oneOverGlideTime = 1.0 / (float) glideTime;
    noteRange = settingsnoteRange;
    stacked = settingsstacked;

#ifdef DEBUG_STARTUP
    Serial.print("Waveform page ");
    Serial.println(waveformPage);
    Serial.print("Waveform set to ");
    Serial.println(waveform);

    Serial.println("-- Settings --");
    Serial.print("Chord Count ");
    Serial.println(chordCount);
    Serial.print("Waveform Pages ");
    Serial.println(waveformPages);
    Serial.print("Glide ");
    Serial.println(glide);
    Serial.print("Glide Time ");
    Serial.println(glideTime);
    Serial.print("Note Range ");
    Serial.println(noteRange);
    Serial.print("Stacked ");
    Serial.println(stacked);

#endif

    // Setup audio
    for(int i=0;i<SINECOUNT;i++) {
        oscillator[i]->pulseWidth(0.5);
    }

    for(int m=0;m<4;m++) {
        chordOrganmixer1.gain(m,0.25);
        chordOrganmixer2.gain(m,0.25);
    }

    chordOrganmixer3.gain(0,0.49);
    chordOrganmixer3.gain(1,0.49);
    chordOrganmixer3.gain(2,0);
    chordOrganmixer3.gain(3,0);

    chordOrganenvelope1.attack(1);
    chordOrganenvelope1.decay(1);
    chordOrganenvelope1.sustain(1.0);
    chordOrganenvelope1.release(1);

    if(waveformPage == 0) {
        // First page is built in waveforms
        setWaveformType(wave_type[waveform]);
    } else {
        // Second and third pages are arbitrary waves
        setupCustomWaveform(waveform);
        // Start the wave led flashing
        flashingWave = true;
        waveformIndicatorTimer = 0;
    }

    // This makes the CV input range for the low note half the size of the other notes.
    rootClampLow = ((float)ADC_MAX_VAL / noteRange) * 0.5;
    // Now map the rest of the range linearly across the input range
    rootMapCoeff = (float)noteRange / (ADC_MAX_VAL - rootClampLow);

#ifdef DEBUG_STARTUP
    Serial.print("Root Clamp Low ");
    Serial.println(rootClampLow);
    Serial.print("Root Map Coeff ");
    Serial.println(rootMapCoeff * 100);
#endif
}


void chordOrgan_run(){

    checkInterface();
    chord_get_encoders_parameters();

    if (changed) {

        // Serial.println("Changed");
        updateAmpAndFreq();
        if(glide) {
            glideTimer = 0;
            gliding = true;
            // Serial.println("Start glide");
        }

        #ifdef CHECK_CPU
        int maxCPU = AudioProcessorUsageMax();
        Serial.print("MaxCPU=");
        Serial.println(maxCPU);
        #endif // CHECK_CPU
    }

    // CHECK BUTTON STATUS
    resetHold = resetHold * resetButton;

    if (shortPress){
        waveform++;
        waveform = waveform % (4 * waveformPages);
        selectWaveform(waveform);
        changed = true;
        shortPress = false;
        displayChange = true;
    }

    if (changed)  {
        // Serial.println("Trig Out");
        pulseOutTimer = 0;
        flashing = true;
        // pinMode(RESET_CV, OUTPUT);
        // digitalWrite (RESET_LED, HIGH);
        // digitalWrite (RESET_CV, HIGH);

        AudioNoInterrupts();
        updateFrequencies();
        updateAmps();
        AudioInterrupts();

        changed = false;
    }

    if(gliding) {
        if(glideTimer >= glideTime) {
            gliding = false;
        }
        AudioNoInterrupts();
        updateFrequencies();
        AudioInterrupts();
    }

    // updateWaveformLEDs();

    if (flashing && (pulseOutTimer > flashTime)) {
        // digitalWrite (RESET_LED, LOW);
        // digitalWrite (RESET_CV, LOW);
        // pinMode(RESET_CV, INPUT);
        flashing = false;
    }

    // usbMIDI.read();
}

#endif
