#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include <LiquidCrystalFast.h>
#include <HardwarePins.h>

extern LiquidCrystalFast lcd;

extern int LCD_NB_COLUMNS;

/* Caractères personnalisés */
extern byte START_DIV_0_OF_1[8]; // Char début 0 / 1

extern byte START_DIV_1_OF_1[8]; // Char début 1 / 1

extern byte DIV_0_OF_2[8]; // Char milieu 0 / 2

extern byte DIV_1_OF_2[8]; // Char milieu 1 / 2

extern byte DIV_2_OF_2[8]; // Char milieu 2 / 2

extern byte END_DIV_0_OF_1[8]; // Char fin 0 / 1

extern byte END_DIV_1_OF_1[8]; // Char fin 1 / 1

/**
 * Fonction de configuration de l'écran LCD pour la barre de progression.
 * Utilise les caractères personnalisés de 0 à 6 (7 reste disponible).
 */
void setup_progressbar();

void setup_lcd();

/**
 * Fonction dessinant la barre de progression.
 *
 * @param percent Le pourcentage à afficher.
 */
void draw_progressbar(byte percent);

#endif
