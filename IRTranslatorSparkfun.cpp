/**
 * IRTranslatorSparkfun
 * Remote-Klasse fuer die Sparkfun-Remote (COM-11759 / https://www.sparkfun.com/products/11759).
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
#include "IRTranslatorSparkfun.h"

// #define DEBUG
#include "Debug.h"

// Codes, mit #debug im Hautprogrammn ausgelesen...
#define SPARKFUN_POWER  0x10EFD827
#define SPARKFUN_A      0x10EFF807
#define SPARKFUN_B      0x10EF7887
#define SPARKFUN_C      0x10EF58A7
#define SPARKFUN_UP     0x10EFA05F
#define SPARKFUN_DOWN   0x10EF00FF
#define SPARKFUN_LEFT   0x10EF10EF
#define SPARKFUN_RIGHT  0x10EF807F
#define SPARKFUN_SELECT 0x10EF20DF

void IRTranslatorSparkfun::printSignature() {
    Serial.println(F("Sparkfun COM-11759"));
}

byte IRTranslatorSparkfun::buttonForCode(unsigned long code) {
    switch (code) {
        case SPARKFUN_POWER:
            return REMOTE_BUTTON_TOGGLEBLANK;
        case SPARKFUN_A:
            return REMOTE_BUTTON_MODE;
        case SPARKFUN_LEFT:
        case SPARKFUN_B:
            return REMOTE_BUTTON_HOUR_PLUS;
        case SPARKFUN_RIGHT:
        case SPARKFUN_C:
            return REMOTE_BUTTON_MINUTE_PLUS;
        case SPARKFUN_UP:
            return REMOTE_BUTTON_BRIGHTER;
        case SPARKFUN_DOWN:
            return REMOTE_BUTTON_DARKER;
        case SPARKFUN_SELECT:
            return REMOTE_BUTTON_EXTMODE;
        default:
            return REMOTE_BUTTON_UNDEFINED;
    }
}
