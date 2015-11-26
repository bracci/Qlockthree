/**
 * IRTranslatorCLT
 * Remote-Klasse fuer die Lunartec-Remote (NX6612-901 / http://www.pearl.de/a-NX6612-3350.shtml?vid=901).
 *
 * @mc       Arduino/RBBB
 * @autor    Manuel Bracher / bracher _AT_ win _DOT_ com
 * @version  1.1
 * @created  5.5.2015
 * @updated  5.5.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */
#ifndef IRTRANSLATORCLT_H
#define IRTRANSLATORCLT_H

#include "IRTranslator.h"

class IRTranslatorCLT : public IRTranslator {
  public:
    void printSignature();
    byte buttonForCode(unsigned long code);
};

#endif
