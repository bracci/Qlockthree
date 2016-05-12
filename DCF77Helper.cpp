/**
 * DCF77Helper
 * Klasse um die Qualitaet der DCF77-Samples zu verbessern. Dazu wird der Minutenabstand
 * zwischen den empfangenen DCF77-Samples mit Hilfe der Echtzeituhr verglichen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.8a
 * @created  19.3.2011
 * @updated  29.03.2016 (ergänzt durch A. Mueller)
 *
 * Versionshistorie:
 * V 1.1:  - Fehler in der Initialisierung behoben.
 * V 1.2:  - Umstellung auf TimeStamps und Verbesserung des Plausibilitaetstests.
 * V 1.3:  - Fehler im samplesOk() behoben.
 * V 1.4:  - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
 * V 1.5:  - Strings in PROGMEM ausgelagert.
 * V 1.6:  - Optimierung hinsichtlich Speicherbedarf.
 * V 1.7:  - Verbessertes Debugging.
 * V 1.8:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 * V 1.8a: - Datum wird jetzt ebenfalls geprüft (via getMinutesOfCentury())
 */
#include "DCF77Helper.h"

//#define DEBUG
#include "Debug.h"

/**
 * Initialisierung und das Array 'falsch' vorbelegen, damit der Zeitabstand
 * der Samples zum Anfang nicht stimmt.
 */
DCF77Helper::DCF77Helper() {
    _cursor = 0;
    for (byte i = 0; i < DCF77HELPER_MAX_SAMPLES; i++) {
        _zeitstempelDcf77[i] = new TimeStamp(i, i, i, i, i, i);
        _zeitstempelRtc[i] = new TimeStamp(100, 0, 0, 0, 0, 0);
    }
}

/**
 * Einen neuen Sample hinzufuegen.
 */
void DCF77Helper::addSample(MyDCF77* dcf77, MyRTC* rtc) {
    DEBUG_PRINT(F("Adding sample from dcf77: "));
    _zeitstempelDcf77[_cursor]->set(dcf77);
    DEBUG_PRINTLN(_zeitstempelDcf77[_cursor]->asString());

    DEBUG_PRINT(F("Adding sample from rtc: "));
    _zeitstempelRtc[_cursor]->set(rtc);
    DEBUG_PRINTLN(_zeitstempelRtc[_cursor]->asString());
    DEBUG_FLUSH();

    _cursor++;
    if (_cursor == DCF77HELPER_MAX_SAMPLES) {
        _cursor = 0;
    }
}

/**
 * Die Samples vergleichen
 */
boolean DCF77Helper::samplesOk() {
    boolean ret = true;
    for (byte i = 0; i < DCF77HELPER_MAX_SAMPLES - 1; i++) {
        #ifdef DEBUG
            long minuteDiffDcf77 = _zeitstempelDcf77[i]->getMinutesOfCentury() - _zeitstempelDcf77[i + 1]->getMinutesOfCentury();
            long minuteDiffRtc = _zeitstempelRtc[i]->getMinutesOfCentury() - _zeitstempelRtc[i + 1]->getMinutesOfCentury();
        #endif
        long diff = _zeitstempelDcf77[i]->getMinutesOfCentury() - _zeitstempelDcf77[i + 1]->getMinutesOfCentury()
                 -( _zeitstempelRtc[i]->getMinutesOfCentury() - _zeitstempelRtc[i + 1]->getMinutesOfCentury() );
        // Teste den Minutenabstand (über den gesamten Datumsbereich) zwischen den Zeitstempeln...
        if ( abs(diff) > 1 ) {
            DEBUG_PRINT(F("Diff #"));
            DEBUG_PRINT(i);
            DEBUG_PRINT(F(" distance is wrong ("));
            DEBUG_PRINT(minuteDiffDcf77);
            DEBUG_PRINT(F("!="));
            DEBUG_PRINT(minuteDiffRtc);
            DEBUG_PRINTLN(F(")."));
            DEBUG_FLUSH();
            ret = false;
        } else {
            DEBUG_PRINT(F("Diff #"));
            DEBUG_PRINT(i);
            DEBUG_PRINT(F(" distance is ok ("));
            DEBUG_PRINT(minuteDiffDcf77);
            DEBUG_PRINT(F("~="));
            DEBUG_PRINT(minuteDiffRtc);
            DEBUG_PRINTLN(F(")."));
            DEBUG_FLUSH();
        }
    }

    return ret;
}

