//************LIBRARIES USED**************
#include "Arduino.h"
#include <Wire.h>
#include <TeensyThreads.h>

#include "macro_oscillator.h"

boolean first = true;
boolean braidsOn = true;

// initialize required values
//  val -> used for set_parameters
//  msec -> used for the sequencer
//  th1 -> used for thread
volatile int valparam = 0;
elapsedMillis msec = 0;
int th1;

using namespace braids;

MacroOscillator osc;
IntervalTimer myTimer;

const uint32_t kSampleRate = 96000;
const uint16_t kAudioBlockSize = 28;

uint8_t sync_buffer[kAudioBlockSize];
int16_t bufferA[kAudioBlockSize];
int16_t bufferB[kAudioBlockSize];
uint8_t buffer_sel;

volatile uint8_t buffer_index;
volatile uint8_t wait;

// Globals that define the parameters of the oscillator
static volatile int16_t pitch,pre_pitch;
static volatile int16_t timbre;
static volatile int16_t color;
static volatile int16_t shape;
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


// Timer interruption to put the following sample
void putSample(void){
    unsigned int val;

    if(buffer_sel){
        val = ((uint16_t)(bufferB[buffer_index]+0x7FFF))>>4;
    }else{
        val = ((uint16_t)(bufferA[buffer_index]+0x7FFF))>>4;
    }

    // // FOR teensy 3.5
    // analogWrite(A21, val);
    analogWrite(A22, val);
    // FOR teensy 3.2
    // analogWrite(A14, val);

    buffer_index++;

    if(buffer_index>=kAudioBlockSize) {
        wait = 0;
        buffer_index = 0;
        buffer_sel = ~buffer_sel;
    }

}

void init_braids(){
  // Initalizes the buffers to zero
  memset(bufferA, 0, kAudioBlockSize);
  memset(bufferB, 0, kAudioBlockSize);

  // Global used to trigger the next buffer to render
  wait = 0;

  // Initializes the objects
  osc.Init();
  osc.set_shape(MACRO_OSC_SHAPE_GRANULAR_CLOUD);
  osc.set_parameters(0, 0);
  myTimer.begin(putSample,1e6/96000.0);

  pitch = 32 << 7;
}

void main_braids(){
  memset(sync_buffer, 0, sizeof(sync_buffer));

  // If the pitch changes update it
  if(pre_pitch!=pitch){
      osc.set_pitch(pitch);
      pre_pitch = pitch;
  }
  // Get the timbre and color parameters from the ui and set them
  osc.set_parameters(timbre,color);

  // Trims the shape to the valid values
  shape = shape >= MACRO_OSC_SHAPE_LAST ? MACRO_OSC_SHAPE_LAST : shape<0 ? 0 : shape;

  // Sets the shape
  MacroOscillatorShape osc_shape = static_cast<MacroOscillatorShape>(shape);//
  osc.set_shape(osc_shape);

  if(buffer_sel){
      osc.Render(sync_buffer, bufferA, kAudioBlockSize);
  }
  else{
      osc.Render(sync_buffer, bufferB, kAudioBlockSize);
  }

  // Waits until the buffer is ready to render again
  wait = 1;
  while(wait);
}

// thread to get parameters from the analog pins
void braids_get_parameters(){
  if(!sampleParam){
  // while(1){
    for (int i=0;i<3;i++){
      analog_slide[i].update();
      if (analog_slide[i].hasChanged()) {
        valparam = analog_slide[i].getValue();
        if(valparam>0){
          if(i == 1){
            // synthBraids.set_braids_color(val<<6);
            color = valparam << 8;
            Serial.print("\ncolor : \n");
            Serial.print(color);
          }
          if(i == 2){
            // synthBraids.set_braids_timbre((val<<6));
            timbre = valparam << 8;
            Serial.print("\ntimbre : \n");
            Serial.print(timbre);
          }
        }
      }
    }
  //   threads.delay(100);
  // }
  }
}

void braids_off(){
  AudioNoInterrupts();
  MIDI.setHandleNoteOn(nothing);
  myTimer.end();
  braidsOn = false;
  myTimer.end();
  analogWrite(A22, (((uint16_t)(0+0x7FFF))>>4));
  AudioInterrupts();
}

void braids_on(){
  AudioNoInterrupts();
  init_braids();
  braidsOn = true;
  AudioInterrupts();
}

void braids_get_shape(){
  if(synthParam){
    long newRight1;
    // Get rotary encoder1 value
    newRight1 = knobRight1.read()/2;
    if (newRight1 != positionRight1) {
      if (newRight1 > 56){
        knobRight1.write(0);
        newRight1 = 0;
      }
      if (newRight1 < 0){
        newRight1 = 56;
        knobRight1.write(newRight1*2);
      }
      positionRight1 = newRight1;
      displayChange = true;
    }
    if(digital_encsw[0].update()){
      if(digital_encsw[0].fallingEdge()){
        if(kelpiesynthParamMsec <= 300){
          synthParam = false;
          knobRight1.write(synthSelect*2);
          displayChange = true;
        }else{
          if(newRight1 <= 56){
            shape = newRight1;
          }
          kelpiesynthParamMsec = 0;
        }
      }
    }
  }
}

// sequencer
void braidsHandleNoteOn(byte channel, byte note, byte velocity){
  pitch = note << 7;
  osc.Strike();
}

//************SETUP**************
void setup_braids() {
  init_braids();

  MIDI.setHandleNoteOn(braidsHandleNoteOn);
}

//************LOOP**************
void run_braids() {
  braids_get_shape();
  braids_get_parameters();
  main_braids();
}
