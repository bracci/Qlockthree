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

struct color_t {
  byte red;
  byte green;
  byte blue;
};

const color_t defaultColors[] PROGMEM = {{0, 0, 0}, {250, 255, 200}, {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {0, 255, 128}, {230, 25, 255}, {255, 255, 0}};

enum eColors {
  color_rgb,
  color_white,
  color_red,
  color_green,
  color_darkblue,
  color_brightblue,
  color_pink,
  color_yellow,

  color_max,
  color_none
};


#endif // COLORS_H
