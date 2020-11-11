/*
 __    _  _____   ___      ___ _  _     __    _ ______ _
/__|_|/ \(_  |     | |\|    | |_)|_||\|(_ |  |_| |  | / \|\|
\_|| |\_/__) |    _|_| |    | | \| || |__)|__| | | _|_\_/| |
If you enjoy my work and music please consider donating.
https://www.ghostintranslation.com/
https://ghostintranslation.bandcamp.com/
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/channel/UCcyUTGTM-hGLIz4194Inxyw
https://github.com/ghostintranslation
*/
#include "Arduino.h"
#include "Psyc03.h"

// Instanciation of Psyc03
Psyc03 * psyc03 = Psyc03::getInstance();

AudioConnection Psyc03patchCord1(*psyc03->getOutput(), 0, mainMix2, 1);
boolean psyc03Running = false;

void setupPsyc03() {
  // psyc03 = Psyc03::getInstance();
  psyc03->init(device);
  // Psyc03patchCord1 = new AudioConnection(*psyc03->envelope1, 0, mainMix2, 1);
  psyc03Running = true;
}

void runPsyc03() {
  psyc03->update();
}

void stopPsyc03() {
  if(psyc03Running){
    psyc03->stop();
    // delete Psyc03patchCord1;
    psyc03Running = false;
  }
}
