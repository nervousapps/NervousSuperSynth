#ifndef Psyc03_h
#define Psyc03_h

/*
// GUItool: begin automatically generated code
AudioSynthWaveformDc     dc1;            //xy=70,83
AudioSynthWaveformSine   sine1;          //xy=73,135
AudioSynthWaveformSine   lfo;          //xy=76,195
AudioAnalyzePeak         peak1;          //xy=161,254
AudioEffectEnvelope      envelope1;      //xy=196,87
AudioSynthWaveformDc     dc2;            //xy=197,427
AudioSynthWaveformModulated sine_fm;        //xy=205,363
AudioMixer4              mixer1;         //xy=257,197
AudioEffectEnvelope      envelope3;      //xy=347,423
AudioEffectEnvelope      envelope2;      //xy=349,366
AudioFilterStateVariable filter;        //xy=516,403
AudioOutputI2S           i2s1;           //xy=735,370
AudioOutputUSB           usb1;           //xy=740,426
AudioConnection          patchCord1(dc1, envelope1);
AudioConnection          patchCord2(sine1, 0, mixer1, 1);
AudioConnection          patchCord3(lfo, peak1);
AudioConnection          patchCord4(envelope1, 0, mixer1, 0);
AudioConnection          patchCord5(dc2, envelope3);
AudioConnection          patchCord6(sine_fm, envelope2);
AudioConnection          patchCord7(mixer1, 0, sine_fm, 0);
AudioConnection          patchCord8(envelope3, 0, filter, 1);
AudioConnection          patchCord9(envelope2, 0, filter, 0);
AudioConnection          patchCord10(filter, 0, i2s1, 0);
AudioConnection          patchCord11(filter, 0, i2s1, 1);
AudioConnection          patchCord12(filter, 0, usb1, 0);
AudioConnection          patchCord13(filter, 0, usb1, 1);
// GUItool: end automatically generated code
*/
#include "Arduino.h"

/*
 * Synth
 */
class Psyc03{

  private:
    static Psyc03 *instance;
    Psyc03();

    AudioSynthWaveformDc        *dc1;
    AudioSynthWaveformSine      *sine1;
    AudioSynthWaveformSine      *lfo;
    AudioAnalyzePeak            *peak1;
    AudioEffectEnvelope         *envelope1;
    AudioSynthWaveformDc        *dc2;
    AudioSynthWaveformModulated *sine_fm;
    AudioMixer4                 *mixer1;
    AudioEffectEnvelope         *envelope3;
    AudioEffectEnvelope         *envelope2;
    AudioFilterStateVariable    *filter;
    AudioConnection* patchCords[10];

    // Output
    AudioMixer4 *output;

    // Motherboard
    // Motherboard9 *device;
    NervousSuperMother *device;

    // Params
    unsigned int tune = 30;
    float depth = 0;
    float speed = 0;
    float lfoPeak = 0;
    int fm = 400;
    int decay = 80;
    int attack = 1;
    float sweep = 0.2;
    int cutoff = 3000;
    float drumFilterRes = 4;
    byte shape = 0;

    byte shapeInputIndex = 0;
    byte tuneInputIndex = 1;
    byte sweepInputIndex = 2;
    byte fmInputIndex = 3;
    byte attackInputIndex = 4;
    byte decayInputIndex = 5;
    byte cutoffInputIndex = 6;
    byte speedInputIndex = 7;
    byte depthInputIndex = 8;

    byte updateMillis = 10;
    elapsedMillis clockUpdate;

  public:
    static Psyc03 *getInstance();
    void init(NervousSuperMother *device);
    static void noteOn(byte channel, byte note, byte velocity);
    static void noteOff(byte channel, byte note, byte velocity);
    void stop();
    void update();
    AudioMixer4 * getOutput();

    // Callbacks
    static void onShapeChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onTuneChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onSweepCHange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onFmChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onAttackChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onDecayChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onCutoffChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onSpeedChange(byte inputIndex, unsigned int value, int diffToPrevious);
    static void onDepth(byte inputIndex, unsigned int value, int diffToPrevious);
};

// Singleton pre init
Psyc03 * Psyc03::instance = nullptr;

/**
 * Constructor
 */
inline Psyc03::Psyc03(){
  this->device = nullptr;

  this->peak1 = new AudioAnalyzePeak();

  this->lfo = new AudioSynthWaveformSine();
  this->lfo->amplitude(1);
  this->lfo->frequency(1);

  this->sine1 = new AudioSynthWaveformSine();
  this->sine1->amplitude(1);
  this->sine1->frequency(this->fm);

  this->sine_fm = new AudioSynthWaveformModulated();
  this->sine_fm->begin(0);
  this->sine_fm->amplitude(.2);
  this->sine_fm->frequency(this->tune);

  this->dc1 = new AudioSynthWaveformDc();
  this->dc1->amplitude(1);

  this->envelope1 = new AudioEffectEnvelope();
  this->envelope1->attack(this->attack);
  this->envelope1->decay(this->decay);
  this->envelope1->sustain(0);
  this->envelope1->hold(1);
  this->envelope1->release(1);

  this->envelope2 = new AudioEffectEnvelope();
  this->envelope2->attack(this->attack);
  this->envelope2->decay(this->decay);
  this->envelope2->sustain(0);
  this->envelope2->hold(1);
  this->envelope2->release(1);

  this->envelope3 = new AudioEffectEnvelope();
  this->envelope3->attack(this->attack);
  this->envelope3->decay(this->decay);
  this->envelope3->sustain(0);
  this->envelope3->hold(1);
  this->envelope3->release(1);

  this->dc2 = new AudioSynthWaveformDc();
  this->dc2->amplitude(1);

  this->filter = new AudioFilterStateVariable();
  this->filter->resonance(this->drumFilterRes);
  this->filter->frequency(this->cutoff);
  this->filter->octaveControl(2.5);

  this->mixer1 = new AudioMixer4();
  this->mixer1->gain(0, this->sweep);
  this->mixer1->gain(1, this->fm);

  this->device = device;

  this->output = new AudioMixer4();
  this->output->gain(0, 1 );

  this->patchCords[0] = new AudioConnection(*this->dc1, *this->envelope1);
  this->patchCords[1] = new AudioConnection(*this->sine1, 0, *this->mixer1, 1);
  this->patchCords[2] = new AudioConnection(*this->lfo, *this->peak1);
  this->patchCords[3] = new AudioConnection(*this->envelope1, 0, *this->mixer1, 0);
  this->patchCords[4] = new AudioConnection(*this->dc2, *this->envelope3);
  this->patchCords[5] = new AudioConnection(*this->sine_fm, *this->envelope2);
  this->patchCords[6] = new AudioConnection(*this->mixer1, 0, *this->sine_fm, 0);
  this->patchCords[7] = new AudioConnection(*this->envelope3, 0, *this->filter, 1);
  this->patchCords[8] = new AudioConnection(*this->envelope2, 0, *this->filter, 0);
  this->patchCords[9] = new AudioConnection(*this->filter, 0, *this->output, 0);
}

/**
 * Singleton instance
 */
inline Psyc03 *Psyc03::getInstance()    {
  if (!instance)
     instance = new Psyc03();
  return instance;
}

inline void Psyc03::init(NervousSuperMother *device){
  this->device = device;

  MIDI.setHandleNoteOn(noteOn);
  MIDI.setHandleNoteOff(noteOff);

  this->device->setHandlePotentiometerChange(0, onShapeChange);
  this->device->setHandlePotentiometerChange(1, onTuneChange);
  this->device->setHandlePotentiometerChange(2, onSweepCHange);
  this->device->setHandlePotentiometerChange(3, onFmChange);
  this->device->setHandlePotentiometerChange(4, onAttackChange);
  this->device->setHandlePotentiometerChange(5, onDecayChange);
  this->device->setHandlePotentiometerChange(6, onCutoffChange);
  this->device->setHandlePotentiometerChange(7, onSpeedChange);
  this->device->setHandlePotentiometerChange(8, onDepth);

  this->peak1 = new AudioAnalyzePeak();

  this->patchCords[0]->connect();
  this->patchCords[1]->connect();
  this->patchCords[2]->connect();
  this->patchCords[3]->connect();
  this->patchCords[4]->connect();
  this->patchCords[5]->connect();
  this->patchCords[6]->connect();
  this->patchCords[7]->connect();
  this->patchCords[8]->connect();
  this->patchCords[9]->connect();

  this->lfo->start();
  this->sine1->start();
  this->sine_fm->start();
  this->dc1->start();
  this->envelope1->start();
  this->envelope2->start();
  this->envelope3->start();
  this->dc2->start();
  this->filter->start();
  this->mixer1->start();
  this->output->start();
}

inline void Psyc03::stop(){
  this->lfo->stop();
  this->sine1->stop();
  this->sine_fm->stop();
  this->dc1->stop();
  this->envelope1->stop();
  this->envelope2->stop();
  this->envelope3->stop();
  this->dc2->stop();
  this->filter->stop();
  this->mixer1->stop();
  this->output->stop();

  this->patchCords[0]->disconnect();
  this->patchCords[1]->disconnect();
  this->patchCords[2]->disconnect();
  this->patchCords[3]->disconnect();
  this->patchCords[4]->disconnect();
  this->patchCords[5]->disconnect();
  this->patchCords[6]->disconnect();
  this->patchCords[7]->disconnect();
  this->patchCords[8]->disconnect();
  this->patchCords[9]->disconnect();
  // delete instance; //= nullptr;
}

/**
 * Note on
 */
inline void Psyc03::noteOn(byte channel, byte note, byte velocity){
  float freq = 440.0 * powf(2.0, (float)(note - 69) * 0.08333333);

  getInstance()->sine_fm->frequency(getInstance()->tune + freq); //lfoPeak*1000
  getInstance()->envelope1->noteOn();
  getInstance()->envelope2->noteOn();
  getInstance()->envelope3->noteOn();
}

/**
 * Note off
 */
inline void Psyc03::noteOff(byte channel, byte note, byte velocity){
}

/**
 * Return the audio output
 */
inline AudioMixer4 * Psyc03::getOutput(){
  return this->output;
}


/**
 * Update
 */
inline void Psyc03::update(){
  if(this->clockUpdate > this->updateMillis){
    // Filter folowwing the envelope
    if (this->peak1->available()) {
      this->lfoPeak = this->peak1->read();
      this->filter->frequency((float)this->cutoff + this->lfoPeak*2000);
    }

    this->clockUpdate = 0;
  }
}


/**
 * On Shape Change
 */
inline void Psyc03::onShapeChange(byte inputIndex, unsigned int value, int diffToPrevious){

  byte shape = map(
    value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    2
  );

  getInstance()->sine_fm->begin(shape);

  getInstance()->device->updateLine(1, "Shape : " + String(shape));
}

/**
 * On Tune Change
 */
inline void Psyc03::onTuneChange(byte inputIndex, unsigned int value, int diffToPrevious){
  // Tune
  unsigned int tune = map(
    value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    500
  );

  getInstance()->tune = tune;

  getInstance()->device->updateLine(1, "Tune : " + String(tune));
}

/**
 * On Sweep Change
 */
inline void Psyc03::onSweepCHange(byte inputIndex, unsigned int value, int diffToPrevious){
  // Sweep
  float sweep = (float)map(
    (float)value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    1
  );

  getInstance()->mixer1->gain(0, sweep);

  getInstance()->device->updateLine(1, "Sweep : " + String(sweep));
}

/**
 * On FM Change
 */
inline void Psyc03::onFmChange(byte inputIndex, unsigned int value, int diffToPrevious){
  float fmLevel = map((float)value, 0, 1023, 0, .6);
  getInstance()->sine1->frequency(value);
  getInstance()->mixer1->gain(1, fmLevel);
  getInstance()->device->updateLine(1, "Fm : " + String(fmLevel));
}

/**
 * On Attack Change
 */
inline void Psyc03::onAttackChange(byte inputIndex, unsigned int value, int diffToPrevious){
  int attack = map(
    value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    500
  );

  getInstance()->envelope2->attack(attack);
  getInstance()->envelope3->attack(attack);

  getInstance()->device->updateLine(1, "Attack : " + String(attack));
}

/**
 * On Decay Change
 */
inline void Psyc03::onDecayChange(byte inputIndex, unsigned int value, int diffToPrevious){
  int decay = map(
    value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    500
  );

  getInstance()->envelope1->decay(decay);
  getInstance()->envelope2->decay(decay);
  getInstance()->envelope3->decay(decay);

  getInstance()->device->updateLine(1, "Decay : " + String(decay));
}

/**
 * On Cutoff Change
 */
inline void Psyc03::onCutoffChange(byte inputIndex, unsigned int value, int diffToPrevious){
  int cutoff = map(
    value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    2000
  );

  getInstance()->cutoff = cutoff;

  getInstance()->device->updateLine(1, "Cutoff : " + String(cutoff));
}

/**
 * On Speed Change
 */
inline void Psyc03::onSpeedChange(byte inputIndex, unsigned int value, int diffToPrevious){
  float speed = (float)map(
    (float)value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    10
  );

  getInstance()->lfo->frequency(speed);

  getInstance()->device->updateLine(1, "Speed : " + String(speed));
}

/**
 * On Depth Change
 */
inline void Psyc03::onDepth(byte inputIndex, unsigned int value, int diffToPrevious){
  float depth = (float)map(
    (float)value,
    getInstance()->device->getAnalogMinValue(),
    getInstance()->device->getAnalogMaxValue(),
    0,
    1
  );

  getInstance()->lfo->amplitude(depth);

  getInstance()->device->updateLine(1, "Depth : " + String(depth));
}
#endif
