#ifndef Sample_Player_h
#define Sample_Player_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include <playsdresmp.h>
#include "SamplePlayerAudioConnections.h"

File root;
const char* sampleplayerBaseDir = "/SAMPLES/";

volatile boolean directory = true;
volatile boolean sample = false;
elapsedMillis sampleParamMsec;
int sampleplayerState = 1;
int sample_number = 0;

int numFile = 0;
struct Directories{
  const String directory;
  const String files[150];
  int numberFile;
};

char editableBank[2][100] = {NULL};

int numDirectory = 0;
int numDirectorymax = 0;
Directories directoriesList[20];

volatile boolean sampleVolCtrl = false;

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

}

void sampleplay(byte inputIndex){
  if(editableBank[inputIndex] != NULL){
    switch (inputIndex) {
      case 0:
      sampleplaySdWav1.playWav(editableBank[inputIndex]);
      device->updateLine(2, "PLAYER ONE READY");
      break;

      case 1:
      // sampleplaySdWav2.playWav(editableBank[inputIndex]);
      device->updateLine(2, "PLAYER TWO READY");
      break;
    }
  }
}


void setPitch1(byte inputIndex, unsigned int value, int diffToPrevious){
  float rate = (map(value, 0, 127, 0, 1024) - 512.0) / 512.0;
  sampleplaySdWav1.setPlaybackRate(rate);
  Serial.println(rate);
  device->updateLine(2, "PITCH 1 : " + String(rate));
}

void setPitch2(byte inputIndex, unsigned int value, int diffToPrevious){
  float rate = (map(value, 0, 127, 0, 1024) - 512.0) / 512.0;
  // sampleplaySdWav2.setPlaybackRate(rate);
  Serial.println(rate);
  device->updateLine(2, "PITCH 2 : " + String(rate));
}

void encoderHandler(byte inputIndex, long value){
  switch(sampleplayerState){
    case 1:
    sample_number = value;
    device->updateLine(1, "PLAYER -> " + String(sample_number+1));
    break;

    case 2:
    Serial.print(value);
    Serial.print(" : ");
    Serial.println(directoriesList[numDirectory].directory);
    numDirectory = int(value);
    device->updateLine(1, directoriesList[numDirectory].directory);
    break;

    case 3:
    Serial.print(value);
    Serial.print(" : ");
    Serial.println(directoriesList[numDirectory].files[numFile]);
    numFile = value;
    device->updateLine(1, directoriesList[numDirectory].files[numFile]);
    break;
  }
}

void simplePressHandler(byte inputIndex){
  switch(sampleplayerState){
    case 1:
    device->updateEncodeursValue(1, numDirectory-1);
    device->updateEncodeursMaxValue(1, numDirectorymax-1);
    device->updateLine(1, directoriesList[numDirectory].directory);
    sampleplayerState = 2;
    break;

    case 2:
    device->updateEncodeursMaxValue(1, directoriesList[numDirectory].numberFile-1);
    device->updateEncodeursValue(1, numFile);
    device->updateLine(1, directoriesList[numDirectory].files[numFile]);
    sampleplayerState = 3;
    break;

    case 3:
    strcpy(editableBank[sample_number], sampleplayerBaseDir);
    strcat(editableBank[sample_number], directoriesList[numDirectory].directory.c_str());
    strcat(editableBank[sample_number], "/");
    strcat(editableBank[sample_number], directoriesList[numDirectory].files[numFile].c_str());
    Serial.print("SAMPLE : ");
    Serial.println(editableBank[sample_number]);
    device->updateLine(1, "SAMPLE CHARGED");
    device->updateLine(2, String(editableBank[sample_number]));
    break;
  }
}

void doublePressHandler(byte inputIndex){
  switch(sampleplayerState){
    case 1:
    device->updateEncodeursMaxValue(1, 1);
    device->updateEncodeursValue(1, sample_number);
    device->updateLine(1, "PLAYER -> " + String(sample_number));
    break;

    case 2:
    device->updateEncodeursMaxValue(1, 1);
    device->updateEncodeursValue(1, sample_number);
    device->updateLine(1, "PLAYER -> " + String(sample_number));
    sampleplayerState = 1;
    break;

    case 3:
    device->updateEncodeursMaxValue(1, numDirectorymax-1);
    device->updateEncodeursValue(1, numDirectory);
    device->updateLine(1, directoriesList[numDirectory].directory);
    sampleplayerState = 2;
    break;
  }
}

void longPressHandler(byte inputIndex){

}

void listDirectories(File dir) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      directoriesList[numDirectory].numberFile = numFile;
      break;
    }
    if (entry.isDirectory()) {
      directoriesList[numDirectory].directory = entry.name();
      numFile = 0;
      Serial.print(entry.name());
      Serial.println("/");
      listDirectories(entry);
      numDirectory++;
    } else {
      Serial.print("\t\t");
      Serial.println(entry.name());
      // files have sizes, directories do not
      directoriesList[numDirectory].files[numFile] = entry.name();
      numFile ++;
    }
    entry.close();
  }
}

void init_banks(){
  // Set mixers gain
  samplemix1.gain(0, 0.25);
  samplemix1.gain(1, 0.25);
  samplemix1.gain(2, 0.25);
  samplemix1.gain(3, 0.25);
  samplemix2.gain(0, 0.25);
  samplemix2.gain(1, 0.25);
  samplemix2.gain(2, 0.25);
  samplemix2.gain(3, 0.25);

  if (hasSD) {
    // Open file for reading
    File root = SD.open(sampleplayerBaseDir);
    listDirectories(root);

    Serial.println(numDirectory);
    numDirectorymax = numDirectory;
    for(int i = 0; i<numDirectory; i++){
      Serial.print(directoriesList[i].directory);
      Serial.print(" -> ");
      Serial.println(directoriesList[i].numberFile);
      for(int j = 0; j<directoriesList[i].numberFile; j++){
        Serial.print("\t");
        Serial.println(directoriesList[i].files[j]);
      }
    }
    numFile = 0;
    numDirectory = 0;
  }
  return;
}

void sampleMuxHandler(byte inputIndex, unsigned int valparam, int diffToPrevious){
  switch(inputIndex){
    case SLIDE10:
    device->updateLine(2, "FREQ -> " + String(((float)valparam/127.0)*1000.0));
    samplefilter2.frequency(((float)valparam/127.0)*1000.0);
    samplefilter1.frequency(((float)valparam/127.0)*1000.0);
    break;

    case POT5:
    device->updateLine(2, "RES -> " + String(((float)valparam/127.0)*5.0));
    samplefilter2.resonance(((float)valparam/127.0)*5.0);
    samplefilter1.resonance(((float)valparam/127.0)*5.0);
    break;
  }
}

// void activeHP(byte inputeIndex, bool value){
//   if(value){
//     sgtl5000_1.adcHighPassFilterDisable();
//     device->updateLine(2, "HP OFF");
//   }else{
//     sgtl5000_1.adcHighPassFilterEnable();
//     device->updateLine(2, "HP ON");
//   }
// }

void setSamplePlayerHandlers(){
  device->setHandleTrigger(0, sampleplay);
  device->setHandleTrigger(1, sampleplay);

  device->setHandlePress(1, simplePressHandler);
  device->setHandleDoublePress(1, doublePressHandler);
  device->setHandleLongPress(1, longPressHandler);
  device->setHandleEncoderChange(1, encoderHandler);
  device->setHandleMuxControlChange(SLIDE1, setPitch1);
  device->setHandleMuxControlChange(POT1, setPitch1);
  device->setHandleMuxControlChange(SLIDE2, setPitch2);
  device->setHandleMuxControlChange(SLIDE10, sampleMuxHandler);
  device->setHandleMuxControlChange(POT5, sampleMuxHandler);

  // device->setHandleSwitchChange(0, activeHP);

  sampleplaySdWav1.begin();
  sampleplaySdWav1.setPlaybackRate(0.5);
  sampleplaySdWav1.setLoopType(looptype_none);
  sampleplaySdWav1.enableInterpolation(true);
  // sampleplaySdWav2.begin();
  // sampleplaySdWav2.setPlaybackRate(0.5);
  // sampleplaySdWav2.setLoopType(looptype_none);
  // sampleplaySdWav2.enableInterpolation(true);
}

void setup_sampleplayer(){
  init_banks();
  setSamplePlayerHandlers();
  volumeControl();
}

#endif
