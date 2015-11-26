/**
 * IRTranslatorLunartec
 * Remote-Klasse fuer die Lunartec-Remote (NX6612-901 / http://www.pearl.de/a-NX6612-3350.shtml?vid=901).
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
#ifndef IRTRANSLATORLUNARTEC_H
#define IRTRANSLATORLUNARTEC_H

#include "Arduino.h"
#include "IRTranslator.h"

class IRTranslatorLunartec : public IRTranslator {
public:
    void printSignature();
    byte buttonForCode(unsigned long code);
};

#endif
