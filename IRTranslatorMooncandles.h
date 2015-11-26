/**
 * IRTranslatorMooncandles
 * Remote-Klasse fuer die Mooncandles-Remote (z. B. http://www.amazon.de/dp/B006L5YO78).
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  7.2.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt..
 */
#ifndef IRTRANSLATORMOONCANDLES_H
#define IRTRANSLATORMOONCANDLES_H

#include "Arduino.h"
#include "IRTranslator.h"

class IRTranslatorMooncandles : public IRTranslator {
public:
    void printSignature();
    byte buttonForCode(unsigned long code);
};

#endif
