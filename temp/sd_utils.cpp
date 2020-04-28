#include "Arduino.h"
#include "sd_utils.h"

// void init_sd_samples(){
//   if (!SD.begin(chipSelect)) {
//     //Serial.println("initialization failed!");
//     return;
//   }
//   initSAMPLE_X();
//
//   root = SD.open("/");
//
//   listSDfiles(root, 0);
//
//   fillSAMPLE_X();
//
// }
//
// //***************List SD files****************
// void listSDfiles(File dir, int numTabs){
//   int i = 0;
//   while(true) {
//      File entry =  dir.openNextFile();
//      if (! entry) {
//        // no more files
//        //Serial.println("**nomorefiles**");
//        break;
//      }
//      for (uint8_t i=0; i<numTabs; i++) {
//        Serial.print('\t');
//      }
//      Serial.println(entry.name());
//      if (entry.isDirectory()) {
//        Serial.println("/");
//        //printDirectory(entry, numTabs+1);
//      } else {
//        // files have sizes, directories do not
//        strcpy(SAMPLE_X[0][i], entry.name());
//      }
//      entry.close();
//      i = i + 1;
//      delay(5);
//    }
// }
//
// void initSAMPLE_X(){
//   for(int i = 0; i<NBTYPE; i++){
//     for(int j = 0; j<NBX; j++){
//       SAMPLE_X[i][j][0] = 0;
//     }
//   }
// }
//
// void fillSAMPLE_X(){
//   for(int i = 0; i<NBX; i++){
//     if(SAMPLE_X[0][i][0] == 0){
//       break;
//     }
//     for(int j = 0; j<NBTYPE; j++){
//       if(SAMPLE_X[0][i][0] == SAMPLE_TYPE[j][0]){
//         for(int k = 0; k<NBX; k++){
//           if(SAMPLE_X[j][k][0] == 0){
//             strcat(SAMPLE_X[j][k],SAMPLE_X[0][i]);
//             break;
//           }
//         }
//       }
//     }
//   }
// }
