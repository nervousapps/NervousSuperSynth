#ifndef Sample_Player_h
#define Sample_Player_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include "SamplePlayerAudioConnections.h"

File root;
const char* sampleplayerBaseDir = "/SAMPLES/";

volatile boolean directory = true;
volatile boolean sample = false;
volatile int bank_number = 0;
volatile int number_banks;
volatile int number_sample_per_bank = 6;
elapsedMillis sampleParamMsec;
boolean sampleplayerOn = false;
int sampleplayerState = 1;
int sample_number = 0;

int numFile = 0;
struct Directories{
  const String directory;
  const String files[150];
  int numberFile;
};

// struct BankEdit {
//   char* sample1;
//   char* sample2;
//   char* sample3;
//   char* sample4;
//   char* sample5;
//   char* sample6;
// };

char editableBank[6][100];

int numDirectory = 0;
int numDirectorymax = 0;
Directories directoriesList[20];

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
  if(sampleplayerOn){
    switch (inputIndex) {
      case 0:
      sampleplaySdWav1.play(editableBank[inputIndex]);
      break;

      case 1:
      sampleplaySdWav2.play(editableBank[inputIndex]);
      break;

      case 2:
      sampleplaySdWav3.play(editableBank[inputIndex]);
      break;

      case 3:
      sampleplaySdWav4.play(editableBank[inputIndex]);
      break;

      case 4:
      sampleplaySdWav5.play(editableBank[inputIndex]);
      break;

      case 5:
      sampleplaySdWav6.play(editableBank[inputIndex]);
      break;
    }
  }else{
    char result[100];
    switch (inputIndex) {
      case 0:
      strcpy(result,sampleplayerBaseDir); // copy string one into the result.
      strcat(result,banks[bank_number].sample1); // append string two to the result.
      sampleplaySdWav1.play(result);
      break;

      case 1:
      strcpy(result,sampleplayerBaseDir); // copy string one into the result.
      strcat(result,banks[bank_number].sample2); // append string two to the result.
      sampleplaySdWav2.play(result);
      break;

      case 2:
      strcpy(result,sampleplayerBaseDir); // copy string one into the result.
      strcat(result,banks[bank_number].sample3); // append string two to the result.
      sampleplaySdWav3.play(result);
      break;

      case 3:
      strcpy(result,sampleplayerBaseDir); // copy string one into the result.
      strcat(result,banks[bank_number].sample4); // append string two to the result.
      sampleplaySdWav4.play(result);
      break;

      case 4:
      strcpy(result,sampleplayerBaseDir); // copy string one into the result.
      strcat(result,banks[bank_number].sample5); // append string two to the result.
      sampleplaySdWav5.play(result);
      break;

      case 5:
      strcpy(result,sampleplayerBaseDir); // copy string one into the result.
      strcat(result,banks[bank_number].sample6); // append string two to the result.
      sampleplaySdWav6.play(result);
      break;
    }
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
    break;
  }
}

void encoderHandler(byte inputIndex, long value){
  if(sampleplayerOn){
    switch(sampleplayerState){
      case 1:
      sample_number = value;
      device->updateLine(2, "PLAYER -> " + String(sample_number+1));
      break;

      case 2:
      Serial.print(value);
      Serial.print(" : ");
      Serial.println(directoriesList[numDirectory].directory);
      numDirectory = int(value);
      device->updateLine(2, directoriesList[numDirectory].directory);
      break;

      case 3:
      Serial.print(value);
      Serial.print(" : ");
      Serial.println(directoriesList[numDirectory].files[numFile]);
      numFile = value;
      device->updateLine(2, directoriesList[numDirectory].files[numFile]);
      break;
    }
  }else{
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
}

void simplePressHandler(byte inputIndex){
  if(sampleplayerOn){
    switch(sampleplayerState){
      case 1:
      device->updateEncodeursValue(1, numDirectory-1);
      device->updateEncodeursMaxValue(1, numDirectorymax-1);
      device->updateLine(2, directoriesList[numDirectory].directory);
      sampleplayerState = 2;
      break;

      case 2:
      device->updateEncodeursMaxValue(1, directoriesList[numDirectory].numberFile-1);
      device->updateEncodeursValue(1, numFile);
      device->updateLine(2, directoriesList[numDirectory].files[numFile]);
      sampleplayerState = 3;
      break;

      case 3:
      device->updateLine(2, directoriesList[numDirectory].files[numFile]);
      strcpy(editableBank[sample_number], sampleplayerBaseDir);
      strcat(editableBank[sample_number], directoriesList[numDirectory].directory.c_str());
      strcat(editableBank[sample_number], "/");
      strcat(editableBank[sample_number], directoriesList[numDirectory].files[numFile].c_str());
      Serial.print("SAMPLE : ");
      Serial.println(editableBank[sample_number]);
      break;
    }
  }else{
    if(sampleVolCtrl){
      if(selectingAmp){
        device->updateEncodeursValue(1, ampVol[ampVolnum]);
      }else{
        device->updateEncodeursValue(1, ampVolnum);
        volumeControl();
      }
      selectingAmp = !selectingAmp;
    }else{
      sampleVolCtrl = true;
    }
    printSamplePlayerLine();
  }
}

void doublePressHandler(byte inputIndex){
  if(sampleplayerOn){
    switch(sampleplayerState){
      case 1:
      device->updateEncodeursMaxValue(1, number_sample_per_bank-1);
      device->updateEncodeursValue(1, sample_number);
      device->updateLine(2, "PLAYER -> " + String(sample_number));
      break;

      case 2:
      device->updateEncodeursMaxValue(1, number_sample_per_bank-1);
      device->updateEncodeursValue(1, sample_number);
      device->updateLine(2, "PLAYER -> " + String(sample_number));
      sampleplayerState = 1;
      break;

      case 3:
      device->updateEncodeursMaxValue(1, numDirectorymax-1);
      device->updateEncodeursValue(1, numDirectory);
      device->updateLine(2, directoriesList[numDirectory].directory);
      sampleplayerState = 2;
      break;
    }
  }else{
    sampleVolCtrl = false;
    selectingAmp = true;
    device->updateEncodeursValue(1, bank_number);
    device->updateLine(2, "BANK : " + String(banks[bank_number].name));
  }
}

void longPressHandler(byte inputIndex){
  sampleplayerOn = !sampleplayerOn;
  if(sampleplayerOn){
    device->updateLine(2, "SampleplayerEDITmode");
    sampleplayerState = 1;
    numFile = 0;
    numDirectory = 0;
    device->updateEncodeursMaxValue(1, number_sample_per_bank-1);
    // sampleplayerStart();
  }else{
    // sampleplayerStop();
    device->updateEncodeursMaxValue(1, number_banks-1);
    device->updateLine(2, "SampleplayerBANKMmode");
  }
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

    // Allocate the memory pool on the stack.
    // Don't forget to change the capacity to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<8192> jsonBuffer;

    // Parse the root object
    File file = SD.open(_filename);
    auto error = deserializeJson(jsonBuffer, file);

    if (error)
    if(debug) Serial.println(F("Failed to read file, using default configuration"));

    number_banks = jsonBuffer["banks"].size();
    if(DEBUG){
      Serial.print("number_banks : ");
      Serial.println(number_banks);
    }

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

void setSamplePlayerHandlers(){
  for(byte i = 0; i < 6; i++){
    device->setHandleTrigger(i, sampleplay);
  }
  device->setHandlePress(1, simplePressHandler);
  device->setHandleDoublePress(1, doublePressHandler);
  device->setHandleLongPress(1, longPressHandler);
  device->setHandleEncoderChange(1, encoderHandler);
}

void initSamplePlayer(){
  init_banks();
  setSamplePlayerHandlers();
  volumeControl();
}


#endif
