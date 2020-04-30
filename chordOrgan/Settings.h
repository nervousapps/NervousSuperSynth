#ifndef Settings_h
#define Settings_h

#include <SD.h>

boolean settingsstacked = false;
boolean settingsextraWaves = false;
boolean settingsglide = false;
int settingsglideTime = 50;
uint8_t settingsnoteRange = 38;
uint8_t settingsnumChords = 0;
File settingsFile;
// Initialise Array with 255s, to identify unfilled elements when reading from SD card
int16_t settingsnotes[16][8] = {
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
  {
      255,255,255,255,255,255,255,255}
  ,
};
int16_t settingsdefaultNotes[16][8] = {
  {
      0,4,7,12,0,255,255,255}
  ,
  {
      0,3,7,12,0,255,255,255}
  ,
  {
      0,4,7,11,0,255,255,255}
  ,
  {
      0,3,7,10,0,255,255,255}
  ,
  {
      0,4,7,11,14,255,255,255}
  ,
  {
      0,3,7,10,14,255,255,255}
  ,
  {
      0,5,7,12,0,255,255,255}
  ,
  {
      0,7,12,0,7,255,255,255}
  ,
  {
      0,5,12,0,5,255,255,255}
  ,
  {
      0,4,7,8,0,255,255,255}
  ,
  {
      0,3,7,8,0,255,255,255}
  ,
  {
      0,3,6,0,3,255,255,255}
  ,
  {
      0,4,8,0,4,255,255,255}
  ,
  {
      0,0,0,0,0,255,255,255}
  ,
  {
      -12,-12,0,0,0,255,255,255}
  ,
  {
      -12,0,0,12,24,255,255,255}
  ,
};


void ChordOrgancopyDefaults() {
	for(int i=0;i<16;i++) {
		for(int a=0;a<8;a++) {
			settingsnotes[i][a] = settingsdefaultNotes[i][a];
		}
	}
	settingsnumChords = 16;
}

void ChordOrganread() {
    settingsnumChords = 0;

    char character;
    int note = 0;
    String settingValue;

    int NONE = 0;
    int CHORD = 1;
    int SETTING = 2;
    int state = NONE;

    settingsFile = SD.open("CHORDORG.TXT");

    while (settingsFile.available()) {

        character = settingsFile.read();

        if (character == '[') {
            if(settingsnumChords < 16) {
                // Serial.println("Enter Chord");
                state = CHORD;
            }
        } else if(character == '!') {
            state = SETTING;
        }

        if(state == CHORD) {
            if (character == ',') {
            	#ifdef DEBUG_CHORDS
                Serial.print("Note ");
                Serial.println(settingValue.toInt());
                #endif
                settingsnotes[settingsnumChords][note] = settingValue.toInt();
                settingValue = "";
                note++;
            } else if(character == ']') {
            	#ifdef DEBUG_CHORDS
                Serial.print("Note ");
                Serial.println(settingValue.toInt());
                #endif
                settingsnotes[settingsnumChords][note] = settingValue.toInt();
                settingValue = "";
                settingsnumChords++;
                note = 0;
                // Serial.println("End Chord");
                state = NONE;
            } else {
                settingValue += character;
            }

        } else if(state == SETTING) {
            if(character == '\n') {
                //
                Serial.print("Config ");
                Serial.print(settingValue);
                Serial.println(".");

                if(settingValue.startsWith("!WAVES")) {
                    settingsextraWaves = true;
                } else if(settingValue.startsWith("!GLIDE")) {
                    settingsglide = true;
                    int spacePos = settingValue.indexOf(' ');
                    if(spacePos > 0) {
                        settingsglideTime = settingValue.substring(spacePos).toInt();
                        if(settingsglideTime < 5) settingsglideTime = 5;
                        if(settingsglideTime > 300) settingsglideTime = 300;
                    }

                } else if(settingValue.startsWith("!RANGE")) {
                    int spacePos = settingValue.indexOf(' ');
                    if(spacePos > 0) {
                        settingsnoteRange = settingValue.substring(spacePos).toInt();
                        if(settingsnoteRange < 12) settingsnoteRange = 12;
                        if(settingsnoteRange > 72) settingsnoteRange = 72;
                    }
                } else if(settingValue.startsWith("!STACK")) {
                	settingsstacked = true;
                } else {
                    Serial.print("Unknown option:");
                    Serial.print(settingValue);
                    Serial.println(":");
                }
                settingValue = "";
                state = NONE;
            } else {
                settingValue += character;
            }
        }
    }
    settingsFile.close();
}

// converting string to Float
float ChordOrgantoFloat(String settingValue){
    char floatbuf[settingValue.length()];
    settingValue.toCharArray(floatbuf, sizeof(floatbuf));
    float f = atof(floatbuf);
    return f;
}

// Converting String to integer and then to boolean
// 1 = true
// 0 = false
boolean ChordOrgantoBoolean(String settingValue) {
    if(settingValue.toInt()==1){
        return true;
    }
    else {
        return false;
    }
}


void ChordOrganwrite() {
    // Delete the old One
    SD.remove("CHORDORG.TXT");
    // Create new one
    settingsFile = SD.open("CHORDORG.TXT", FILE_WRITE);
    //  // writing in the file works just like regular print()/println() function

    settingsFile.println("o   o o   o  o-o  o-O-o   o-o   o-O-o o  o o-O-o o   o  o-o ");
    settingsFile.println("|\\ /| |   | |       |    /        |   |  |   |   |\\  | o    ");
    settingsFile.println("| O | |   |  o-o    |   O         |   O--O   |   | \\ | |  -o ");
    settingsFile.println("|   | |   |     |   |    \\        |   |  |   |   |  \\| o   | ");
    settingsFile.println("o   o  o-o  o--o  o-O-o   o-o     o   o  o o-O-o o   o  o-o ");
    settingsFile.println("");
    settingsFile.println("  o-o o  o  o-o  o--o  o-o        o-o  o--o   o-o    O  o   o ");
    settingsFile.println(" /    |  | o   o |   | |  \\      o   o |   | o      / \\ |\\  | ");
    settingsFile.println("O     O--O |   | O-Oo  |   O     |   | O-Oo  |  -o o---o| \\ | ");
    settingsFile.println(" \\    |  | o   o |  \\  |  /      o   o |  \\  o   | |   ||  \\| ");
    settingsFile.println("  o-o o  o  o-o  o   o o-o        o-o  o   o  o-o  o   oo   o");
    settingsFile.println("");
    settingsFile.println("Edit chord shapes into the spaces below.");
    settingsFile.println("No more than 16 chords and up to 8 notes per chord.");
    settingsFile.println("Anything outside the square brackets is ignored");
    settingsFile.println("Reduce clicks on chord changes by giving all chords");
    settingsFile.println("the same number of notes.");
    settingsFile.println("");

    settingsFile.println("1  [0,4,7,12,0] Major");
    settingsFile.println("2  [0,3,7,12,0] Minor");
    settingsFile.println("3  [0,4,7,11,0] Major 7th");
    settingsFile.println("4  [0,3,7,10,0] Minor 7th");
    settingsFile.println("5  [0,4,7,11,14] Major 9th");
    settingsFile.println("6  [0,3,7,10,14] Minor 9th");
    settingsFile.println("7  [0,5,7,12,0] Suspended 4th");
    settingsFile.println("8  [0,7,12,0,7] Power 5th");
    settingsFile.println("9  [0,5,12,0,5] Power 4th");
    settingsFile.println("10 [0,4,7,8,0] Major 6th");
    settingsFile.println("11 [0,3,7,8,0] Minor 6th");
    settingsFile.println("12 [0,3,6,0,3] Diminished");
    settingsFile.println("13 [0,4,8,0,4] Augmented");
    settingsFile.println("14 [0,0,0,0,0] Root");
    settingsFile.println("15 [-12,-12,0,0,0] Sub Octave");
    settingsFile.println("16 [-12,0,0,12,24] 2 up 1 down octaves");

    //
    // close the file:
    settingsFile.close();
    //Serial.println("Writing done.");
}

void ChordOrganinit(boolean hasSD) {

	if(!hasSD) {
		// Configure defaults
		ChordOrgancopyDefaults();
	} else {
	    File root = SD.open("/");
	    if (SD.exists("CHORDORG.TXT")) {
	        ChordOrganread();
	    }
	    else {

	#ifdef DEBUG_MODE
	        Serial.println("Settings file not found, writing new settings");
	#endif
	        ChordOrganwrite();
	        ChordOrganread();
	    };
	}

}

#endif
