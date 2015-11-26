/**
 * AnalogButton
 * Kleine Klasse zum Entprellen der Tasten auf rein anlogen Ports.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.0
 * @created  16.2.2015
 * @updated  -
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */
#ifndef ANALOG_BUTTON_H
#define ANALOG_BUTTON_H

#include "Arduino.h"

class AnalogButton {
public:
    AnalogButton(byte pin, boolean inverse);

    boolean pressed();

private:
    byte _pin;
    boolean _inverse;
    unsigned long _lastPressTime;
};

#endif
