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
AudioOutputAnalogStereo  dacs1;          //xy=847,355

// Create Audio connections between the components
AudioConnection          patchCord55(playSdWav1, 0, mix1, 0);
AudioConnection          patchCord7(playSdWav2, 0, mix1, 1);
AudioConnection          patchCord6(playSdWav3, 0, mix1, 2);
AudioConnection          patchCord1(playSdWav4, 0, mix2, 0);
AudioConnection          patchCord2(playSdWav5, 0, mix2, 1);
AudioConnection          patchCord3(playSdWav6, 0, mix2, 2);
AudioConnection          patchCord10(mix1, 0, mix3, 0);
AudioConnection          patchCord11(mix2, 0, mix3, 1);
AudioConnection          patchCord12(mix3, 0, dacs1, 0);
AudioConnection          patchCord13(mix3, 0, dacs1, 1);

#endif
