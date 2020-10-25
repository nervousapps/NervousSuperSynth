#ifndef Sample_Player_h
#define Sample_Player_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include "SamplePlayerAudioConnections.h"

File root;

volatile boolean directory = true;
volatile boolean sample = false;
volatile int bank_number = 0;
volatile int number_banks;
volatile int number_sample_per_bank = 6;
elapsedMillis sampleParamMsec;

struct Bank {
  const char* name;
  const char* sample1;
  const char* sample2;
  const char* sample3;
  const char* sample4;
  const char* sample5;
  const char* sample6;
};

volatile Bank banks[30];

const char* _filename = "banks.txt";

volatile int ampVolnum = 0;
volatile boolean selectingAmp = true;
volatile boolean sampleVolCtrl = false;
volatile float ampVol[7] = {
  5,
  5,
  5,
  5,
  5,
  5,
  10
};

String splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void volumeControl(){
  amp1.gain(((float)ampVol[0]/(float)20)+2);
  amp2.gain(((float)ampVol[1]/(float)20)+2);
  amp3.gain(((float)ampVol[2]/(float)20)+2);
  amp4.gain(((float)ampVol[3]/(float)20)+2);
  amp5.gain(((float)ampVol[4]/(float)20)+2);
  amp6.gain(((float)ampVol[5]/(float)20)+2);
  amp7.gain(((float)ampVol[6]/(float)20)+10);
}

void sampleplay(byte inputIndex){
  switch (inputIndex) {
    case 0:
      sampleplaySdWav1.play(banks[bank_number].sample1);
      break;

    case 1:
      sampleplaySdWav2.play(banks[bank_number].sample2);
      break;

    case 2:
      sampleplaySdWav3.play(banks[bank_number].sample3);
      break;

    case 3:
      sampleplaySdWav4.play(banks[bank_number].sample4);
      break;

    case 4:
      sampleplaySdWav5.play(banks[bank_number].sample5);
      break;

    case 5:
      sampleplaySdWav6.play(banks[bank_number].sample6);
      break;
  }
}

void printSamplePlayerLine(){
  switch(ampVolnum){
    case 0:
    device->updateLine(2, splitString(splitString(banks[bank_number].sample1, '/', 1), '.', 0) + " : " + ampVol[ampVolnum]);
    break;

    case 1:
    device->updateLine(2, splitString(splitString(banks[bank_number].sample2, '/', 1), '.', 0) + " : " + ampVol[ampVolnum]);
    break;

    case 2:
    device->updateLine(2, splitString(splitString(banks[bank_number].sample3, '/', 1), '.', 0) + " : " + ampVol[ampVolnum]);
    break;

    case 3:
    device->updateLine(2, splitString(splitString(banks[bank_number].sample4, '/', 1), '.', 0) + " : " + ampVol[ampVolnum]);
    break;

    case 4:
    device->updateLine(2, splitString(splitString(banks[bank_number].sample5, '/', 1), '.', 0) + " : " + ampVol[ampVolnum]);
    break;

    case 5:
    device->updateLine(2, splitString(splitString(banks[bank_number].sample6, '/', 1), '.', 0) + " : " + ampVol[ampVolnum]);
    break;

    case 6:
    device->updateLine(2, "MAINAMP : " + String(ampVol[ampVolnum]));
  }
}

void encoderHandler(byte inputIndex, long value){
  if(sampleVolCtrl){
    if(selectingAmp){
      device->updateEncodeursMaxValue(1, 7);
      ampVolnum = value;
    }else{
      device->updateEncodeursMaxValue(1, 300);
      ampVol[ampVolnum] = abs(int(value));
      volumeControl();
    }
    printSamplePlayerLine();
  }else{
    device->updateEncodeursMaxValue(1, number_banks-1);
    bank_number = value;
    device->updateLine(2, "BANK : " + String(banks[bank_number].name));
  }
}

void simplePressHandler(byte inputIndex){
  if(sampleVolCtrl){
    if(selectingAmp){
      device->updateEncodeursValue(1, ampVol[ampVolnum]);
    }else{
      device->updateEncodeursValue(1, ampVolnum);
      volumeControl();
    }
    selectingAmp = !selectingAmp;
  }else {
    sampleVolCtrl = !sampleVolCtrl;
  }
  printSamplePlayerLine();
}

void doublePressHandler(byte inputIndex){
  sampleVolCtrl = !sampleVolCtrl;
  selectingAmp = true;
  device->updateEncodeursValue(1, bank_number);
  device->updateLine(2, "BANK : " + String(banks[bank_number].name));
}

void longPressHandler(byte inputIndex){

}

void init_banks(){
    // Set mixers gain
    samplemix1.gain(0, 0.25);
    samplemix1.gain(1, 0.25);
    samplemix1.gain(2, 0.25);
    samplemix2.gain(0, 0.25);
    samplemix2.gain(1, 0.25);
    samplemix2.gain(2, 0.25);
    samplemix3.gain(0, 0.25);
    samplemix3.gain(1, 0.25);

    amp1.gain(ampVol[0]);
    amp2.gain(ampVol[1]);
    amp3.gain(ampVol[2]);
    amp4.gain(ampVol[3]);
    amp5.gain(ampVol[4]);
    amp6.gain(ampVol[5]);
    amp7.gain(ampVol[6]);

  if (hasSD) {
    // Open file for reading
    File file = SD.open(_filename);

    // Allocate the memory pool on the stack.
    // Don't forget to change the capacity to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<8192> jsonBuffer;

    // Parse the root object
    auto error = deserializeJson(jsonBuffer, file);

    if (error)
      if(debug) Serial.println(F("Failed to read file, using default configuration"));

    number_banks = jsonBuffer["banks"].size();
    if(debug) Serial.println(number_banks);

    // Copy values from the JsonObject to the Config
    for(int i=0; i<number_banks; i++){
      banks[i].name = jsonBuffer["banks"][i]["name"];
      for(int j=0; j<number_sample_per_bank; j++){
        switch(j){
          case 0:
          banks[i].sample1 = jsonBuffer["banks"][i]["value"][j];
          break;
          case 1:
          banks[i].sample2 = jsonBuffer["banks"][i]["value"][j];
          break;
          case 2:
          banks[i].sample3 = jsonBuffer["banks"][i]["value"][j];
          break;
          case 3:
          banks[i].sample4 = jsonBuffer["banks"][i]["value"][j];
          break;
          case 4:
          banks[i].sample5 = jsonBuffer["banks"][i]["value"][j];
          break;
          case 5:
          banks[i].sample6 = jsonBuffer["banks"][i]["value"][j];
          break;
        }
      }
    }
    // Close the file (File's destructor doesn't close the file)
    file.close();
  }
  return;
}

void setSamplePlayerHandlers(NervousSuperMother *device){
  for(byte i = 0; i < 6; i++){
    device->setHandleTrigger(i, sampleplay);
  }
  device->setHandlePress(1, simplePressHandler);
  device->setHandleDoublePress(1, doublePressHandler);
  device->setHandleEncoderChange(1, encoderHandler);
}

void initSamplePlayer(NervousSuperMother *device){
  init_banks();
  setSamplePlayerHandlers(device);
  volumeControl();
}


#endif
