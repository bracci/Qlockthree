/**
 * IRTranslator
 * Abstrakte Klasse fuer die Umsetzung von Fernbedienungs-Codes.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  7.2.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#ifndef COLORS_H
#define COLORS_H

typedef struct color_s {
  byte red;
  byte green;
  byte blue;
}color_t;

const color_t defaultColors[] PROGMEM = { 
  {255, 255, 255}, 
  {255, 0, 0}, 
  {0, 255, 0}, 
  {0, 0, 255}, 
  {0, 255, 128}, 
  {230, 25, 255}, 
  {255, 255, 0},
  {255, 64, 0},
  {255, 128, 0},
  {255, 196, 0},
  {255, 255, 0},
  {0, 255, 64},
  {0, 255, 128},
  {0, 255, 196},
  {0, 255, 255},
  {64, 0, 255},
  {128, 0, 255},
  {196, 0, 255},
  {255, 0, 255},
  };

enum eColors : byte{
  color_white,
  color_red,
  color_green,
  color_darkBlue,
  color_brightBlue,
  color_pink,
  color_yellow,
  color_orange1,
  color_orange2,
  color_orange3,
  color_orange4,
  color_turquois1,
  color_turquois2,
  color_turquois3,
  color_turquois4,
  color_purple1,
  color_purple2,
  color_purple3,
  color_purple4,
  color_single_max = color_purple4,
  
  color_rgb_continuous,
  color_rgb_step,
  color_max = color_rgb_step,
  color_none
};

#endif // COLORS_H
