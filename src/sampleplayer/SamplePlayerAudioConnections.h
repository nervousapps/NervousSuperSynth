#ifndef Sample_Player_connections_h
#define Sample_Player_connections_h

AudioPlaySdWav           sampleplaySdWav1;     //xy=185,396
AudioAmplifier           amp1;
AudioPlaySdWav           sampleplaySdWav2;     //xy=199,472
AudioAmplifier           amp2;
AudioPlaySdWav           sampleplaySdWav3;     //xy=207,519
AudioAmplifier           amp3;
AudioPlaySdWav           sampleplaySdWav4;     //xy=219,562
AudioAmplifier           amp4;
AudioPlaySdWav           sampleplaySdWav5;     //xy=228,200
AudioAmplifier           amp5;
AudioPlaySdWav           sampleplaySdWav6;     //xy=233,333
AudioAmplifier           amp6;
AudioMixer4              samplemix1;         //xy=505,293
AudioMixer4              samplemix2;         //xy=512,420
AudioMixer4              samplemix3;         //xy=694,351
AudioAmplifier           amp7;

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


void sampleplayerStop(){
  // sampleplaySdWav1.stop();     //xy=185,396
  amp1.stop();
  // sampleplaySdWav2.stop();     //xy=199,472
  amp2.stop();
  // sampleplaySdWav3.stop();     //xy=207,519
  amp3.stop();
  // sampleplaySdWav4.stop();     //xy=219,562
  amp4.stop();
  // sampleplaySdWav5.stop();     //xy=228,200
  amp5.stop();
  // sampleplaySdWav6.stop();     //xy=233,333
  amp6.stop();
  samplemix1.stop();         //xy=505,293
  samplemix2.stop();         //xy=512,420
  samplemix3.stop();         //xy=694,351
  amp7.stop();

  samplepatchCord1.disconnect();
  samplepatchCord2.disconnect();
  samplepatchCord3.disconnect();
  samplepatchCord4.disconnect();
  samplepatchCord5.disconnect();
  samplepatchCord6.disconnect();
  samplepatchCord7.disconnect();
  samplepatchCord8.disconnect();
  samplepatchCord9.disconnect();
  samplepatchCord10.disconnect();
  samplepatchCord11.disconnect();
  samplepatchCord12.disconnect();
  samplepatchCord13.disconnect();
  samplepatchCord14.disconnect();
  samplepatchCord15.disconnect();
  samplepatchCord16.disconnect();
  samplepatchCord17.disconnect();
}

void sampleplayerStart(){
  // sampleplaySdWav1.start();     //xy=185,396
  amp1.start();
  // sampleplaySdWav2.start();     //xy=199,472
  amp2.start();
  // sampleplaySdWav3.start();     //xy=207,519
  amp3.start();
  // sampleplaySdWav4.start();     //xy=219,562
  amp4.start();
  // sampleplaySdWav5.start();     //xy=228,200
  amp5.start();
  // sampleplaySdWav6.start();     //xy=233,333
  amp6.start();
  samplemix1.start();         //xy=505,293
  samplemix2.start();         //xy=512,420
  samplemix3.start();         //xy=694,351
  amp7.start();

  samplepatchCord1.connect();
  samplepatchCord2.connect();
  samplepatchCord3.connect();
  samplepatchCord4.connect();
  samplepatchCord5.connect();
  samplepatchCord6.connect();
  samplepatchCord7.connect();
  samplepatchCord8.connect();
  samplepatchCord9.connect();
  samplepatchCord10.connect();
  samplepatchCord11.connect();
  samplepatchCord12.connect();
  samplepatchCord13.connect();
  samplepatchCord14.connect();
  samplepatchCord15.connect();
  samplepatchCord16.connect();
  samplepatchCord17.connect();
}

#endif
