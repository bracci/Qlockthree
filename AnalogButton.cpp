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
#include "AnalogButton.h"

// #define DEBUG
#include "Debug.h"

#include "Configuration.h"

/**
 * Initialisierung mit dem Pin, an dem der Taster haengt.
 *
 * @param  pin: der Pin, an dem der Taster haengt
 *         inverse: Schaltverhalten umdrehen? (HIGH/LOW)
 */
AnalogButton::AnalogButton(byte pin, boolean inverse) {
    _pin = pin;
    _lastPressTime = 0;
    _inverse = inverse;
}

/**
 * Wurde der Taster gedrueckt?
 */
boolean AnalogButton::pressed() {
    boolean _retVal = false;

    if (millis() < _lastPressTime) {
        // wir hatten einen Ueberlauf...
        _lastPressTime = millis();
    }

    if (!_inverse) {
        if ((analogRead(_pin) >= 512) && (_lastPressTime + BUTTON_TRESHOLD < millis())) {
            _lastPressTime = millis();
            _retVal = true;
        }
    } else {
        if ((analogRead(_pin) < 512) && (_lastPressTime + BUTTON_TRESHOLD < millis())) {
            _lastPressTime = millis();
            _retVal = true;
        }
    }

    return _retVal;
}
