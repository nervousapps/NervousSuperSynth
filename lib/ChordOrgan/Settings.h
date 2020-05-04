#ifndef Settings_h
#define Settings_h

#include "Arduino.h"
#include <SD.h>

extern boolean settingsextraWaves;
extern boolean settingsglide;
extern int settingsglideTime;
extern uint8_t settingsnoteRange;
extern uint8_t settingsnumChords;
extern File settingsFile;
extern boolean settingsstacked;
extern int16_t settingsnotes[16][8];
extern int16_t settingsdefaultNotes[16][8];

void ChordOrgancopyDefaults();

void ChordOrganread();

// converting string to Float
float ChordOrgantoFloat(String settingValue);

// Converting String to integer and then to boolean
// 1 = true
// 0 = false
boolean ChordOrgantoBoolean(String settingValue);

void ChordOrganwrite();

void ChordOrganinit(boolean hasSD);

#endif
