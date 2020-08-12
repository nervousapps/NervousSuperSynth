#ifndef Sample_Player_h
#define Sample_Player_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include <Audio.h>
#include "SamplePlayerAudioConnections.h"
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

File root;

boolean directory = true;
boolean sample = false;
int bank_number = 0;
int number_banks;
int number_sample_per_bank = 6;
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

Bank banks[30];

const char* _filename = "banks.txt";

int ampVolnum = 0;
boolean selectingAmp = false;
boolean sampleVolCtrl = false;
float ampVol[7] = {
  0.1,
  0.1,
  0.1,
  0.1,
  0.1,
  0.1,
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


void sampleplay(){
  for (int i=0;i<TRIGGER_PINS;i++){
    digital_trig[i].update();
    if (digital_trig[i].fallingEdge()) {
      switch (i) {
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
  }
}

void control_sampleplayer(){

  // TODO : add editor mode !
  long newRight2;
  // Get rotary encoder2 value
  newRight2 = knobRight2.read()/2;
  if (newRight2 != positionRight2) {
    if(sampleVolCtrl){
      if(selectingAmp){
        if (newRight2 > 6){
          knobRight2.write(0);
          newRight2 = 0;
        }
        if (newRight2 < 0){
          newRight2 = 6;
          knobRight2.write(newRight2*2);
        }
        ampVolnum = newRight2;
      }else{
        ampVol[ampVolnum] = abs(int(newRight2));
      }
    }else{
      if (newRight2 > number_banks-1){
        knobRight2.write(0);
        newRight2 = 0;
      }
      if (newRight2 < 0){
        newRight2 = number_banks-1;
        knobRight2.write(newRight2*2);
      }
      bank_number = newRight2;
    }
    positionRight2 = newRight2;
    displayChange = true;
  }

  // Switch to volume control mode
  if(digital_encsw[1].update()){
    if(digital_encsw[1].fallingEdge()){
      sampleParamMsec = 0;
    }

    if(digital_encsw[1].risingEdge()){
      if(sampleParamMsec < 300){
        if(sampleVolCtrl){
          if(selectingAmp){
            knobRight2.write(ampVol[ampVolnum]*2);
          }else{
            knobRight2.write(ampVolnum*2);
          }
          selectingAmp = !selectingAmp;
        }
      }
      else if(sampleParamMsec >= 600){
        sampleVolCtrl = !sampleVolCtrl;
        selectingAmp = true;
      }
      sampleParamMsec = 0;
      displayChange = true;
    }
  }
}

void volumeControl(){
  amp1.gain(((float)ampVol[0]/(float)20)+0.1);
  amp2.gain(((float)ampVol[1]/(float)20)+0.1);
  amp3.gain(((float)ampVol[2]/(float)20)+0.1);
  amp4.gain(((float)ampVol[3]/(float)20)+0.1);
  amp5.gain(((float)ampVol[4]/(float)20)+0.1);
  amp6.gain(((float)ampVol[5]/(float)20)+0.1);
  amp7.gain(((float)ampVol[6]/(float)20)+10);
}

void init_banks(){
    // Set mixers gain
    samplemix1.gain(0, 0.5);
    samplemix1.gain(1, 0.25);
    samplemix1.gain(2, 0.25);
    samplemix2.gain(0, 0.25);
    samplemix2.gain(1, 0.25);
    samplemix2.gain(2, 0.5);
    samplemix3.gain(0, 0.5);
    samplemix3.gain(1, 0.5);

    amp1.gain(ampVol[0]);
    amp2.gain(ampVol[1]);
    amp3.gain(ampVol[2]);
    amp4.gain(ampVol[3]);
    amp5.gain(ampVol[4]);
    amp6.gain(ampVol[5]);
    amp7.gain(ampVol[6]);

  // if (SD.exists(_filename)) {
    // Open file for reading
    File file = SD.open(_filename);

    // Allocate the memory pool on the stack.
    // Don't forget to change the capacity to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<8192> jsonBuffer;

    // Parse the root object
    auto error = deserializeJson(jsonBuffer, file);

    if (error)
      Serial.println(F("Failed to read file, using default configuration"));

    number_banks = jsonBuffer["banks"].size();
    Serial.println(number_banks);

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
  // }
  return;
}

void runSamplePlayer(){
  if(sampleVolCtrl){
    volumeControl();
  }
  control_sampleplayer();
  sampleplay();
}


#endif
