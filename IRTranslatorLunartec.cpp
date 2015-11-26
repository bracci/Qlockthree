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
#include "IRTranslatorLunartec.h"

// #define DEBUG
#include "Debug.h"

// Codes, mit #DEBUG im Hauptprogramm ausgelesen...
#define LUNBARTEC_BRIGHTER 0xFF906F
#define LUNBARTEC_DARKER   0xFFB847
#define LUNBARTEC_OFF      0xFFF807
#define LUNBARTEC_ON       0xFFB04F
#define LUNBARTEC_R        0xFF9867
#define LUNBARTEC_G        0xFFD827
#define LUNBARTEC_B        0xFF8877
#define LUNBARTEC_W        0xFFA857
#define LUNBARTEC_FLASH    0xFFB24D
#define LUNBARTEC_STROBE   0xFF00FF
#define LUNBARTEC_FADE     0xFF58A7
#define LUNBARTEC_SMOOTH   0xFF30CF

#define LUNBARTEC_ROTGELB_1 0xFFE817
#define LUNBARTEC_ROTGELB_2 0xFF02FD
#define LUNBARTEC_ROTGELB_3 0xFF50AF
#define LUNBARTEC_ROTGELB_4 0xFF38C7

#define LUNBARTEC_GRUENBLAU_1 0xFF48B7
#define LUNBARTEC_GRUENBLAU_2 0xFF32CD
#define LUNBARTEC_GRUENBLAU_3 0xFF7887
#define LUNBARTEC_GRUENBLAU_4 0xFF28D7

#define LUNBARTEC_BLAUROT_1 0xFF6897
#define LUNBARTEC_BLAUROT_2 0xFF20DF
#define LUNBARTEC_BLAUROT_3 0xFF708F
#define LUNBARTEC_BLAUROT_4 0xFFF00F

void IRTranslatorLunartec::printSignature() {
    Serial.println(F("Lunartec NX6612-901"));
}

byte IRTranslatorLunartec::buttonForCode(unsigned long code) {
    switch (code) {
        case LUNBARTEC_BRIGHTER:
            return REMOTE_BUTTON_BRIGHTER;
        case LUNBARTEC_DARKER:
            return REMOTE_BUTTON_DARKER;
        case LUNBARTEC_OFF:
            return REMOTE_BUTTON_BLANK;
        case LUNBARTEC_ON:
            return REMOTE_BUTTON_RESUME;
        case LUNBARTEC_R:
            setColor(255, 0, 0);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_G:
            setColor(0, 255, 0);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_B:
            setColor(0, 0, 255);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_W:
            setColor(255, 255, 225);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_FLASH:
            return REMOTE_BUTTON_MODE;
        case LUNBARTEC_STROBE:
            return REMOTE_BUTTON_MINUTE_PLUS;
        case LUNBARTEC_FADE:
            return REMOTE_BUTTON_HOUR_PLUS;
        case LUNBARTEC_SMOOTH:
            return REMOTE_BUTTON_EXTMODE;

        case LUNBARTEC_ROTGELB_1:
            setColor(255, 64, 0);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_ROTGELB_2:
            setColor(255, 128, 0);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_ROTGELB_3:
            setColor(255, 196, 0);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_ROTGELB_4:
            setColor(255, 255, 0);
            return REMOTE_BUTTON_SETCOLOR;

        case LUNBARTEC_GRUENBLAU_1:
            setColor(0, 255, 64);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_GRUENBLAU_2:
            setColor(0, 255, 128);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_GRUENBLAU_3:
            setColor(0, 255, 196);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_GRUENBLAU_4:
            setColor(0, 255, 255);
            return REMOTE_BUTTON_SETCOLOR;

        case LUNBARTEC_BLAUROT_1:
            setColor(64, 0, 255);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_BLAUROT_2:
            setColor(128, 0, 255);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_BLAUROT_3:
            setColor(196, 0, 255);
            return REMOTE_BUTTON_SETCOLOR;
        case LUNBARTEC_BLAUROT_4:
            setColor(255, 0, 255);
            return REMOTE_BUTTON_SETCOLOR;

        default:
            return REMOTE_BUTTON_UNDEFINED;
    }
}
