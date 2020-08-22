// GUItool: begin automatically generated code

AudioSynthWaveform       chordOrganwaveform1;      //xy=215,232
AudioSynthWaveform       chordOrganwaveform2;      //xy=243,295
AudioSynthWaveform       chordOrganwaveform3;      //xy=273,354
AudioSynthWaveform       chordOrganwaveform4;      //xy=292,394
AudioSynthWaveform       chordOrganwaveform5;      //xy=215,232
AudioSynthWaveform       chordOrganwaveform6;      //xy=243,295
AudioSynthWaveform       chordOrganwaveform7;      //xy=273,354
AudioSynthWaveform       chordOrganwaveform8;      //xy=292,394
AudioMixer4Private              chordOrganmixer1;         //xy=424,117
AudioMixer4Private              chordOrganmixer2;         //xy=424,181
AudioMixer4Private              chordOrganmixer3;         //xy=571,84
AudioEffectEnvelopePrivate      chordOrganenvelope1;      //xy=652,281
// AudioOutputAnalog        dac1;           //xy=784,129
AudioConnection          chordOrganpatchCord1(chordOrganwaveform1, 0, chordOrganmixer1, 0);
AudioConnection          chordOrganpatchCord2(chordOrganwaveform2, 0, chordOrganmixer1, 1);
AudioConnection          chordOrganpatchCord7(chordOrganwaveform3, 0, chordOrganmixer1, 2);
AudioConnection          chordOrganpatchCord8(chordOrganwaveform4, 0, chordOrganmixer1, 3);
AudioConnection          chordOrganpatchCord3(chordOrganwaveform5, 0, chordOrganmixer2, 0);
AudioConnection          chordOrganpatchCord4(chordOrganwaveform6, 0, chordOrganmixer2, 1);
AudioConnection          chordOrganpatchCord5(chordOrganwaveform7, 0, chordOrganmixer2, 2);
AudioConnection          chordOrganpatchCord6(chordOrganwaveform8, 0, chordOrganmixer2, 3);
AudioConnection          chordOrganpatchCord9(chordOrganmixer1, 0, chordOrganmixer3, 0);
AudioConnection          chordOrganpatchCord10(chordOrganmixer2, 0, chordOrganmixer3, 1);
AudioConnection          chordOrganpatchCord11(chordOrganmixer3, chordOrganenvelope1);
AudioConnection          chordOrganpatchCord12(chordOrganenvelope1, 0, mainMix, 1);
// GUItool: end automatically generated code

void chordOrgan_AOstop(){
  chordOrganwaveform1.stop();
  chordOrganwaveform2.stop();
  chordOrganwaveform3.stop();
  chordOrganwaveform4.stop();
  chordOrganwaveform5.stop();
  chordOrganwaveform6.stop();
  chordOrganwaveform7.stop();
  chordOrganwaveform8.stop();
  chordOrganmixer1.stop();
  chordOrganmixer2.stop();
  chordOrganmixer3.stop();
  chordOrganenvelope1.stop();
}

void chordOrgan_AOstart(){
  chordOrganwaveform1.start();
  chordOrganwaveform2.start();
  chordOrganwaveform3.start();
  chordOrganwaveform4.start();
  chordOrganwaveform5.start();
  chordOrganwaveform6.start();
  chordOrganwaveform7.start();
  chordOrganwaveform8.start();
  chordOrganmixer1.start();
  chordOrganmixer2.start();
  chordOrganmixer3.start();
  chordOrganenvelope1.start();
}
