#ifndef Chord_Organ_h
#define Chord_Organ_h

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Display.h>
#include <HardwareControls.h>
#include <HardwarePins.h>
#include <LiquidCrystalFast.h>
// #include <EEPROM.h>
#include "audioConnections.h"
#include "Settings.h"
#include "Waves.h"

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

extern int chordCV;


class ChordOrgan
{
  public:
    ChordOrgan() { };

    void run();
    void setup(bool hasSD);
    static void OnNoteOn(byte channel, byte note, byte velocity);
    static void OnNoteOff(byte channel, byte note, byte velocity);
    float numToFreq(int input);
    void printPlaying();
    void printRootInfo(int rootPot, int rootCV);
    void reBoot(int delayTime);
    void get_encoders_parameters();
    void get_parameters();
    void ledWrite(int n);
    void updateAmps();
    void updateFrequencies();
    void updateWaveformLEDs();
    void selectWaveform(int waveform);
    void updateAmpAndFreq();
    void setupCustomWaveform(int waveselect);
    void setWaveformType(short waveformType);

    HardwareControls hardwarecontrols;


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

    boolean synthParam;

    AudioSynthWaveform* oscillator[8];

  private:
    // LiquidCrystalFast lcd = LiquidCrystalFast(DISPLAY_RS, DISPLAY_RW, DISPLAY_E, DISPLAY_DB4, DISPLAY_DB5, DISPLAY_DB6, DISPLAY_DB7);
};

#endif
