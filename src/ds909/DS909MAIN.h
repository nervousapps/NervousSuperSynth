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

#include <Arduino.h>
#include <AudioPrivate.h>

#include "DS909.h"

// Instanciation of DS9
DS909 * ds909 = DS909::getInstance();

// Connecting DS9 to general audio output
AudioConnection ds909patchCord1(*ds909->getOutput(), 0, mainMix2, 2);

void setupDS909() {
  ds909->init(device);

  // while (!Serial && millis() < 2500); // wait for serial monitor
  //
  // // Starting sequence
  // Serial.println("Ready!");
}

void runDS909() {
  ds909->update();
}

void stopDS909(){
  ds909->stop();
}
