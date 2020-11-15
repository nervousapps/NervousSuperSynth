/*
 __    _  _____   ___      ___ _  _     __    _ ______ _
/__|_|/ \(_  |     | |\|    | |_)|_||\|(_ |  |_| |  | / \|\|
\_|| |\_/__) |    _|_| |    | | \| || |__)|__| | | _|_\_/| |

DS909
v0.5

https://ghostintranslation.bandcamp.com
https://www.instagram.com/ghostintranslation
https://github.com/ghostintranslation
https://www.ghostintranslation.com
*/

#include <AudioPrivate.h>
#include "DS909.h"

// Instanciation of DS9
DS909 * ds909 = DS909::getInstance();

// Connecting DS9 to general audio output
AudioAmplifier  AMP;
AudioConnection patchCord1(*ds909->getOutput(), 0, AMP, 0);
AudioConnection patchCord2(AMP, 0, mainMix1, 1);

void setAMPGAIN(byte inputIndex, unsigned int value, int diffToPrevious) {
  AMP.gain(value);
}

void setupDS909() {
  ds909->init(device);
  patchCord1.connect();
  patchCord2.connect();
  AMP.gain(10);
  device->setHandlePotentiometerChange(9, setAMPGAIN);
}

void runDS909() {
  ds909->update();
}

void stopDS909(){
  patchCord1.disconnect();
  patchCord2.disconnect();
}
