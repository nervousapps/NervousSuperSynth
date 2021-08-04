#ifndef Tsynth_Audio_Connections_h
#define Tsynth_Audio_Connections_h

// GUItool: begin automatically generated code
// waveformMixer1 --> filterMixer --> voiceMixer --> voiceMixerM --> volumeMixer --> EffectsMixer
// oscLevel                            velocity

#include <Arduino.h>
#include <AudioPrivate.h>

AudioSynthWaveformDc     constant1Dc;    //xy=69.5,437
AudioSynthNoisePink      pink;           //xy=268,349
AudioSynthNoiseWhite     white;           //xy=268,359
AudioMixer4              noiseMixer;       //xy=288,369
AudioSynthWaveformTS       pwmLfo;         //xy=92,208
AudioSynthWaveformDc     pwa;            //xy=107,256
AudioSynthWaveformDc     pwb;            //xy=110,301
AudioAnalyzePeak peak;
AudioEffectEnvelope      filterEnvelope6;   //xy=629,1107
AudioEffectEnvelope      filterEnvelope5;   //xy=638,300
AudioEffectEnvelope      filterEnvelope4;   //xy=617,1107
AudioEffectEnvelope      filterEnvelope1;   //xy=628,300
AudioEffectEnvelope      filterEnvelope3;   //xy=636,857
AudioEffectEnvelope      filterEnvelope2;   //xy=640,604
AudioMixer4              pwMixer1a;       //xy=281,169
AudioMixer4              pwMixer1b;       //xy=287,272
AudioMixer4              pwMixer4b;         //xy=313.75,954.75
AudioMixer4              pwMixer4a;         //xy=314.75,887.75
AudioMixer4              pwMixer2a;         //xy=323.75,472.75
AudioMixer4              pwMixer2b;         //xy=323.75,553.75
AudioMixer4              pwMixer3a;         //xy=353.75,688.75
AudioMixer4              pwMixer3b;         //xy=358.75,763.75
AudioMixer4              pwMixer5a;         //xy=323.75,472.75
AudioMixer4              pwMixer5b;         //xy=323.75,553.75
AudioMixer4              pwMixer6a;         //xy=353.75,688.75
AudioMixer4              pwMixer6b;         //xy=358.75,763.75
AudioSynthWaveformTS       pitchLfo;         //xy=122,111
AudioSynthWaveformDc     glide1;          //xy=124,152
AudioSynthWaveformDc     glide2;          //xy=124,152
AudioSynthWaveformDc     glide3;          //xy=124,152
AudioSynthWaveformDc     glide4;          //xy=124,152
AudioSynthWaveformDc     glide5;          //xy=124,152
AudioSynthWaveformDc     glide6;          //xy=124,152
AudioSynthWaveformDc     pitchBend;      //xy=128,74
AudioSynthWaveformTS       filterLfo;         //xy=166,632
AudioSynthWaveformDc     keytracking1;    //xy=176,745
AudioSynthWaveformDc     keytracking2;    //xy=196,745
AudioSynthWaveformDc     keytracking3;    //xy=216,745
AudioSynthWaveformDc     keytracking4;    //xy=236,745
AudioSynthWaveformDc     keytracking5;    //xy=216,745
AudioSynthWaveformDc     keytracking6;    //xy=236,745
AudioMixer4              oscGlobalModMixer;    //xy=296,95
AudioMixer4              oscModMixer1a;         //xy=550,165
AudioMixer4              oscModMixer1b;         //xy=550,195
AudioMixer4              oscModMixer2a;         //xy=550,205
AudioMixer4              oscModMixer2b;         //xy=550,225
AudioMixer4              oscModMixer3a;         //xy=550,245
AudioMixer4              oscModMixer3b;         //xy=550,265
AudioMixer4              oscModMixer4a;         //xy=550,285
AudioMixer4              oscModMixer4b;         //xy=550,305
AudioMixer4              oscModMixer5a;         //xy=550,245
AudioMixer4              oscModMixer5b;         //xy=550,265
AudioMixer4              oscModMixer6a;         //xy=550,285
AudioMixer4              oscModMixer6b;         //xy=550,305
AudioSynthWaveformModulatedTS waveformMod1b;  //xy=503,170
AudioSynthWaveformModulatedTS waveformMod1a;  //xy=507,114
AudioSynthWaveformModulatedTS waveformMod2b;  //xy=513,550
AudioSynthWaveformModulatedTS waveformMod4a;  //xy=519,899
AudioSynthWaveformModulatedTS waveformMod2a;  //xy=521,458
AudioSynthWaveformModulatedTS waveformMod4b;  //xy=525,940
AudioSynthWaveformModulatedTS waveformMod3a;  //xy=532,678
AudioSynthWaveformModulatedTS waveformMod3b;  //xy=535,744
AudioSynthWaveformModulatedTS waveformMod5a;  //xy=521,458
AudioSynthWaveformModulatedTS waveformMod5b;  //xy=525,940
AudioSynthWaveformModulatedTS waveformMod6a;  //xy=532,678
AudioSynthWaveformModulatedTS waveformMod6b;  //xy=535,744
AudioEffectDigitalCombine oscFX1;       //xy=663,223
AudioEffectDigitalCombine oscFX2;       //xy=684,542
AudioEffectDigitalCombine oscFX4;       //xy=686,1046
AudioEffectDigitalCombine oscFX3;       //xy=687,803
AudioEffectDigitalCombine oscFX5;       //xy=686,1046
AudioEffectDigitalCombine oscFX6;       //xy=687,803
AudioMixer4              waveformMixer1; //xy=824,170
AudioMixer4              waveformMixer2; //xy=829,476
AudioMixer4              waveformMixer3; //xy=846,774
AudioMixer4              waveformMixer4; //xy=854,1023
AudioMixer4              waveformMixer5; //xy=846,774
AudioMixer4              waveformMixer6; //xy=854,1023
AudioMixer4              filterModMixer1;   //xy=845,292
AudioMixer4              filterModMixer2;   //xy=848,637
AudioMixer4              filterModMixer3;   //xy=852,937
AudioMixer4              filterModMixer4;   //xy=855,1099
AudioMixer4              filterModMixer5;   //xy=852,937
AudioMixer4              filterModMixer6;   //xy=855,1099
AudioFilterStateVariable filter2;        //xy=994,498
AudioFilterStateVariable filter1;        //xy=1000,210
AudioFilterStateVariable filter3;        //xy=1002,822
AudioFilterStateVariable filter4;        //xy=1022,1047
AudioFilterStateVariable filter5;        //xy=1002,822
AudioFilterStateVariable filter6;        //xy=1022,1047
AudioMixer4              filterMixer2;   //xy=1144,504
AudioMixer4              filterMixer3;   //xy=1144,825
AudioMixer4              filterMixer1;   //xy=1151,214
AudioMixer4              filterMixer4;   //xy=1155,1050
AudioMixer4              filterMixer5;   //xy=1151,214
AudioMixer4              filterMixer6;   //xy=1155,1050
AudioEffectEnvelope      ampEnvelope2;   //xy=1315,503
AudioEffectEnvelope      ampEnvelope3;   //xy=1315,823
AudioEffectEnvelope      ampEnvelope4;   //xy=1321,1045
AudioEffectEnvelope      ampEnvelope1;   //xy=1327,211
AudioEffectEnvelope      ampEnvelope5;   //xy=1321,1045
AudioEffectEnvelope      ampEnvelope6;   //xy=1327,211
AudioMixer4              voiceMixer1;     //xy=1524,570
AudioMixer4              voiceMixer2;     //xy=1524,570
AudioMixer4              voiceMixerM;     //xy=1544,580
AudioFilterStateVariable dcOffsetFilter;     //xy=1564,580
AudioMixer4              volumeMixer;     //xy=1544,580
AudioEffectEnsemble       ensemble;  //xy=1800,600
// Oscilloscope              scope;
AudioMixer4              effectMixerR;         //xy=1848,625
AudioMixer4              effectMixerL;         //xy=1857,539
// AudioOutputI2S           i2s;            //xy=2364,547
// AudioOutputUSB           usbAudio;       //xy=2356,593
AudioConnection          TsynthpatchCord1(constant1Dc, filterEnvelope2);
AudioConnection          TsynthpatchCord2(constant1Dc, filterEnvelope3);
AudioConnection          TsynthpatchCord3(constant1Dc, filterEnvelope4);
AudioConnection          TsynthpatchCord4(constant1Dc, filterEnvelope1);
AudioConnection          TsynthpatchCord5(pwmLfo, 0, pwMixer1a, 0);
AudioConnection          TsynthpatchCord6(pwmLfo, 0, pwMixer1b, 0);
AudioConnection          TsynthpatchCord7(pwmLfo, 0, pwMixer2a, 0);
AudioConnection          TsynthpatchCord8(pwmLfo, 0, pwMixer2b, 0);
AudioConnection          TsynthpatchCord9(pwmLfo, 0, pwMixer3a, 0);
AudioConnection          TsynthpatchCord10(pwmLfo, 0, pwMixer3b, 0);
AudioConnection          TsynthpatchCord11(pwmLfo, 0, pwMixer4a, 0);
AudioConnection          TsynthpatchCord12(pwmLfo, 0, pwMixer4b, 0);
AudioConnection          TsynthpatchCord13(pwa, 0, pwMixer1a, 1);
AudioConnection          TsynthpatchCord14(pwa, 0, pwMixer2a, 1);
AudioConnection          TsynthpatchCord15(pwa, 0, pwMixer3a, 1);
AudioConnection          TsynthpatchCord16(pwa, 0, pwMixer4a, 1);
AudioConnection          TsynthpatchCord17(pwb, 0, pwMixer1b, 1);
AudioConnection          TsynthpatchCord18(pwb, 0, pwMixer2b, 1);
AudioConnection          TsynthpatchCord19(pwb, 0, pwMixer3b, 1);
AudioConnection          TsynthpatchCord20(pwb, 0, pwMixer4b, 1);
AudioConnection          TsynthpatchCord21(pitchLfo, 0, oscGlobalModMixer, 1);
AudioConnection          TsynthpatchCord23(pitchBend, 0, oscGlobalModMixer, 0);
AudioConnection          TsynthpatchCord24(filterLfo, 0, filterModMixer1, 1);
AudioConnection          TsynthpatchCord25(filterLfo, 0, filterModMixer2, 1);
AudioConnection          TsynthpatchCord26(filterLfo, 0, filterModMixer3, 1);
AudioConnection          TsynthpatchCord27(filterLfo, 0, filterModMixer4, 1);
AudioConnection          TsynthpatchCord28(keytracking1, 0, filterModMixer1, 2);
AudioConnection          TsynthpatchCord29(keytracking2, 0, filterModMixer2, 2);
AudioConnection          TsynthpatchCord30(keytracking3, 0, filterModMixer3, 2);
AudioConnection          TsynthpatchCord31(keytracking4, 0, filterModMixer4, 2);
AudioConnection          TsynthpatchCord32(noiseMixer, 0, waveformMixer1, 2);
AudioConnection          TsynthpatchCord33(noiseMixer, 0, waveformMixer2, 2);
AudioConnection          TsynthpatchCord34(noiseMixer, 0, waveformMixer3, 2);
AudioConnection          TsynthpatchCord35(noiseMixer, 0, waveformMixer4, 2);
AudioConnection          TsynthpatchCord36(pwMixer1a, 0, waveformMod1a, 1);
AudioConnection          TsynthpatchCord37(pwMixer1b, 0, waveformMod1b, 1);
AudioConnection          TsynthpatchCord38(pwMixer3b, 0, waveformMod3b, 1);
AudioConnection          TsynthpatchCord39(oscGlobalModMixer, 0, oscModMixer1a, 0);
AudioConnection          TsynthpatchCord40(oscGlobalModMixer, 0, oscModMixer1b, 0);
AudioConnection          TsynthpatchCord41(oscGlobalModMixer, 0, oscModMixer2a, 0);
AudioConnection          TsynthpatchCord42(oscGlobalModMixer, 0, oscModMixer2b, 0);
AudioConnection          TsynthpatchCord43(oscGlobalModMixer, 0, oscModMixer3a, 0);
AudioConnection          TsynthpatchCord44(oscGlobalModMixer, 0, oscModMixer3b, 0);
AudioConnection          TsynthpatchCord45(oscGlobalModMixer, 0, oscModMixer4a, 0);
AudioConnection          TsynthpatchCord46(oscGlobalModMixer, 0, oscModMixer4b, 0);
AudioConnection          TsynthpatchCord47(pwMixer4b, 0, waveformMod4b, 1);
AudioConnection          TsynthpatchCord48(pwMixer4a, 0, waveformMod4a, 1);
AudioConnection          TsynthpatchCord49(pwMixer2a, 0, waveformMod2a, 1);
AudioConnection          TsynthpatchCord50(pwMixer2b, 0, waveformMod2b, 1);
AudioConnection          TsynthpatchCord51(pwMixer3a, 0, waveformMod3a, 1);
AudioConnection          TsynthpatchCord52(waveformMod1b, 0, waveformMixer1, 1);
AudioConnection          TsynthpatchCord53(waveformMod1b, 0, oscFX1, 1);
AudioConnection          TsynthpatchCord54(waveformMod1a, 0, waveformMixer1, 0);
AudioConnection          TsynthpatchCord55(waveformMod1a, 0, oscFX1, 0);
AudioConnection          TsynthpatchCord56(waveformMod2b, 0, waveformMixer2, 1);
AudioConnection          TsynthpatchCord57(waveformMod2b, 0, oscFX2, 1);
AudioConnection          TsynthpatchCord58(waveformMod4a, 0, waveformMixer4, 0);
AudioConnection          TsynthpatchCord59(waveformMod4a, 0, oscFX4, 0);
AudioConnection          TsynthpatchCord60(waveformMod2a, 0, waveformMixer2, 0);
AudioConnection          TsynthpatchCord61(waveformMod2a, 0, oscFX2, 0);
AudioConnection          TsynthpatchCord62(waveformMod4b, 0, waveformMixer4, 1);
AudioConnection          TsynthpatchCord63(waveformMod4b, 0, oscFX4, 1);
AudioConnection          TsynthpatchCord64(waveformMod3a, 0, oscFX3, 0);
AudioConnection          TsynthpatchCord65(waveformMod3a, 0, waveformMixer3, 0);
AudioConnection          TsynthpatchCord66(waveformMod3b, 0, oscFX3, 1);
AudioConnection          TsynthpatchCord67(waveformMod3b, 0, waveformMixer3, 1);
AudioConnection          TsynthpatchCord68(filterEnvelope4, 0, filterModMixer4, 0);
AudioConnection          TsynthpatchCord69(filterEnvelope4, 0, pwMixer4a, 2);
AudioConnection          TsynthpatchCord70(filterEnvelope4, 0, pwMixer4b, 2);
AudioConnection          TsynthpatchCord71(filterEnvelope1, 0, filterModMixer1, 0);
AudioConnection          TsynthpatchCord72(filterEnvelope1, 0, pwMixer1a, 2);
AudioConnection          TsynthpatchCord73(filterEnvelope1, 0, pwMixer1b, 2);
AudioConnection          TsynthpatchCord74(filterEnvelope3, 0, filterModMixer3, 0);
AudioConnection          TsynthpatchCord75(filterEnvelope3, 0, pwMixer3a, 2);
AudioConnection          TsynthpatchCord76(filterEnvelope3, 0, pwMixer3b, 2);
AudioConnection          TsynthpatchCord77(filterEnvelope2, 0, filterModMixer2, 0);
AudioConnection          TsynthpatchCord78(filterEnvelope2, 0, pwMixer2a, 2);
AudioConnection          TsynthpatchCord79(filterEnvelope2, 0, pwMixer2b, 2);
AudioConnection          TsynthpatchCord80(oscFX1, 0, waveformMixer1, 3);
AudioConnection          TsynthpatchCord81(oscFX2, 0, waveformMixer2, 3);
AudioConnection          TsynthpatchCord82(oscFX4, 0, waveformMixer4, 3);
AudioConnection          TsynthpatchCord83(oscFX3, 0, waveformMixer3, 3);
AudioConnection          TsynthpatchCord84(waveformMixer1, 0, filter1, 0);
AudioConnection          TsynthpatchCord85(waveformMixer2, 0, filter2, 0);
AudioConnection          TsynthpatchCord86(filterModMixer1, 0, filter1, 1);
AudioConnection          TsynthpatchCord87(waveformMixer3, 0, filter3, 0);
AudioConnection          TsynthpatchCord88(filterModMixer2, 0, filter2, 1);
AudioConnection          TsynthpatchCord89(filterModMixer3, 0, filter3, 1);
AudioConnection          TsynthpatchCord90(waveformMixer4, 0, filter4, 0);
AudioConnection          TsynthpatchCord91(filterModMixer4, 0, filter4, 1);
AudioConnection          TsynthpatchCord92(filter2, 0, filterMixer2, 0);
AudioConnection          TsynthpatchCord93(filter2, 1, filterMixer2, 1);
AudioConnection          TsynthpatchCord94(filter2, 2, filterMixer2, 2);
AudioConnection          TsynthpatchCord95(filter1, 0, filterMixer1, 0);
AudioConnection          TsynthpatchCord96(filter1, 1, filterMixer1, 1);
AudioConnection          TsynthpatchCord97(filter1, 2, filterMixer1, 2);
AudioConnection          TsynthpatchCord98(filter3, 0, filterMixer3, 0);
AudioConnection          TsynthpatchCord99(filter3, 1, filterMixer3, 1);
AudioConnection          TsynthpatchCord100(filter3, 2, filterMixer3, 2);
AudioConnection          TsynthpatchCord101(filter4, 0, filterMixer4, 0);
AudioConnection          TsynthpatchCord102(filter4, 1, filterMixer4, 1);
AudioConnection          TsynthpatchCord103(filter4, 2, filterMixer4, 2);
AudioConnection          TsynthpatchCord201(filter5, 0, filterMixer5, 0);
AudioConnection          TsynthpatchCord189(filter5, 1, filterMixer5, 1);
AudioConnection          TsynthpatchCord190(filter5, 2, filterMixer5, 2);
AudioConnection          TsynthpatchCord202(filter6, 0, filterMixer6, 0);
AudioConnection          TsynthpatchCord191(filter6, 1, filterMixer6, 1);
AudioConnection          TsynthpatchCord192(filter6, 2, filterMixer6, 2);
AudioConnection          TsynthpatchCord104(filterMixer2, ampEnvelope2);
AudioConnection          TsynthpatchCord105(filterMixer3, ampEnvelope3);
AudioConnection          TsynthpatchCord106(filterMixer1, ampEnvelope1);
AudioConnection          TsynthpatchCord107(filterMixer4, ampEnvelope4);
AudioConnection          TsynthpatchCord193(filterMixer5, ampEnvelope5);
AudioConnection          TsynthpatchCord194(filterMixer6, ampEnvelope6);
AudioConnection          TsynthpatchCord108(ampEnvelope2, 0, voiceMixer1, 1);
AudioConnection          TsynthpatchCord109(ampEnvelope3, 0, voiceMixer1, 2);
AudioConnection          TsynthpatchCord110(ampEnvelope4, 0, voiceMixer2, 0);
AudioConnection          TsynthpatchCord111(ampEnvelope1, 0, voiceMixer1, 0);
AudioConnection          TsynthpatchCord195(ampEnvelope5, 0, voiceMixer2, 1);
AudioConnection          TsynthpatchCord196(ampEnvelope6, 0, voiceMixer2, 2);
// AudioConnection       Tsynth   patchCord215(dcOffsetFilter, 2, scope, 0);
AudioConnection          TsynthpatchCord112(volumeMixer, 0, ensemble, 0);
AudioConnection          TsynthpatchCord113(ensemble, 0, effectMixerL, 1);
AudioConnection          TsynthpatchCord114(ensemble, 1, effectMixerR, 1);
AudioConnection          TsynthpatchCord115(volumeMixer, 0, effectMixerL, 0);
AudioConnection          TsynthpatchCord116(volumeMixer, 0, effectMixerR, 0);

// AudioConnection          patchCord117(effectMixerR, 0, usbAudio, 1);
// AudioConnection          patchCord118(effectMixerR, 0, i2s, 1);
// AudioConnection          patchCord119(effectMixerL, 0, i2s, 0);
// AudioConnection          patchCord120(effectMixerL, 0, usbAudio, 0);
AudioConnection          TsynthpatchCord118(effectMixerR, 0, mainMix2, 1);
// AudioConnection          TsynthpatchCord119(effectMixerL, 0, mainMix, 3);

AudioConnection          TsynthpatchCord121(oscModMixer1a, 0, waveformMod1a, 0);
AudioConnection          TsynthpatchCord122(oscModMixer1b, 0, waveformMod1b, 0);
AudioConnection          TsynthpatchCord123(oscModMixer2a, 0, waveformMod2a, 0);
AudioConnection          TsynthpatchCord124(oscModMixer2b, 0, waveformMod2b, 0);
AudioConnection          TsynthpatchCord125(oscModMixer3a, 0, waveformMod3a, 0);
AudioConnection          TsynthpatchCord126(oscModMixer3b, 0, waveformMod3b, 0);
AudioConnection          TsynthpatchCord127(oscModMixer4a, 0, waveformMod4a, 0);
AudioConnection          TsynthpatchCord128(oscModMixer4b, 0, waveformMod4b, 0);
AudioConnection          TsynthpatchCord129(filterEnvelope1, 0, oscModMixer1a, 1);
AudioConnection          TsynthpatchCord130(filterEnvelope1, 0, oscModMixer1b, 1);
AudioConnection          TsynthpatchCord131(filterEnvelope2, 0, oscModMixer2a, 1);
AudioConnection          TsynthpatchCord132(filterEnvelope2, 0, oscModMixer2b, 1);
AudioConnection          TsynthpatchCord133(filterEnvelope3, 0, oscModMixer3a, 1);
AudioConnection          TsynthpatchCord134(filterEnvelope3, 0, oscModMixer3b, 1);
AudioConnection          TsynthpatchCord135(filterEnvelope4, 0, oscModMixer4a, 1);
AudioConnection          TsynthpatchCord136(filterEnvelope4, 0, oscModMixer4b, 1);
AudioConnection          TsynthpatchCord22(glide1, 0, oscModMixer1a, 2);
AudioConnection          TsynthpatchCord204(glide1, 0, oscModMixer1b, 2);
AudioConnection          TsynthpatchCord205(glide2, 0, oscModMixer2a, 2);
AudioConnection          TsynthpatchCord206(glide2, 0, oscModMixer2b, 2);
AudioConnection          TsynthpatchCord207(glide3, 0, oscModMixer3a, 2);
AudioConnection          TsynthpatchCord208(glide3, 0, oscModMixer3b, 2);
AudioConnection          TsynthpatchCord209(glide4, 0, oscModMixer4a, 2);
AudioConnection          TsynthpatchCord210(glide4, 0, oscModMixer4b, 2);
AudioConnection          TsynthpatchCord211(glide5, 0, oscModMixer5a, 2);
AudioConnection          TsynthpatchCord212(glide5, 0, oscModMixer5b, 2);
AudioConnection          TsynthpatchCord213(glide6, 0, oscModMixer6a, 2);
AudioConnection          TsynthpatchCord214(glide6, 0, oscModMixer6b, 2);
AudioConnection          TsynthpatchCord137(pink, 0, noiseMixer, 0);
AudioConnection          TsynthpatchCord138(white, 0, noiseMixer, 1);
AudioConnection          TsynthpatchCord139(pwmLfo, 0, pwMixer5a, 0);
AudioConnection          TsynthpatchCord140(pwmLfo, 0, pwMixer5b, 0);
AudioConnection          TsynthpatchCord141(pwmLfo, 0, pwMixer6a, 0);
AudioConnection          TsynthpatchCord142(pwmLfo, 0, pwMixer6b, 0);
AudioConnection          TsynthpatchCord143(pwa, 0, pwMixer5a, 1);
AudioConnection          TsynthpatchCord144(pwb, 0, pwMixer5b, 1);
AudioConnection          TsynthpatchCord145(pwa, 0, pwMixer6a, 1);
AudioConnection          TsynthpatchCord146(pwb, 0, pwMixer6b, 1);
AudioConnection          TsynthpatchCord147(filterLfo, 0, filterModMixer5, 1);
AudioConnection          TsynthpatchCord148(filterLfo, 0, filterModMixer6, 1);
AudioConnection          TsynthpatchCord149(keytracking5, 0, filterModMixer5, 2);
AudioConnection          TsynthpatchCord150(keytracking6, 0, filterModMixer6, 2);
AudioConnection          TsynthpatchCord151(noiseMixer, 0, waveformMixer5, 2);
AudioConnection          TsynthpatchCord152(noiseMixer, 0, waveformMixer6, 2);
AudioConnection          TsynthpatchCord153(oscGlobalModMixer, 0, oscModMixer5a, 0);
AudioConnection          TsynthpatchCord154(oscGlobalModMixer, 0, oscModMixer5b, 0);
AudioConnection          TsynthpatchCord155(oscGlobalModMixer, 0, oscModMixer6a, 0);
AudioConnection          TsynthpatchCord156(oscGlobalModMixer, 0, oscModMixer6b, 0);
AudioConnection          TsynthpatchCord157(oscModMixer5a, 0, waveformMod5a, 0);
AudioConnection          TsynthpatchCord158(oscModMixer5b, 0, waveformMod5b, 0);
AudioConnection          TsynthpatchCord159(oscModMixer6a, 0, waveformMod6a, 0);
AudioConnection          TsynthpatchCord160(oscModMixer6b, 0, waveformMod6b, 0);
AudioConnection          TsynthpatchCord161(filterEnvelope5, 0, oscModMixer5a, 1);
AudioConnection          TsynthpatchCord162(filterEnvelope5, 0, oscModMixer5b, 1);
AudioConnection          TsynthpatchCord163(filterEnvelope6, 0, oscModMixer6a, 1);
AudioConnection          TsynthpatchCord164(filterEnvelope6, 0, oscModMixer6b, 1);
AudioConnection          TsynthpatchCord165(pwMixer5a, 0, waveformMod5a, 1);
AudioConnection          TsynthpatchCord166(pwMixer5b, 0, waveformMod5b, 1);
AudioConnection          TsynthpatchCord167(pwMixer6a, 0, waveformMod6a, 1);
AudioConnection          TsynthpatchCord168(pwMixer6b, 0, waveformMod6b, 1);
AudioConnection          TsynthpatchCord169(waveformMod5a, 0, waveformMixer5, 0);
AudioConnection          TsynthpatchCord170(waveformMod5a, 0, oscFX5, 0);
AudioConnection          TsynthpatchCord171(waveformMod5b, 0, waveformMixer5, 1);
AudioConnection          TsynthpatchCord172(waveformMod5b, 0, oscFX5, 1);
AudioConnection          TsynthpatchCord173(waveformMod6a, 0, waveformMixer6, 0);
AudioConnection          TsynthpatchCord174(waveformMod6a, 0, oscFX6, 0);
AudioConnection          TsynthpatchCord175(waveformMod6b, 0, waveformMixer6, 1);
AudioConnection          TsynthpatchCord176(waveformMod6b, 0, oscFX6, 1);
AudioConnection          TsynthpatchCord177(filterEnvelope5, 0, filterModMixer5, 0);
AudioConnection          TsynthpatchCord178(filterEnvelope5, 0, pwMixer5a, 2);
AudioConnection          TsynthpatchCord179(filterEnvelope5, 0, pwMixer5b, 2);
AudioConnection          TsynthpatchCord180(filterEnvelope6, 0, filterModMixer6, 0);
AudioConnection          TsynthpatchCord181(filterEnvelope6, 0, pwMixer6a, 2);
AudioConnection          TsynthpatchCord182(filterEnvelope6, 0, pwMixer6b, 2);
AudioConnection          TsynthpatchCord183(oscFX5, 0, waveformMixer5, 3);
AudioConnection          TsynthpatchCord184(oscFX6, 0, waveformMixer6, 3);
AudioConnection          TsynthpatchCord185(waveformMixer5, 0, filter5, 0);
AudioConnection          TsynthpatchCord186(filterModMixer5, 0, filter5, 1);
AudioConnection          TsynthpatchCord187(waveformMixer6, 0, filter6, 0);
AudioConnection          TsynthpatchCord188(filterModMixer6, 0, filter6, 1);
AudioConnection          TsynthpatchCord197(voiceMixer1, 0, voiceMixerM, 0);
AudioConnection          TsynthpatchCord198(voiceMixer2, 0, voiceMixerM, 1);
AudioConnection          TsynthpatchCord199(constant1Dc, filterEnvelope5);
AudioConnection          TsynthpatchCord200(constant1Dc, filterEnvelope6);
AudioConnection          TsynthpatchCord203(voiceMixerM, 0, dcOffsetFilter, 0);
AudioConnection          TsynthpatchCord217(dcOffsetFilter, 2, volumeMixer, 0);
AudioConnection          TsynthpatchCord216(dcOffsetFilter, 2, peak, 0);
// GUItool: end automatically generated code

void Tsynth_AOstop(){
  constant1Dc.stop();
  pink.stop();
  white.stop();
  noiseMixer.stop();
  pwmLfo.stop();
  pwa.stop();
  pwb.stop();
  filterEnvelope6.stop();
  filterEnvelope5.stop();
  filterEnvelope4.stop();
  filterEnvelope1.stop();
  filterEnvelope3.stop();
  filterEnvelope2.stop();
  pwMixer1a.stop();
  pwMixer1b.stop();
  pwMixer4b.stop();
  pwMixer4a.stop();
  pwMixer2a.stop();
  pwMixer2b.stop();
  pwMixer3a.stop();
  pwMixer3b.stop();
  pwMixer5a.stop();
  pwMixer5b.stop();
  pwMixer6a.stop();
  pwMixer6b.stop();
  pitchLfo.stop();
  glide1.stop();
  glide2.stop();
  glide3.stop();
  glide4.stop();
  glide5.stop();
  glide6.stop();
  pitchBend.stop();
  filterLfo.stop();
  keytracking1.stop();
  keytracking2.stop();
  keytracking3.stop();
  keytracking4.stop();
  keytracking5.stop();
  keytracking6.stop();
  oscGlobalModMixer.stop();
  oscModMixer1a.stop();
  oscModMixer1b.stop();
  oscModMixer2a.stop();
  oscModMixer2b.stop();
  oscModMixer3a.stop();
  oscModMixer3b.stop();
  oscModMixer4a.stop();
  oscModMixer4b.stop();
  oscModMixer5a.stop();
  oscModMixer5b.stop();
  oscModMixer6a.stop();
  oscModMixer6b.stop();
  waveformMod1b.stop();
  waveformMod1a.stop();
  waveformMod2b.stop();
  waveformMod4a.stop();
  waveformMod2a.stop();
  waveformMod4b.stop();
  waveformMod3a.stop();
  waveformMod3b.stop();
  waveformMod5a.stop();
  waveformMod5b.stop();
  waveformMod6a.stop();
  waveformMod6b.stop();
  oscFX1.stop();
  oscFX2.stop();
  oscFX4.stop();
  oscFX3.stop();
  oscFX5.stop();
  oscFX6.stop();
  waveformMixer1.stop();
  waveformMixer2.stop();
  waveformMixer3.stop();
  waveformMixer4.stop();
  waveformMixer5.stop();
  waveformMixer6.stop();
  filterModMixer1.stop();
  filterModMixer2.stop();
  filterModMixer3.stop();
  filterModMixer4.stop();
  filterModMixer5.stop();
  filterModMixer6.stop();
  filter2.stop();
  filter1.stop();
  filter3.stop();
  filter4.stop();
  filter5.stop();
  filter6.stop();
  filterMixer2.stop();
  filterMixer3.stop();
  filterMixer1.stop();
  filterMixer4.stop();
  filterMixer5.stop();
  filterMixer6.stop();
  ampEnvelope2.stop();
  ampEnvelope3.stop();
  ampEnvelope4.stop();
  ampEnvelope1.stop();
  ampEnvelope5.stop();
  ampEnvelope6.stop();
  voiceMixer1.stop();
  voiceMixer2.stop();
  voiceMixerM.stop();
  dcOffsetFilter.stop();
  ensemble.stop();
  effectMixerR.stop();
  effectMixerL.stop();

  TsynthpatchCord1.disconnect();
  TsynthpatchCord2.disconnect();
  TsynthpatchCord3.disconnect();
  TsynthpatchCord4.disconnect();
  TsynthpatchCord6.disconnect();
  TsynthpatchCord7.disconnect();
  TsynthpatchCord8.disconnect();
  TsynthpatchCord9.disconnect();
  TsynthpatchCord10.disconnect();
  TsynthpatchCord11.disconnect();
  TsynthpatchCord12.disconnect();
  TsynthpatchCord13.disconnect();
  TsynthpatchCord14.disconnect();
  TsynthpatchCord15.disconnect();
  TsynthpatchCord16.disconnect();
  TsynthpatchCord17.disconnect();
  TsynthpatchCord18.disconnect();
  TsynthpatchCord19.disconnect();
  TsynthpatchCord20.disconnect();
  TsynthpatchCord21.disconnect();
  TsynthpatchCord23.disconnect();
  TsynthpatchCord24.disconnect();
  TsynthpatchCord25.disconnect();
  TsynthpatchCord26.disconnect();
  TsynthpatchCord27.disconnect();
  TsynthpatchCord28.disconnect();
  TsynthpatchCord29.disconnect();
  TsynthpatchCord30.disconnect();
  TsynthpatchCord31.disconnect();
  TsynthpatchCord32.disconnect();
  TsynthpatchCord33.disconnect();
  TsynthpatchCord34.disconnect();
  TsynthpatchCord35.disconnect();
  TsynthpatchCord36.disconnect();
  TsynthpatchCord37.disconnect();
  TsynthpatchCord38.disconnect();
  TsynthpatchCord39.disconnect();
  TsynthpatchCord40.disconnect();
  TsynthpatchCord41.disconnect();
  TsynthpatchCord42.disconnect();
  TsynthpatchCord43.disconnect();
  TsynthpatchCord44.disconnect();
  TsynthpatchCord45.disconnect();
  TsynthpatchCord46.disconnect();
  TsynthpatchCord47.disconnect();
  TsynthpatchCord48.disconnect();
  TsynthpatchCord49.disconnect();
  TsynthpatchCord50.disconnect();
  TsynthpatchCord51.disconnect();
  TsynthpatchCord52.disconnect();
  TsynthpatchCord53.disconnect();
  TsynthpatchCord54.disconnect();
  TsynthpatchCord55.disconnect();
  TsynthpatchCord56.disconnect();
  TsynthpatchCord57.disconnect();
  TsynthpatchCord58.disconnect();
  TsynthpatchCord59.disconnect();
  TsynthpatchCord60.disconnect();
  TsynthpatchCord61.disconnect();
  TsynthpatchCord62.disconnect();
  TsynthpatchCord63.disconnect();
  TsynthpatchCord64.disconnect();
  TsynthpatchCord65.disconnect();
  TsynthpatchCord66.disconnect();
  TsynthpatchCord67.disconnect();
  TsynthpatchCord68.disconnect();
  TsynthpatchCord69.disconnect();
  TsynthpatchCord70.disconnect();
  TsynthpatchCord71.disconnect();
  TsynthpatchCord72.disconnect();
  TsynthpatchCord73.disconnect();
  TsynthpatchCord74.disconnect();
  TsynthpatchCord75.disconnect();
  TsynthpatchCord76.disconnect();
  TsynthpatchCord77.disconnect();
  TsynthpatchCord78.disconnect();
  TsynthpatchCord79.disconnect();
  TsynthpatchCord80.disconnect();
  TsynthpatchCord81.disconnect();
  TsynthpatchCord82.disconnect();
  TsynthpatchCord83.disconnect();
  TsynthpatchCord84.disconnect();
  TsynthpatchCord85.disconnect();
  TsynthpatchCord86.disconnect();
  TsynthpatchCord87.disconnect();
  TsynthpatchCord88.disconnect();
  TsynthpatchCord89.disconnect();
  TsynthpatchCord90.disconnect();
  TsynthpatchCord91.disconnect();
  TsynthpatchCord92.disconnect();
  TsynthpatchCord93.disconnect();
  TsynthpatchCord94.disconnect();
  TsynthpatchCord95.disconnect();
  TsynthpatchCord96.disconnect();
  TsynthpatchCord97.disconnect();
  TsynthpatchCord98.disconnect();
  TsynthpatchCord99.disconnect();
  TsynthpatchCord100.disconnect();
  TsynthpatchCord101.disconnect();
  TsynthpatchCord102.disconnect();
  TsynthpatchCord103.disconnect();
  TsynthpatchCord201.disconnect();
  TsynthpatchCord189.disconnect();
  TsynthpatchCord190.disconnect();
  TsynthpatchCord202.disconnect();
  TsynthpatchCord191.disconnect();
  TsynthpatchCord192.disconnect();
  TsynthpatchCord104.disconnect();
  TsynthpatchCord105.disconnect();
  TsynthpatchCord106.disconnect();
  TsynthpatchCord107.disconnect();
  TsynthpatchCord193.disconnect();
  TsynthpatchCord194.disconnect();
  TsynthpatchCord108.disconnect();
  TsynthpatchCord109.disconnect();
  TsynthpatchCord110.disconnect();
  TsynthpatchCord111.disconnect();
  TsynthpatchCord195.disconnect();
  TsynthpatchCord196.disconnect();
  TsynthpatchCord112.disconnect();
  TsynthpatchCord113.disconnect();
  TsynthpatchCord114.disconnect();
  TsynthpatchCord115.disconnect();
  TsynthpatchCord116.disconnect();
  TsynthpatchCord118.disconnect();
  // TsynthpatchCord119.disconnect();
  TsynthpatchCord121.disconnect();
  TsynthpatchCord122.disconnect();
  TsynthpatchCord123.disconnect();
  TsynthpatchCord124.disconnect();
  TsynthpatchCord125.disconnect();
  TsynthpatchCord126.disconnect();
  TsynthpatchCord127.disconnect();
  TsynthpatchCord128.disconnect();
  TsynthpatchCord129.disconnect();
  TsynthpatchCord130.disconnect();
  TsynthpatchCord131.disconnect();
  TsynthpatchCord132.disconnect();
  TsynthpatchCord133.disconnect();
  TsynthpatchCord134.disconnect();
  TsynthpatchCord135.disconnect();
  TsynthpatchCord136.disconnect();
  TsynthpatchCord22.disconnect();
  TsynthpatchCord204.disconnect();
  TsynthpatchCord205.disconnect();
  TsynthpatchCord206.disconnect();
  TsynthpatchCord207.disconnect();
  TsynthpatchCord208.disconnect();
  TsynthpatchCord209.disconnect();
  TsynthpatchCord210.disconnect();
  TsynthpatchCord211.disconnect();
  TsynthpatchCord212.disconnect();
  TsynthpatchCord213.disconnect();
  TsynthpatchCord214.disconnect();
  TsynthpatchCord137.disconnect();
  TsynthpatchCord138.disconnect();
  TsynthpatchCord139.disconnect();
  TsynthpatchCord140.disconnect();
  TsynthpatchCord141.disconnect();
  TsynthpatchCord142.disconnect();
  TsynthpatchCord143.disconnect();
  TsynthpatchCord144.disconnect();
  TsynthpatchCord145.disconnect();
  TsynthpatchCord146.disconnect();
  TsynthpatchCord147.disconnect();
  TsynthpatchCord148.disconnect();
  TsynthpatchCord149.disconnect();
  TsynthpatchCord150.disconnect();
  TsynthpatchCord151.disconnect();
  TsynthpatchCord152.disconnect();
  TsynthpatchCord153.disconnect();
  TsynthpatchCord154.disconnect();
  TsynthpatchCord155.disconnect();
  TsynthpatchCord156.disconnect();
  TsynthpatchCord157.disconnect();
  TsynthpatchCord158.disconnect();
  TsynthpatchCord159.disconnect();
  TsynthpatchCord160.disconnect();
  TsynthpatchCord161.disconnect();
  TsynthpatchCord162.disconnect();
  TsynthpatchCord163.disconnect();
  TsynthpatchCord164.disconnect();
  TsynthpatchCord165.disconnect();
  TsynthpatchCord166.disconnect();
  TsynthpatchCord167.disconnect();
  TsynthpatchCord168.disconnect();
  TsynthpatchCord169.disconnect();
  TsynthpatchCord170.disconnect();
  TsynthpatchCord171.disconnect();
  TsynthpatchCord172.disconnect();
  TsynthpatchCord173.disconnect();
  TsynthpatchCord174.disconnect();
  TsynthpatchCord175.disconnect();
  TsynthpatchCord176.disconnect();
  TsynthpatchCord177.disconnect();
  TsynthpatchCord178.disconnect();
  TsynthpatchCord179.disconnect();
  TsynthpatchCord180.disconnect();
  TsynthpatchCord181.disconnect();
  TsynthpatchCord182.disconnect();
  TsynthpatchCord183.disconnect();
  TsynthpatchCord184.disconnect();
  TsynthpatchCord185.disconnect();
  TsynthpatchCord186.disconnect();
  TsynthpatchCord187.disconnect();
  TsynthpatchCord188.disconnect();
  TsynthpatchCord197.disconnect();
  TsynthpatchCord198.disconnect();
  TsynthpatchCord199.disconnect();
  TsynthpatchCord217.disconnect();
  TsynthpatchCord216.disconnect();
}

void Tsynth_AOstart(){
  TsynthpatchCord1.connect();
  TsynthpatchCord2.connect();
  TsynthpatchCord3.connect();
  TsynthpatchCord4.connect();
  TsynthpatchCord6.connect();
  TsynthpatchCord7.connect();
  TsynthpatchCord8.connect();
  TsynthpatchCord9.connect();
  TsynthpatchCord10.connect();
  TsynthpatchCord11.connect();
  TsynthpatchCord12.connect();
  TsynthpatchCord13.connect();
  TsynthpatchCord14.connect();
  TsynthpatchCord15.connect();
  TsynthpatchCord16.connect();
  TsynthpatchCord17.connect();
  TsynthpatchCord18.connect();
  TsynthpatchCord19.connect();
  TsynthpatchCord20.connect();
  TsynthpatchCord21.connect();
  TsynthpatchCord23.connect();
  TsynthpatchCord24.connect();
  TsynthpatchCord25.connect();
  TsynthpatchCord26.connect();
  TsynthpatchCord27.connect();
  TsynthpatchCord28.connect();
  TsynthpatchCord29.connect();
  TsynthpatchCord30.connect();
  TsynthpatchCord31.connect();
  TsynthpatchCord32.connect();
  TsynthpatchCord33.connect();
  TsynthpatchCord34.connect();
  TsynthpatchCord35.connect();
  TsynthpatchCord36.connect();
  TsynthpatchCord37.connect();
  TsynthpatchCord38.connect();
  TsynthpatchCord39.connect();
  TsynthpatchCord40.connect();
  TsynthpatchCord41.connect();
  TsynthpatchCord42.connect();
  TsynthpatchCord43.connect();
  TsynthpatchCord44.connect();
  TsynthpatchCord45.connect();
  TsynthpatchCord46.connect();
  TsynthpatchCord47.connect();
  TsynthpatchCord48.connect();
  TsynthpatchCord49.connect();
  TsynthpatchCord50.connect();
  TsynthpatchCord51.connect();
  TsynthpatchCord52.connect();
  TsynthpatchCord53.connect();
  TsynthpatchCord54.connect();
  TsynthpatchCord55.connect();
  TsynthpatchCord56.connect();
  TsynthpatchCord57.connect();
  TsynthpatchCord58.connect();
  TsynthpatchCord59.connect();
  TsynthpatchCord60.connect();
  TsynthpatchCord61.connect();
  TsynthpatchCord62.connect();
  TsynthpatchCord63.connect();
  TsynthpatchCord64.connect();
  TsynthpatchCord65.connect();
  TsynthpatchCord66.connect();
  TsynthpatchCord67.connect();
  TsynthpatchCord68.connect();
  TsynthpatchCord69.connect();
  TsynthpatchCord70.connect();
  TsynthpatchCord71.connect();
  TsynthpatchCord72.connect();
  TsynthpatchCord73.connect();
  TsynthpatchCord74.connect();
  TsynthpatchCord75.connect();
  TsynthpatchCord76.connect();
  TsynthpatchCord77.connect();
  TsynthpatchCord78.connect();
  TsynthpatchCord79.connect();
  TsynthpatchCord80.connect();
  TsynthpatchCord81.connect();
  TsynthpatchCord82.connect();
  TsynthpatchCord83.connect();
  TsynthpatchCord84.connect();
  TsynthpatchCord85.connect();
  TsynthpatchCord86.connect();
  TsynthpatchCord87.connect();
  TsynthpatchCord88.connect();
  TsynthpatchCord89.connect();
  TsynthpatchCord90.connect();
  TsynthpatchCord91.connect();
  TsynthpatchCord92.connect();
  TsynthpatchCord93.connect();
  TsynthpatchCord94.connect();
  TsynthpatchCord95.connect();
  TsynthpatchCord96.connect();
  TsynthpatchCord97.connect();
  TsynthpatchCord98.connect();
  TsynthpatchCord99.connect();
  TsynthpatchCord100.connect();
  TsynthpatchCord101.connect();
  TsynthpatchCord102.connect();
  TsynthpatchCord103.connect();
  TsynthpatchCord201.connect();
  TsynthpatchCord189.connect();
  TsynthpatchCord190.connect();
  TsynthpatchCord202.connect();
  TsynthpatchCord191.connect();
  TsynthpatchCord192.connect();
  TsynthpatchCord104.connect();
  TsynthpatchCord105.connect();
  TsynthpatchCord106.connect();
  TsynthpatchCord107.connect();
  TsynthpatchCord193.connect();
  TsynthpatchCord194.connect();
  TsynthpatchCord108.connect();
  TsynthpatchCord109.connect();
  TsynthpatchCord110.connect();
  TsynthpatchCord111.connect();
  TsynthpatchCord195.connect();
  TsynthpatchCord196.connect();
  TsynthpatchCord112.connect();
  TsynthpatchCord113.connect();
  TsynthpatchCord114.connect();
  TsynthpatchCord115.connect();
  TsynthpatchCord116.connect();
  TsynthpatchCord118.connect();
  // TsynthpatchCord119.connect();
  TsynthpatchCord121.connect();
  TsynthpatchCord122.connect();
  TsynthpatchCord123.connect();
  TsynthpatchCord124.connect();
  TsynthpatchCord125.connect();
  TsynthpatchCord126.connect();
  TsynthpatchCord127.connect();
  TsynthpatchCord128.connect();
  TsynthpatchCord129.connect();
  TsynthpatchCord130.connect();
  TsynthpatchCord131.connect();
  TsynthpatchCord132.connect();
  TsynthpatchCord133.connect();
  TsynthpatchCord134.connect();
  TsynthpatchCord135.connect();
  TsynthpatchCord136.connect();
  TsynthpatchCord22.connect();
  TsynthpatchCord204.connect();
  TsynthpatchCord205.connect();
  TsynthpatchCord206.connect();
  TsynthpatchCord207.connect();
  TsynthpatchCord208.connect();
  TsynthpatchCord209.connect();
  TsynthpatchCord210.connect();
  TsynthpatchCord211.connect();
  TsynthpatchCord212.connect();
  TsynthpatchCord213.connect();
  TsynthpatchCord214.connect();
  TsynthpatchCord137.connect();
  TsynthpatchCord138.connect();
  TsynthpatchCord139.connect();
  TsynthpatchCord140.connect();
  TsynthpatchCord141.connect();
  TsynthpatchCord142.connect();
  TsynthpatchCord143.connect();
  TsynthpatchCord144.connect();
  TsynthpatchCord145.connect();
  TsynthpatchCord146.connect();
  TsynthpatchCord147.connect();
  TsynthpatchCord148.connect();
  TsynthpatchCord149.connect();
  TsynthpatchCord150.connect();
  TsynthpatchCord151.connect();
  TsynthpatchCord152.connect();
  TsynthpatchCord153.connect();
  TsynthpatchCord154.connect();
  TsynthpatchCord155.connect();
  TsynthpatchCord156.connect();
  TsynthpatchCord157.connect();
  TsynthpatchCord158.connect();
  TsynthpatchCord159.connect();
  TsynthpatchCord160.connect();
  TsynthpatchCord161.connect();
  TsynthpatchCord162.connect();
  TsynthpatchCord163.connect();
  TsynthpatchCord164.connect();
  TsynthpatchCord165.connect();
  TsynthpatchCord166.connect();
  TsynthpatchCord167.connect();
  TsynthpatchCord168.connect();
  TsynthpatchCord169.connect();
  TsynthpatchCord170.connect();
  TsynthpatchCord171.connect();
  TsynthpatchCord172.connect();
  TsynthpatchCord173.connect();
  TsynthpatchCord174.connect();
  TsynthpatchCord175.connect();
  TsynthpatchCord176.connect();
  TsynthpatchCord177.connect();
  TsynthpatchCord178.connect();
  TsynthpatchCord179.connect();
  TsynthpatchCord180.connect();
  TsynthpatchCord181.connect();
  TsynthpatchCord182.connect();
  TsynthpatchCord183.connect();
  TsynthpatchCord184.connect();
  TsynthpatchCord185.connect();
  TsynthpatchCord186.connect();
  TsynthpatchCord187.connect();
  TsynthpatchCord188.connect();
  TsynthpatchCord197.connect();
  TsynthpatchCord198.connect();
  TsynthpatchCord199.connect();
  TsynthpatchCord217.connect();
  TsynthpatchCord216.connect();

  constant1Dc.start();
  pink.start();
  white.start();
  noiseMixer.start();
  pwmLfo.start();
  pwa.start();
  pwb.start();
  filterEnvelope6.start();
  filterEnvelope5.start();
  filterEnvelope4.start();
  filterEnvelope1.start();
  filterEnvelope3.start();
  filterEnvelope2.start();
  pwMixer1a.start();
  pwMixer1b.start();
  pwMixer4b.start();
  pwMixer4a.start();
  pwMixer2a.start();
  pwMixer2b.start();
  pwMixer3a.start();
  pwMixer3b.start();
  pwMixer5a.start();
  pwMixer5b.start();
  pwMixer6a.start();
  pwMixer6b.start();
  pitchLfo.start();
  glide1.start();
  glide2.start();
  glide3.start();
  glide4.start();
  glide5.start();
  glide6.start();
  pitchBend.start();
  filterLfo.start();
  keytracking1.start();
  keytracking2.start();
  keytracking3.start();
  keytracking4.start();
  keytracking5.start();
  keytracking6.start();
  oscGlobalModMixer.start();
  oscModMixer1a.start();
  oscModMixer1b.start();
  oscModMixer2a.start();
  oscModMixer2b.start();
  oscModMixer3a.start();
  oscModMixer3b.start();
  oscModMixer4a.start();
  oscModMixer4b.start();
  oscModMixer5a.start();
  oscModMixer5b.start();
  oscModMixer6a.start();
  oscModMixer6b.start();
  waveformMod1b.start();
  waveformMod1a.start();
  waveformMod2b.start();
  waveformMod4a.start();
  waveformMod2a.start();
  waveformMod4b.start();
  waveformMod3a.start();
  waveformMod3b.start();
  waveformMod5a.start();
  waveformMod5b.start();
  waveformMod6a.start();
  waveformMod6b.start();
  oscFX1.start();
  oscFX2.start();
  oscFX4.start();
  oscFX3.start();
  oscFX5.start();
  oscFX6.start();
  waveformMixer1.start();
  waveformMixer2.start();
  waveformMixer3.start();
  waveformMixer4.start();
  waveformMixer5.start();
  waveformMixer6.start();
  filterModMixer1.start();
  filterModMixer2.start();
  filterModMixer3.start();
  filterModMixer4.start();
  filterModMixer5.start();
  filterModMixer6.start();
  filter2.start();
  filter1.start();
  filter3.start();
  filter4.start();
  filter5.start();
  filter6.start();
  filterMixer2.start();
  filterMixer3.start();
  filterMixer1.start();
  filterMixer4.start();
  filterMixer5.start();
  filterMixer6.start();
  ampEnvelope2.start();
  ampEnvelope3.start();
  ampEnvelope4.start();
  ampEnvelope1.start();
  ampEnvelope5.start();
  ampEnvelope6.start();
  voiceMixer1.start();
  voiceMixer2.start();
  voiceMixerM.start();
  dcOffsetFilter.start();
  ensemble.start();
  effectMixerR.start();
  effectMixerL.start();
}

#endif
