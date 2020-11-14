/*
 __    _  _____   ___      ___ _  _     __    _ ______ _
/__|_|/ \(_  |     | |\|    | |_)|_||\|(_ |  |_| |  | / \|\|
\_|| |\_/__) |    _|_| |    | | \| || |__)|__| | | _|_\_/| |

SYNTH
v1.0.0

If you enjoy my work and music please consider donating.

https://www.ghostintranslation.com/
https://ghostintranslation.bandcamp.com/
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/channel/UCcyUTGTM-hGLIz4194Inxyw
https://github.com/ghostintranslation
*/

#include "Arduino.h"
#include <AudioPrivate.h>
#include "Synth.h"

// Instanciation of DS9
Synth * synth = Synth::getInstance();;

AudioConnection FmSynthpatchCord1(*synth->getOutput(), 0, mainMix3, 1);

void setupFMSynth() {
  // synth = Synth::getInstance();
  synth->init(device);
}

void runFMSynth() {
  // Synth update
  synth->update();
}

void stopFMSynth() {
  // Synth stop
  // if(synth != nullptr){
    synth->stop();
  //   delete synth;
  //   synth = nullptr;
  // }
}
