#ifndef Effects_Processor_connections_h
#define Effects_Processor_connections_h

AudioAmplifier           effectamp1;
AudioAmplifier           effectamp1_1;
AudioFilterStateVariable effectfilter2;
AudioFilterStateVariable effectfilter1;

AudioConnection          effectPatchchCord1(input_i2s, 0, effectfilter1, 0);
AudioConnection          effectPatchchCord2(input_i2s, 1, effectfilter2, 0);
AudioConnection          effectPatchchCord3(effectfilter1, 0, effectamp1, 0);
AudioConnection          effectPatchchCord4(effectfilter2, 0, effectamp1_1, 0);
AudioConnection          effectPatchchCord5(effectamp1, 0, mainMixI2s_L, 1);
AudioConnection          effectPatchchCord6(effectamp1_1, 0, mainMixI2s_R, 1);
AudioConnection          effectPatchchCord7(effectamp1, 0, mainMix8211_L, 1);
AudioConnection          effectPatchchCord8(effectamp1_1, 0, mainMix8211_R, 1);

#endif
