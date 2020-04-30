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
const int chipSelect = BUILTIN_SDCARD;

boolean directory = true;
boolean sample = false;
boolean bank = true;
int bank_number = 0;
int number_banks;
int number_sample_per_bank = 6;

struct Bank {
  const char* name;
  const char* sample1;
  const char* sample2;
  const char* sample3;
  const char* sample4;
  const char* sample5;
  const char* sample6;
};

Bank banks[16];

const char* _filename = "banks.txt";


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
  long newRight2;

  // Update bank or editor mode
  analog_sw[2].update();
  if(analog_sw[2].hasChanged()){
    if(analog_sw[2].getValue() > 90){
      bank = true;
    }else{
      bank = false;
    }
  }

  // Get rotary encoder2 value
  newRight2 = knobRight2.read()/2;
  if (newRight2 != positionRight2) {
    if (newRight2 >= number_banks-1){
      knobRight2.write(0);
      newRight2 = 0;
    }
    if (newRight2 < 0){
      newRight2 = number_banks-1;
      knobRight2.write(newRight2*2);
    }
    if(bank){
      bank_number = newRight2;
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(banks[bank_number].name);
      lcd.setCursor(0, 0);
      lcd.print(banks[bank_number].sample2);
      Serial.print(banks[bank_number].sample2);
    }else{

    }
    positionRight2 = newRight2;
  }
}

void init_banks(){
    // Set mixers gain
    samplemix1.gain(0, 3);
    samplemix1.gain(1, 3);
    samplemix2.gain(0, 3);
    samplemix2.gain(1, 3);
    samplemix2.gain(2, 3);
    samplemix2.gain(3, 3);
    samplemix3.gain(0, 3);
    samplemix3.gain(1, 3);
  // if (SD.exists(_filename)) {
    // Open file for reading
    File file = SD.open(_filename);

    // Allocate the memory pool on the stack.
    // Don't forget to change the capacity to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<4096> jsonBuffer;

    // Parse the root object
    auto error = deserializeJson(jsonBuffer, file);

    if (error)
      Serial.println(F("Failed to read file, using default configuration"));

    number_banks = jsonBuffer["banks"].size()-1;

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


#endif
