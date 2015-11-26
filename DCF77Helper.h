/**
 * DCF77Helper
 * Klasse um die Qualitaet der DCF77-Samples zu verbessern. Dazu wird der Minutenabstand
 * zwischen den empfangenen DCF77-Samples mit Hilfe der Echtzeituhr verglichen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.8
 * @created  19.3.2011
 * @updated  16.2.2015
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
 */
#ifndef DCF77HELPER_H
#define DCF77HELPER_H

#include "Arduino.h"
#include "MyRTC.h"
#include "MyDCF77.h"
#include "TimeStamp.h"
#include "Configuration.h"

class DCF77Helper {
public:
    DCF77Helper();

    void addSample(MyDCF77 dcf77, MyRTC rtc);
    boolean samplesOk();

private:
    byte _cursor;
    TimeStamp *_zeitstempelDcf77[DCF77HELPER_MAX_SAMPLES];
    TimeStamp *_zeitstempelRtc[DCF77HELPER_MAX_SAMPLES];
};

#endif
