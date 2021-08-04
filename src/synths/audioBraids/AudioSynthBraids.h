//************LIBRARIES USED**************

#include <Wire.h>
#include "synth_braids.h"
#include <AudioPrivate.h>


AudioSynthBraids         synthBraids;
AudioConnection          BraidspatchCord1(synthBraids, 0, mainMix2, 2);

int color = 0;
int timbre = 0;
int shape = 0;

const char* const shape_values[] = {
    "CSAW",
    "^\x88\x8D_",
    "\x88\x8A\x8C\x8D",
    "FOLD",
    "\x8E\x8E\x8E\x8E",
    "SUB\x8C",
    "SUB\x88",
    "SYN\x8C",
    "SYN\x88",
    "\x88\x88x3",
    "\x8C_x3",
    "/\\x3",
    "SIx3",
    "RING",
    "\x88\x88" "CH",
    "\x8C_CH",
    "/\\CH",
    "SICH",
    "WTCH",
    "\x88\x88x6",
    "\x8C_x6",
    "/\\x6",
    "SIx6",
    "WTx6",
    "\x88\x89\x88\x89",
    "\x88\x88\x8E\x8E",
    "TOY*",
    "ZLPF",
    "ZPKF",
    "ZBPF",
    "ZHPF",
    "VOSM",
    "VOWL",
    "VFOF",
    "HARM",
    "FM  ",
    "FBFM",
    "WTFM",
    "PLUK",
    "BOWD",
    "BLOW",
    "FLUT",
    "BELL",
    "DRUM",
    "KICK",
    "CYMB",
    "SNAR",
    "WTBL",
    "WMAP",
    "WLIN",
#if NUM_BANKS >= 1
    "SAM1",
#endif
#if NUM_BANKS >= 2
    "SAM2",
#endif
#if NUM_BANKS >= 3
    "SAM3",
#endif
#if NUM_BANKS >= 4
    "SAM4",
#endif
    "NOIS",
    "TWNQ",
    "CLKN",
    "CLOU",
    "PRTC",    // "NAME" // For your algorithm
};


// Handles note on events
void braidsOnNoteOn(byte channel, byte note, byte velocity){
    // If the velocity is larger than zero, means that is turning on
    if(velocity)
        synthBraids.set_braids_pitch(note << 7);
}

void braidsChangeShape(byte inputIndex, long value){
  if(value > 12 || value < 1){
    shape = value;
    AudioNoInterrupts();
    synthBraids.set_braids_shape(int(shape));
    AudioInterrupts();
    device->updateLine(1, String(shape_values[int(shape)]));
  }
  else {
    device->updateLine(1, String(shape_values[int(value)]) + " NOT USABLE :(");
  }
}

void braidsChangeMux(byte inputIndex, unsigned int value, int diffToPrevious)
{
  Serial.print((value));
  Serial.print("\n");
  int mapped_value = value << 8;//map(value, 0, 127, 0, 10000);
  if(mapped_value>0){
    if(inputIndex == 0){
      device->updateLine(2, "C:" + String(mapped_value));
      AudioNoInterrupts();
      synthBraids.set_braids_color(mapped_value);
      AudioInterrupts();
    }
    if(inputIndex == 1){
      device->updateLine(2, "T:" + String(mapped_value));
      AudioNoInterrupts();
      synthBraids.set_braids_timbre(mapped_value);
      AudioInterrupts();
    }
  }
}

void braidsOnCV(byte inputIndex, unsigned int value, int diffToPrevious) {
  float pitch = map(value,1024,0, 0.0, 127<<8);
  String line = "";
  switch(inputIndex){
    case 0:
    line = "ColorCV : " + String(pitch);
    AudioNoInterrupts();
    synthBraids.set_braids_color((synthBraids.color + pitch)/2);
    AudioInterrupts();
    break;

    case 1:
    line = "TimbreCV : " + String(pitch);
    AudioNoInterrupts();
    synthBraids.set_braids_timbre((synthBraids.timbre + pitch)/2);
    AudioInterrupts();
    break;

    case 2:
    line = "CV " + String(inputIndex) + " : " + String(value)+ " : " + String(pitch);
    break;

    case 3:
    line = "CV " + String(inputIndex) + " : " + String(value)+ " : " + String(pitch);
    break;
  }
  device->updateLine(2, line);
}

//************SETUP**************
void setupAudioBraids() {
  BraidspatchCord1.connect();
  for(byte i =0; i<3; i++){
    device->setHandleCVChange(i, braidsOnCV);
  }
  device->setHandleMuxControlChange(SLIDE1, braidsChangeMux);
  device->setHandleMuxControlChange(SLIDE1, braidsChangeMux);
  device->setHandleMuxControlChange(SLIDE2, braidsChangeMux);
  device->setHandleEncoderChange(0, braidsChangeShape);
  device->updateEncodeursMaxValue(0, 1-56);
  device->updateEncodeursValue(0, 0);
  MIDI.setHandleNoteOn(braidsOnNoteOn);
  // init the audio object AudioSynthBraids
  synthBraids.init_braids();
  synthBraids.set_braids_shape(MACRO_OSC_SHAPE_CSAW);
}

void stopAudioBraids(){
  BraidspatchCord1.disconnect();
}
