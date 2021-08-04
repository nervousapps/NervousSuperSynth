#ifndef Effects_Processor_h
#define Effects_Processor_h

#include "EffectsProcessorAudioConnections.h"

void effectMuxHandler(byte inputIndex, unsigned int valparam, int diffToPrevious){
  switch(inputIndex){
    case SLIDE10:
    device->updateLine(2, "FREQ -> " + String(((float)valparam/127.0)*500.0));
    effectfilter1.frequency(((float)valparam/127.0)*500.0);
    effectfilter2.frequency(((float)valparam/127.0)*500.0);
    break;

    case POT5:
    device->updateLine(2, "RES -> " + String(((float)valparam/127.0)*5.0));
    effectfilter1.resonance(((float)valparam/127.0)*5.0);
    effectfilter2.resonance(((float)valparam/127.0)*5.0);
    break;
  }
}

void effectPressHandler(byte inputIndex){

}
void effectDoublePressHandler(byte inputIndex){

}
void effectLongPressHandler(byte inputIndex){

}
void effectEncoderHandler(byte inputIndex, long value){

}

void setup_effect_processor(){
  // device->setHandlePress(0, effectPressHandler);
  // device->setHandleDoublePress(0, effectDoublePressHandler);
  // device->setHandleLongPress(0, effectLongPressHandler);
  // device->setHandleEncoderChange(0, effectEncoderHandler);
  device->setHandleMuxControlChange(SLIDE10, effectMuxHandler);
  device->setHandleMuxControlChange(POT5, effectMuxHandler);
}

#endif
