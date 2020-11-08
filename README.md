# NervousSuperSynth
Synthesizer and sampler built around Teensy 3.5/3.6.

It is mainly a development platform using almost all the ports on Teensy.

The audio library is based on Teensy's one with the ability to stop the update process
of audio objects in order to save CPU load.

# ModularGrid : https://www.modulargrid.net/e/other-unknown-nervoussupersynth

![Image of NervousSuperSynth bare pcb](https://github.com/nervousapps/NervousSuperSynth/raw/proto_1/images/bare_pcb.png)

![Image of NervousSuperSynth monster vs proto_1 1](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_1.png)

![Image of NervousSuperSynth monster vs proto_1 2](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_2.png)

![Image of NervousSuperSynth monster vs proto_1 3](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_3.png)

![Image of NervousSuperSynth monster vs proto_1 4](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_4.png)

# I/O
- 6 trigger/gate input
- MIDI IN
- DAC0 output
- DAC1 output with analog volume control
- PT8211 DAC stereo output
- LED screen 20x2 with contrast control

# Analog controls
- 10 sliders
- 5 potentiometers
- 3 switches

# Digital controls
- 2 encoders with pushbutton

# Hardware controls
- RESET
- PROGRAM

# Firmware
The firmware is an aggregation of several synths ported for this motherboard:
- Kelpie (https://github.com/friedpies/kelpie-pocket-synth)
- Braids (https://github.com/pichenettes/eurorack and https://github.com/modlfo/teensy-braids)
- ChordOrgan (https://github.com/TomWhitwell/Chord-Organ)
- Tsynth (https://github.com/ElectroTechnique/TSynth/)
- SamplePlayer

# Compilation
PlatformIO

# SCHEMATICS & PCB
https://easyeda.com/xtreemechille/supersynth-copy

SMD and TH parts, one PCB.

I have some PCBs in stock, assembled, kit or bare, contact me at nervousrvoltage@icloud.com if you are interested.

# BOM
https://docs.google.com/spreadsheets/d/1-4hl6AcAoJYJrRvISyrwIaEGTYHwAB_IcKs__rtEGYY/edit?usp=sharing

# PANEL
https://cad.onshape.com/documents/76309e780cb99c0f2066b519/w/a28c1d39745cdb4e42b1148c/e/933dfecdfdc7ae9463372842

I made the panel here : https://www.john-steel.com/

# TODO
- add editor mode to SamplePlayer
- add some synths:
    - https://github.com/otem/teensypolysynth/
    - https://github.com/mattybrad/polymod
    - https://github.com/marshalltaylorSFE/Proto-8
    - https://github.com/ghostintranslation/SimpleSynth
    - https://github.com/kbob/drum-machine
    - https://github.com/RyoKosaka/HelloDrum-arduino-Library (https://gist.github.com/RyoKosaka/2046d4af9e4f12aaba618d9c9036d89e)
    - https://forum.pjrc.com/threads/32167-My-Teensy-3-1-Commodore-SID-Emulation-Player
- review kelpie parameters for switches
- clean code
- update to ChordOrgan v0.6 (https://github.com/TomWhitwell/Chord-Organ/tree/v0.6.0/Chord-Organ)
