#include "Arduino.h"
#include "ChordOrgan.h"

int chordCV = 0;

void ChordOrgan::setWaveformType(short waveformType) {
    for(int i=0;i<SINECOUNT;i++) {
        oscillator[i]->begin(1.0,FREQ[i],waveformType);
    }
}

void ChordOrgan::setupCustomWaveform(int waveselect) {
    waveselect = (waveselect - 4) % 8;

    const int16_t* wave = waveTables[waveselect];
    for(int i=0;i<SINECOUNT;i++) {
        oscillator[i]->arbitraryWaveform(wave, MAX_FREQ);
    }

    setWaveformType(WAVEFORM_ARBITRARY);
}

void ChordOrgan::updateAmpAndFreq() {
    int16_t* chord = settingsnotes[chordQuant];

    int noteNumber;
    int voiceCount = 0;
    int halfSinecount = SINECOUNT>>1;

    if(stacked) {
        for(int i=0;i < halfSinecount;i++) {
            if (chord[i] != 255) {
                noteNumber = rootQuant + chord[i];
                if(noteNumber < 0) noteNumber = 0;
                if(noteNumber > 127) noteNumber = 127;
                float newFreq = MIDI_TO_FREQ[noteNumber];

                FREQ[i] = newFreq;
                FREQ[i+halfSinecount] = newFreq * stackFreqScale;
                // Serial.println("Stack Freq");
                // Serial.println(FREQ[i]);
                // Serial.println(FREQ[i+halfSinecount]);

                deltaFrequency[i] = newFreq - currentFrequency[i];
                deltaFrequency[i+halfSinecount] = (newFreq * stackFreqScale) - currentFrequency[i];

                voiceCount += 2;
            }
        }
    } else {
        for(int i = 0; i< SINECOUNT; i++){
            if (chord[i] != 255) {
                noteNumber = rootQuant + chord[i];
                if(noteNumber < 0) noteNumber = 0;
                if(noteNumber > 127) noteNumber = 127;
                float newFreq = MIDI_TO_FREQ[noteNumber];

                // TODO : Allow option to choose between jump from current or new?
                //deltaFrequency[i] = newFreq - FREQ[i];
                deltaFrequency[i] = newFreq - currentFrequency[i];

                // Serial.print("Delta ");
                // Serial.print(i);
                // Serial.print(" ");
                // Serial.print(deltaFrequency[i]);
                // Serial.print(" ");
                // Serial.println(newFreq);

                FREQ[i] = newFreq;
                voiceCount++;
            }
        }

    }

    float ampPerVoice = AMP_PER_VOICE[voiceCount-1];
    float totalAmp = 0;

    if(stacked) {
        for (int i = 0; i < halfSinecount; i++){
            if (chord[i] != 255) {
                AMP[i] = ampPerVoice;
                AMP[i + halfSinecount] = ampPerVoice;
                totalAmp += ampPerVoice;
            }
            else{
                AMP[i] = 0.0;
            }
        }
    } else {
        for (int i = 0; i< SINECOUNT; i++){
            if (chord[i] != 255) {
                AMP[i] = ampPerVoice;
                totalAmp += ampPerVoice;
            }
            else{
                AMP[i] = 0.0;
            }
        }
    }
}

void ChordOrgan::selectWaveform(int waveform) {
    waveformPage = waveform >> 2;
    if(waveformPage > 0) {
        flashingWave = true;
        waveformIndicatorTimer = 0;
    }
    // ledWrite(waveform % 4);
    // EEPROM.write(1234, waveform);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waveform : ");
    lcd.print(waveform);

    AudioNoInterrupts();
    if(waveformPage == 0) {
        setWaveformType(wave_type[waveform]);
    } else {
        setupCustomWaveform(waveform);
    }
    AudioInterrupts();
}


void ChordOrgan::updateWaveformLEDs() {
    // // Flash waveform LEDs for custom waves
    // if(waveformPage > 0) {
    //     uint32_t blinkTime = 100 + ((waveformPage - 1) * 300);
    //     if(waveformIndicatorTimer >= blinkTime) {
    //         waveformIndicatorTimer = 0;
    //         flashingWave = !flashingWave;
    //         if(flashingWave) {
    //             ledWrite(waveform % 4);
    //         } else {
    //             ledWrite(15);
    //         }
    //     }
    // }
}

void ChordOrgan::updateFrequencies() {

    if(gliding) {
        // TODO : Replace division with reciprocal multiply.
        float dt = 1.0 - (glideTimer * oneOverGlideTime);
        if(dt < 0.0) {
            dt = 0.0;
            gliding = false;
        }
        // Serial.print("dt ");
        // Serial.print(dt);
        // Serial.print(" ");
        // Serial.println(glideTimer);

        for(int i=0;i<SINECOUNT;i++) {
            currentFrequency[i] = FREQ[i] - (deltaFrequency[i] * dt);
            oscillator[i]->frequency(currentFrequency[i]);
        }
    } else {
        for(int i=0;i<SINECOUNT;i++) {
            oscillator[i]->frequency(FREQ[i]);
        }
    }
}

void ChordOrgan::updateAmps(){
    float waveAmp = WAVEFORM_AMP[waveform];
    chordOrganmixer1.gain(0,AMP[0] * waveAmp);
    chordOrganmixer1.gain(1,AMP[1] * waveAmp);
    chordOrganmixer1.gain(2,AMP[2] * waveAmp);
    chordOrganmixer1.gain(3,AMP[3] * waveAmp);
    chordOrganmixer2.gain(0,AMP[4] * waveAmp);
    chordOrganmixer2.gain(1,AMP[5] * waveAmp);
    chordOrganmixer2.gain(2,AMP[6] * waveAmp);
    chordOrganmixer2.gain(3,AMP[7] * waveAmp);
}

// WRITE A 4 DIGIT BINARY NUMBER TO LED0-LED3
void ChordOrgan::ledWrite(int n){
    // digitalWrite(LED3, HIGH && (n==0));
    // digitalWrite(LED2, HIGH && (n==1));
    // digitalWrite(LED1, HIGH && (n==2));
    // digitalWrite(LED0, HIGH && (n==3));
}

void ChordOrgan::get_parameters(){

    // Read pots + CVs
    for (int i=0;i<hardwarecontrols.SLIDERS_PINS;i++){
      hardwarecontrols.analog_slide[i].update();
      if (hardwarecontrols.analog_slide[i].hasChanged()) {
        int value = hardwarecontrols.analog_slide[i].getValue();
        switch(i){
          case 0:
          lcd.setCursor(0,0);
          lcd.print(value*64);
          chordPot = value*64;
          break;

          case 1:
          rootPot = value*64;
          break;

          case 2:
          rootCV = value*64;
          break;

          case 3:
          lcd.setCursor(0,0);
          lcd.print(value);
          chordOrganenvelope1.attack(value*10);
          break;

          case 4:
          chordOrganenvelope1.decay(value*10);
          break;

          case 5:
          chordOrganenvelope1.sustain(value*10);
          break;

          case 6:
          chordOrganenvelope1.release(value*10);
          break;
        }
      }
    }
    // int chordCV = analogRead(CHORD_CV_PIN);

    // Copy pots and CVs to new value
    chordRaw = chordPot + chordCV;
    chordRaw = constrain(chordRaw, 0, ADC_MAX_VAL - 1);

    rootPot = constrain(rootPot, 0, ADC_MAX_VAL - 1);
    rootCV = constrain(rootCV, 0, ADC_MAX_VAL - 1);

    rootChanged = false;
    // Apply hysteresis and filtering to prevent jittery quantization
    // Thanks to Matthias Puech for this code

    if ((chordRaw > chordRawOld + CHANGE_TOLERANCE) || (chordRaw < chordRawOld - CHANGE_TOLERANCE)){
        chordRawOld = chordRaw;
    }
    else {
        chordRawOld += (chordRaw - chordRawOld) >>5;
        chordRaw = chordRawOld;
    }

    // Do Pot and CV separately
    if ((rootPot > rootPotOld + CHANGE_TOLERANCE) || (rootPot < rootPotOld - CHANGE_TOLERANCE)){
        rootPotOld = rootPot;
        rootChanged = true;
    }
    else {
        rootPotOld += (rootPot - rootPotOld) >>5;
        rootPot = rootPotOld;
    }
    if ((rootCV > rootCVOld + CHANGE_TOLERANCE) || (rootCV < rootCVOld - CHANGE_TOLERANCE)){
        rootCVOld = rootCV;
        rootChanged = true;
    }
    else {
        rootCVOld += (rootCV - rootCVOld) >>5;
        rootCV = rootCVOld;
    }

    chordQuant = map(chordRaw, 0, ADC_MAX_VAL, 0, chordCount);
    if (chordQuant != chordQuantOld){
        changed = true;
        chordQuantOld = chordQuant;
    }

    // Map ADC reading to Note Numbers
    int rootCVQuant = LOW_NOTE;
    if(rootCV > rootClampLow) {
      rootCVQuant = ((rootCV - rootClampLow) * rootMapCoeff) + LOW_NOTE + 1;
    }
    // Use Pot as transpose for CV
    int rootPotQuant = map(rootPot,0,ADC_MAX_VAL,0,48);
    rootQuant = rootCVQuant + rootPotQuant;
    if (rootQuant != rootQuantOld){
        changed = true;
        rootQuantOld = rootQuant;
    }

#ifdef DEBUG_MODE
   if(rootChanged) {
        // printRootInfo(rootPot,rootCV);
   }
#endif

    //    resetSwitch.update();
    //    resetButton = resetSwitch.read();

    if (!flashing){
        // resetCV.update();
        // resetCVRose = resetCV.rose();
        // if (resetCVRose) resetFlash = 0;

        // digitalWrite(RESET_LED, (resetFlash<20));
    }
}

void ChordOrgan::get_encoders_parameters(){
  if(synthParam){
    if(hardwarecontrols.digital_encsw[0].update()){
      if(hardwarecontrols.digital_encsw[0].fallingEdge()){
        if(ChordsynthParamMsec <= 300){
          synthParam = false;
        }else{
          buttonTimer = 0;
        }
      }
      if(hardwarecontrols.digital_encsw[0].risingEdge()){
        if (buttonTimer > SHORT_PRESS_DURATION && lockOut > 999 ){
            shortPress = true;
        }
        if (buttonTimer > LONG_PRESS_DURATION){
            longPress = true;
            lockOut = 0;
            buttonTimer = 0;
        }
      }
      ChordsynthParamMsec = 0;
    }
  }
}

void ChordOrgan::reBoot(int delayTime){
    // if (delayTime > 0)
    //     delay (delayTime);
    // WRITE_RESTART(0x5FA0004);
}

void ChordOrgan::printRootInfo(int rootPot, int rootCV) {
    Serial.print("Root ");
    Serial.print(rootPot);
    Serial.print(" ");
    Serial.print(rootCV);
    Serial.print(" ");
    Serial.println(rootQuant);
}

void ChordOrgan::printPlaying(){
    Serial.print("Chord: ");
    Serial.print(chordQuant);
    Serial.print(" Root: ");
    Serial.print(rootQuant);
    Serial.print(" ");
    for(int i = 0; i<SINECOUNT; i++){
        Serial.print(i);
        Serial.print(": ");
        Serial.print (FREQ[i]);
        Serial.print(" ");
        Serial.print(AMP[i]);
        Serial.print (" | ");
    }
    Serial.println("--");

}

float ChordOrgan::numToFreq(int input) {
    int number = input - 21; // set to midi note numbers = start with 21 at A0
    number = number - 48; // A0 is 48 steps below A4 = 440hz
    return 440*(pow (1.059463094359,number));
}

void ChordOrgan::OnNoteOn(byte channel, byte note, byte velocity) {
  if (note > 23 && note < 108)
  {
    chordCV = note*64;
    chordOrganenvelope1.noteOn();
  }
}

void ChordOrgan::OnNoteOff(byte channel, byte note, byte velocity) {
  if (note > 23 && note < 108)
  {
    chordOrganenvelope1.noteOff();
  }
}

void ChordOrgan::setup(bool hasSD){
    // pinMode(BANK_BUTTON,INPUT);
    // pinMode(RESET_BUTTON, INPUT);
    // pinMode(RESET_CV, INPUT);
    // pinMode(RESET_LED, OUTPUT);
    // pinMode(LED0,OUTPUT);
    // pinMode(LED1,OUTPUT);
    // pinMode(LED2,OUTPUT);
    // pinMode(LED3,OUTPUT);
    // analogReadRes(ADC_BITS);

    oscillator[0] = &chordOrganwaveform1;
    oscillator[1] = &chordOrganwaveform2;
    oscillator[2] = &chordOrganwaveform3;
    oscillator[3] = &chordOrganwaveform4;
    oscillator[4] = &chordOrganwaveform5;
    oscillator[5] = &chordOrganwaveform6;
    oscillator[6] = &chordOrganwaveform7;
    oscillator[7] = &chordOrganwaveform8;

    for(int i=0;i<128;i++) {
        MIDI_TO_FREQ[i] = numToFreq(i);
    }

#ifdef DEBUG_STARTUP
  while( !Serial );

    Serial.println("Starting");
    // ledWrite(waveform);
#endif // DEBUG_STARTUP

    // SD CARD SETTINGS FOR MODULE
    // SPI.setMOSI(7);
    // SPI.setSCK(14);
    //
    // // Read waveform settings from EEPROM
    // waveform = EEPROM.read(1234);

#ifdef DEBUG_STARTUP
    Serial.print("Waveform from EEPROM ");
    Serial.println(waveform);
#endif

#ifdef DEBUG_STARTUP
    Serial.print("Has SD ");
    Serial.println(hasSD);
#endif
    // READ SETTINGS FROM SD CARD
    ChordOrganinit(hasSD);

    chordCount = settingsnumChords;
    waveformPages = settingsextraWaves ? 3 : 1;
    if(waveformPages > 1) {
        waveformPage = waveform >> 2;
    } else {
        // If we read a custom waveform index from EEPROM
        // but they are not enabled in the config then change back to sine
        waveform = 0;
    }

    glide = settingsglide;
    glideTime = settingsglideTime;
    oneOverGlideTime = 1.0 / (float) glideTime;
    noteRange = settingsnoteRange;
    stacked = settingsstacked;

#ifdef DEBUG_STARTUP
    Serial.print("Waveform page ");
    Serial.println(waveformPage);
    Serial.print("Waveform set to ");
    Serial.println(waveform);

    Serial.println("-- Settings --");
    Serial.print("Chord Count ");
    Serial.println(chordCount);
    Serial.print("Waveform Pages ");
    Serial.println(waveformPages);
    Serial.print("Glide ");
    Serial.println(glide);
    Serial.print("Glide Time ");
    Serial.println(glideTime);
    Serial.print("Note Range ");
    Serial.println(noteRange);
    Serial.print("Stacked ");
    Serial.println(stacked);

#endif

    // Setup audio
    for(int i=0;i<SINECOUNT;i++) {
        oscillator[i]->pulseWidth(0.5);
    }

    for(int m=0;m<4;m++) {
        chordOrganmixer1.gain(m,0.25);
        chordOrganmixer2.gain(m,0.25);
    }

    chordOrganmixer3.gain(0,0.49);
    chordOrganmixer3.gain(1,0.49);
    chordOrganmixer3.gain(2,0);
    chordOrganmixer3.gain(3,0);

    chordOrganenvelope1.attack(1);
    chordOrganenvelope1.decay(1);
    chordOrganenvelope1.sustain(1.0);
    chordOrganenvelope1.release(1);
    chordOrganenvelope1.noteOn();

    if(waveformPage == 0) {
        // First page is built in waveforms
        setWaveformType(wave_type[waveform]);
    } else {
        // Second and third pages are arbitrary waves
        setupCustomWaveform(waveform);
        // Start the wave led flashing
        flashingWave = true;
        waveformIndicatorTimer = 0;
    }

    // This makes the CV input range for the low note half the size of the other notes.
    rootClampLow = ((float)ADC_MAX_VAL / noteRange) * 0.5;
    // Now map the rest of the range linearly across the input range
    rootMapCoeff = (float)noteRange / (ADC_MAX_VAL - rootClampLow);

#ifdef DEBUG_STARTUP
    Serial.print("Root Clamp Low ");
    Serial.println(rootClampLow);
    Serial.print("Root Map Coeff ");
    Serial.println(rootMapCoeff * 100);
#endif
}


void ChordOrgan::run(){

    get_parameters();
    get_encoders_parameters();

    if (changed) {

        // Serial.println("Changed");
        updateAmpAndFreq();
        if(glide) {
            glideTimer = 0;
            gliding = true;
            // Serial.println("Start glide");
        }

        #ifdef CHECK_CPU
        int maxCPU = AudioProcessorUsageMax();
        Serial.print("MaxCPU=");
        Serial.println(maxCPU);
        #endif // CHECK_CPU
    }

    // CHECK BUTTON STATUS
    resetHold = resetHold * resetButton;

    if (shortPress){
        waveform++;
        waveform = waveform % (4 * waveformPages);
        selectWaveform(waveform);
        changed = true;
        shortPress = false;
    }

    if (changed)  {
        // Serial.println("Trig Out");
        pulseOutTimer = 0;
        flashing = true;
        // pinMode(RESET_CV, OUTPUT);
        // digitalWrite (RESET_LED, HIGH);
        // digitalWrite (RESET_CV, HIGH);

        AudioNoInterrupts();
        updateFrequencies();
        updateAmps();
        AudioInterrupts();

        changed = false;
    }

    if(gliding) {
        if(glideTimer >= glideTime) {
            gliding = false;
        }
        AudioNoInterrupts();
        updateFrequencies();
        AudioInterrupts();
    }

    // updateWaveformLEDs();

    if (flashing && (pulseOutTimer > flashTime)) {
        // digitalWrite (RESET_LED, LOW);
        // digitalWrite (RESET_CV, LOW);
        // pinMode(RESET_CV, INPUT);
        flashing = false;
    }

    usbMIDI.read();
}
