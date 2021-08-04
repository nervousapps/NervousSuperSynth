#ifndef Sample_Player_connections_h
#define Sample_Player_connections_h

AudioPlaySdResmp           sampleplaySdWav1;
// AudioPlaySdResmp           sampleplaySdWav2;
AudioMixer4              samplemix1;
AudioMixer4              samplemix2;
AudioFilterStateVariable samplefilter1;
AudioFilterStateVariable samplefilter2;

AudioConnection          samplepatchCord1(sampleplaySdWav1, 0, samplemix1, 0);
AudioConnection          samplepatchCord2(sampleplaySdWav1, 0, samplemix2, 0);
// AudioConnection          samplepatchCord3(sampleplaySdWav2, 0, samplemix1, 1);
// AudioConnection          samplepatchCord4(sampleplaySdWav2, 1, samplemix2, 1);
// AudioConnection          samplepatchCord5(samplemix1, 0, samplefilter1, 0);
// AudioConnection          samplepatchCord6(samplemix2, 0, samplefilter2, 0);

AudioConnection          samplepatchCord7(samplemix1, 0, mainMixI2s_L, 2);
AudioConnection          samplepatchCord8(samplemix2, 0, mainMixI2s_R, 2);

AudioConnection          samplepatchCord77(samplemix1, 0, mainMix8211_L, 2);
AudioConnection          samplepatchCord88(samplemix2, 0, mainMix8211_R, 2);

#endif
