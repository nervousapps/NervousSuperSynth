#ifndef Sample_Player_connections_h
#define Sample_Player_connections_h

AudioPlaySdWav           sampleplaySdWav1;     //xy=185,396
AudioAmplifierPrivate           amp1;
AudioPlaySdWav           sampleplaySdWav2;     //xy=199,472
AudioAmplifierPrivate           amp2;
AudioPlaySdWav           sampleplaySdWav3;     //xy=207,519
AudioAmplifierPrivate           amp3;
AudioPlaySdWav           sampleplaySdWav4;     //xy=219,562
AudioAmplifierPrivate           amp4;
AudioPlaySdWav           sampleplaySdWav5;     //xy=228,200
AudioAmplifierPrivate           amp5;
AudioPlaySdWav           sampleplaySdWav6;     //xy=233,333
AudioAmplifierPrivate           amp6;
AudioMixer4Private              samplemix1;         //xy=505,293
AudioMixer4Private              samplemix2;         //xy=512,420
AudioMixer4Private              samplemix3;         //xy=694,351
AudioAmplifierPrivate           amp7;

// Create Audio connections between the components
AudioConnection          samplepatchCord1(sampleplaySdWav1, 0, amp1, 0);
AudioConnection          samplepatchCord2(amp1, 0, samplemix1, 0);
AudioConnection          samplepatchCord3(sampleplaySdWav2, 0, amp2, 0);
AudioConnection          samplepatchCord4(amp2, 0, samplemix1, 1);
AudioConnection          samplepatchCord5(sampleplaySdWav3, 0, amp3, 0);
AudioConnection          samplepatchCord6(amp3, 0, samplemix1, 2);
AudioConnection          samplepatchCord7(sampleplaySdWav4, 0, amp4, 0);
AudioConnection          samplepatchCord8(amp4, 0, samplemix2, 0);
AudioConnection          samplepatchCord9(sampleplaySdWav5, 0, amp5, 0);
AudioConnection          samplepatchCord10(amp5, 0, samplemix2, 1);
AudioConnection          samplepatchCord11(sampleplaySdWav6, 0, amp6, 0);
AudioConnection          samplepatchCord12(amp6, 0, samplemix2, 2);
AudioConnection          samplepatchCord13(samplemix1, 0, samplemix3, 0);
AudioConnection          samplepatchCord14(samplemix2, 0, samplemix3, 1);
AudioConnection          samplepatchCord15(samplemix3, 0, amp7, 0);
AudioConnection          samplepatchCord16(amp7, 0, pt8211_1, 0);
AudioConnection          samplepatchCord17(amp7, 0, pt8211_1, 1);

#endif
