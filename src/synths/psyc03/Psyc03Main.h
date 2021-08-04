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

AudioConnection Psyc03patchCord1(*psyc03->getOutput(), 0, mainMix2, 0);
boolean psyc03Running = false;

void setupPsyc03() {
  psyc03->init(device);
  Psyc03patchCord1.connect();
}

void runPsyc03() {
  psyc03->update();
}

void stopPsyc03() {
  psyc03->stop();
  Psyc03patchCord1.disconnect();
}
