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

Bank banks[30];

const char* _filename = "banks.txt";


void sampleplay(){
  lcd.setCursor(0, 1);
  lcd.print("BANK : ");
  lcd.print(banks[bank_number].name);
  lcd.print("      ");
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

  if(digital_encsw[1].update()){
    if(digital_encsw[1].fallingEdge()){
      sampleParam = !sampleParam;
    }
  }

  // Get rotary encoder2 value
  newRight2 = knobRight2.read()/2;
  if (newRight2 != positionRight2) {
    if (newRight2 > number_banks-1){
      knobRight2.write(0);
      newRight2 = 0;
    }
    if (newRight2 < 0){
      newRight2 = number_banks-1;
      knobRight2.write(newRight2*2);
    }
    if(bank){
      bank_number = newRight2;
    }else{

    }
    positionRight2 = newRight2;
  }
}

void volumeControl(){
  // Read pots + CVs
  for (int i=0;i<SLIDERS_PINS;i++){
    analog_slide[i].update();
    if (analog_slide[i].hasChanged()) {
      int value = analog_slide[i].getValue();
      Serial.print(((float)value/(float)20)+0.1);
      Serial.print("\n");
      switch(i){
        case 0:
        amp1.gain(((float)value/(float)20)+0.1);
        break;

        case 1:
        amp2.gain(((float)value/(float)20)+0.1);
        break;

        case 2:
        amp3.gain(((float)value/(float)20)+0.1);
        break;

        case 3:
        amp4.gain(((float)value/(float)20)+0.1);
        break;

        case 4:
        amp5.gain(((float)value/(float)20)+0.1);
        break;

        case 5:
        amp6.gain(((float)value/(float)20)+0.1);
        break;

        case 6:
        amp7.gain(((float)value/(float)20)+5);
        break;
      }
    }
  }
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

    amp1.gain(0.1);
    amp2.gain(0.1);
    amp3.gain(0.1);
    amp4.gain(0.1);
    amp5.gain(0.1);
    amp6.gain(0.1);
    amp7.gain(5);

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
  lcd.setCursor(0, 1);
  lcd.print(banks[bank_number].name);
  return;
}

void runSamplePlayer(){
  if(sampleParam){
    volumeControl();
  }
  control_sampleplayer();
  sampleplay();
}


#endif
