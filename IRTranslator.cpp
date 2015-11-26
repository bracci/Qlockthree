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
#include "IRTranslator.h"

byte IRTranslator::getRed() {
    return _red;
}

byte IRTranslator::getGreen() {
    return _green;
}

byte IRTranslator::getBlue() {
    return _blue;
}

Mode IRTranslator::getMode() {
  return _mode;
}

byte IRTranslator::getTransition() {
  return _transition;
}

void IRTranslator::setColor(byte red, byte green, byte blue) {
    _red = red;
    _green = green;
    _blue = blue;
}

void IRTranslator::setMode(Mode mode) {
  _mode = mode;
}

void IRTranslator::setTransition(byte transition) {
  _transition = transition;
}
