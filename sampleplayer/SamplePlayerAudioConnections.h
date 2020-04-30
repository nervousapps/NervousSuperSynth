#ifndef Sample_Player_connections_h
#define Sample_Player_connections_h

AudioPlaySdWav           sampleplaySdWav1;     //xy=185,396
AudioPlaySdWav           sampleplaySdWav2;     //xy=199,472
AudioPlaySdWav           sampleplaySdWav3;     //xy=207,519
AudioPlaySdWav           sampleplaySdWav4;     //xy=219,562
AudioPlaySdWav           sampleplaySdWav5;     //xy=228,200
AudioPlaySdWav           sampleplaySdWav6;     //xy=233,333
AudioMixer4              samplemix1;         //xy=505,293
AudioMixer4              samplemix2;         //xy=512,420
AudioMixer4              samplemix3;         //xy=694,351

// Create Audio connections between the components
AudioConnection          samplepatchCord1(sampleplaySdWav1, 0, samplemix1, 0);
AudioConnection          samplepatchCord2(sampleplaySdWav2, 0, samplemix1, 1);
AudioConnection          samplepatchCord3(sampleplaySdWav3, 0, samplemix1, 2);
AudioConnection          samplepatchCord4(sampleplaySdWav4, 0, samplemix2, 0);
AudioConnection          samplepatchCord5(sampleplaySdWav5, 0, samplemix2, 1);
AudioConnection          samplepatchCord6(sampleplaySdWav6, 0, samplemix2, 2);
AudioConnection          samplepatchCord7(samplemix1, 0, samplemix3, 0);
AudioConnection          samplepatchCord8(samplemix2, 0, samplemix3, 1);
// AudioConnection          patchCord12(mix3, 0, dacs1, 0);
AudioConnection          samplepatchCord9(samplemix3, 0, DACS1, 1);

#endif
