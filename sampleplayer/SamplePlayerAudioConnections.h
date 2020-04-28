#ifndef Sample_Player_connections_h
#define Sample_Player_connections_h

AudioPlaySdWav           playSdWav1;     //xy=185,396
AudioPlaySdWav           playSdWav2;     //xy=199,472
AudioPlaySdWav           playSdWav3;     //xy=207,519
AudioPlaySdWav           playSdWav4;     //xy=219,562
AudioPlaySdWav           playSdWav5;     //xy=228,200
AudioPlaySdWav           playSdWav6;     //xy=233,333
AudioMixer4              mix1;         //xy=505,293
AudioMixer4              mix2;         //xy=512,420
AudioMixer4              mix3;         //xy=694,351
AudioOutputAnalogStereo  DACS1;          //xy=847,355

// Create Audio connections between the components
AudioConnection          samplepatchCord1(playSdWav1, 0, mix1, 0);
AudioConnection          samplepatchCord2(playSdWav2, 0, mix1, 1);
AudioConnection          samplepatchCord3(playSdWav3, 0, mix1, 2);
AudioConnection          samplepatchCord4(playSdWav4, 0, mix2, 0);
AudioConnection          samplepatchCord5(playSdWav5, 0, mix2, 1);
AudioConnection          samplepatchCord6(playSdWav6, 0, mix2, 2);
AudioConnection          samplepatchCord7(mix1, 0, mix3, 0);
AudioConnection          samplepatchCord8(mix2, 0, mix3, 1);
// AudioConnection          patchCord12(mix3, 0, dacs1, 0);
AudioConnection          samplepatchCord9(mix3, 0, DACS1, 1);

#endif
