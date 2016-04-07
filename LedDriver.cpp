/**
 * LedDriver
 * Abstrakte Klasse fuer einen LED-Treiber. Hiervon koennen andere Implementierungen
 * erben (75HC595, MAX7219, AS1130 etc.)
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.5
 * @created  18.1.2013
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - printSignature() eingefuehrt.
 * V 1.2:  - Setzen und lesen von Pixel eingefuehrt.
 * V 1.3:  - Anpassung auf Helligkeit in Prozent.
 * V 1.4:  - Getter fuer Helligkeit eingefuehrt.
 * V 1.5:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#include "LedDriver.h"

void LedDriver::setPixelInScreenBuffer(byte x, byte y, word matrix[16]) {
  matrix[y] |= 0b1000000000000000 >> x;
}

boolean LedDriver::getPixelFromScreenBuffer(byte x, byte y, word matrix[16]) {
  return (matrix[y] & (0b1000000000000000 >> x)) == (0b1000000000000000 >> x);
}

void LedDriver::demoTransition() {
  _demoTransition = true;
}

void LedDriver::resetWheelPos() {
  _wheelPos = 0;
  _lastColorUpdate = millis();
}

