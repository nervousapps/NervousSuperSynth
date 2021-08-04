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

AudioConnection FmSynthpatchCord1(*synth->getOutput(), 0, mainMix1, 2);

void setupFMSynth() {
  synth->init(device);
  FmSynthpatchCord1.connect();
}

void runFMSynth() {
  // Synth update
  synth->update();
}

void stopFMSynth() {
  synth->stop();
  FmSynthpatchCord1.disconnect();
}
