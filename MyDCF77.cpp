/**
 * MyDCF77
 * Klasse fuer die Auswertung des DCF77-Zeitsignals.
 * Diese Klasse benötigt eine externe 1-Sekunden-Basis und einen
 * zusätzlichen Timer, der im Default-Fall alle 100ms ein Flag setzt.
 * Dieser feste Takt wird benötigt, um die zwingend erforderliche
 * Driftkorrektur durchführen zu können.
 * Diese Klasse geht nicht von einem 'sauberen' Signal aus,
 * und ermittelt die HIGH/LOWs statistisch.
 * 
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Mueller
 *           Vorlage von: Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.2
 * @created  21.3.2016
 * @updated  14.04.2016
 *
 * Versionshistorie:
 * V 1.0:   * Signalauswertealgoritmus komplett neu geschrieben! *
 *          Zuverlässigkeit der Zeitsynchronisation erhöht:
 *          - Drifts in der Phase werden durch eine automatische Offsetkorrektur bereinigt.
 *          - Damit ist die Erkennung der Schaltsekunde nach spätestens einer Minute
 *            zuverlässig möglich.
 *            Das Fehlen der korrekten Erkennung dieser Schaltsekunde in früheren Versionen
 *            verhinderte eine zuverlässige Zeitsynchronisation.
 *          - Deutlich exaktere Einstellung der Zeit dank Driftkorrektur möglich.        
 * V 1.1:   - Funktion für EXT_MODE_DCF_DEBUG eingeführt.
 *          - Umschaltung von Timer1 auf Timer2 in Header-Datei möglich. (Entfällt ab V 1.2)
 * V 1.2:   * Die Driftkorrektur benötigt keinen Timer mehr! *
 */
#include "MyDCF77.h"

//#define DEBUG
#include "Debug.h"

// Anzeige des Signalgraphen, nur wenn auch DEBUG gesetzt
#define DEBUG_SIGNAL
// Höhe des Signalgraphen, wenn DEBUG_SIGNAL gesetzt (Default: 40.)
#define DEBUG_SIGNAL_VIS_HEIGHT 40.

byte MyDCF77::DCF77Factors[] = {1, 2, 4, 8, 10, 20, 40, 80};

/**
 * Initialisierung mit dem Pin, an dem das Signal des Empfaengers anliegt
 */
MyDCF77::MyDCF77(byte signalPin, byte statusLedPin) : TimeStamp(0, 0, 0, 0, 0, 0) {
    _signalPin = signalPin;
#ifndef MYDCF77_SIGNAL_IS_ANALOG
    pinMode(_signalPin, INPUT);
#endif

    _statusLedPin = statusLedPin;
    pinMode(_statusLedPin, OUTPUT);
    digitalWrite(_statusLedPin, LOW);

    clearBits();
    clearBins();

    _dcf77Freq = 1000000/MYDCF77_SIGNAL_BINS;
    _dcf77LastTime = micros();
}

/**
 * Die LED ein- oder ausschalten.
 */
void MyDCF77::statusLed(boolean on) {
    if (on) {
        digitalWrite(_statusLedPin, HIGH);
    } else {
        digitalWrite(_statusLedPin, LOW);
    }
}

/**
 * Liegt ein Signal vom Empfaenger an?
 */
boolean MyDCF77::signal(boolean signalIsInverted) {
    boolean val;
#ifdef MYDCF77_SIGNAL_IS_ANALOG
    if (signalIsInverted) {
        val = analogRead(_signalPin) < MYDCF77_ANALOG_SIGNAL_TRESHOLD;
    } else {
        val = analogRead(_signalPin) > MYDCF77_ANALOG_SIGNAL_TRESHOLD;
    }
#else
    if (signalIsInverted) {
        val = (digitalRead(_signalPin) == LOW);
    } else {
        val = (digitalRead(_signalPin) == HIGH);
    }
#endif
    return val;
}

/**
 * Aufsammeln der Zustaende des DCF77-Signals.
 */
boolean MyDCF77::poll(boolean signalIsInverted) {
    boolean retVal = false;

    if (micros() - _dcf77LastTime >= _dcf77Freq) {
        _dcf77LastTime += _dcf77Freq;
        newCycle();
    }

    if (!_updateFromDCF77) {
        retVal = true;
        _updateFromDCF77 = -1;
    }

    if (_binsPointer >= 0) {             
        if (signal(signalIsInverted)) {
            // Array an _binsPointer-Position wird mit Messwerten gefüllt
            _bins[_binsPointer]++;
        }   
        _nPolls++;   
    }
       
    return retVal;
}

/**
 * Der ( 1 / MYDCF77_SIGNAL_BINS )-ste Teil einer Sekunde startet.
 * Muss von einem externen Zeitgeber, z. B. einer RTC, aufgerufen werden.
 *
 * Diese Funktion setzt updateFromDCF77 auf eine Zeitdauer, die abgewartet
 * wird, bevor die Funktion poll() (siehe oben) WAHR zurückgibt.
 * TRUE bedeutet, das Zeittelegramm wurde korrekt ausgewertet, die Zeitdaten
 * koennen mit den Gettern abgerufen werden.
 * FALSE bedeutet, die Auswertung laeuft oder war falsch, die Getter liefern
 * alte Informationen.
 */
void MyDCF77::newCycle() {

        // (1s / MYDCF77_SIGNAL_BINS) sind vorbei
        _binsPointer++;

        if (_updateFromDCF77 > 0)
            _updateFromDCF77--;
        
        // Springe zurück zu Bin 0 und analysiere die letzte Sekunde
        if (_binsPointer >= MYDCF77_SIGNAL_BINS) {
            _binsPointer = 0;

            unsigned int average = _nPolls * 0.17;
            _nPolls = 0;
            
            // Maximumsuche und Summenbildung
            unsigned int imax = 0;
            unsigned int isum = 0;
            byte pos = 0;
            for (byte i = 0; i < MYDCF77_SIGNAL_BINS; i++) {
               if (_bins[i] > imax) {
                  imax = _bins[i];
                  pos = i;
               }
               isum += _bins[i];
            }
          
            if (isum > average) {
                _bits[_bitsPointer] = 1;
            } else {
                _bits[_bitsPointer] = 0;
            }

            #ifdef DEBUG_SIGNAL
                OutputSignal(average, imax, isum);
            #endif

            _bitsPointer++;
            if (_bitsPointer > MYDCF77_TELEGRAMMLAENGE) {
                _bitsPointer = 0;
            }

            if (!isum) {
                if (decode()) {
                  /*
                   * Signal befindet sich zentriert im Sekundenintervall, 
                   * daher 500ms mit dem Einstellen der Uhr warten.
                   */
                    _updateFromDCF77 = MYDCF77_SIGNAL_BINS / 2;
                }
                clearBits();
            }
            _binsOffset += ((MYDCF77_SIGNAL_BINS-1)/2) - pos;
            _driftTimer++;
        }
        
        // MYDCF77_DRIFT_CORRECTION_TIME Sekunden sind vorbei
        if (_driftTimer >= MYDCF77_DRIFT_CORRECTION_TIME) {
            _driftTimer = 0;
          /* 
           * Wenn Offset < 0, muss gewartet werden
           * Wenn Offset > 0, muss früher begonnen werden
           * Ergänzung: Eigentlich müsste nPolls ebenfalls angepasst werden,
           * da die Berechnung von "average" im Moment der Driftkorrektur 
           * falsch ist.
           * Diese kleine Abweichung wird aber bewusst in Kauf genommen.
           */  
            _binsOffset /= MYDCF77_DRIFT_CORRECTION_TIME;
            _binsPointer = _binsOffset;
            clearBins();
            
            if (_binsPointer) {
                DEBUG_PRINT(F("Driftkorrektur erforderlich! Offset: "));
                DEBUG_PRINTLN(_binsPointer);
                DEBUG_FLUSH();
            }            
            _binsOffset = 0;
        }     

        // Lösche den nächsten Datenpunkt
        if (_binsPointer >= 0) {
          _bins[_binsPointer] = 0;
        }
}

void MyDCF77::OutputSignal(unsigned int average, unsigned int imax, unsigned int isum) {
    float t1 = imax / DEBUG_SIGNAL_VIS_HEIGHT;
    for (byte i = 0; i < MYDCF77_SIGNAL_BINS; i++ ) {
        if (i < 10) {
            DEBUG_PRINT(F("0"));
        }
        DEBUG_PRINT(i);
        DEBUG_PRINT(F(" "));
        for (byte j = 0; j < DEBUG_SIGNAL_VIS_HEIGHT; j++) {
            if ((_bins[i] / t1) > j) {
                DEBUG_PRINT("-");
            }
        }
        DEBUG_PRINTLN();
    }
    DEBUG_PRINT(F("Drift: "));
    DEBUG_PRINT(_binsOffset);
    DEBUG_PRINT(F(" Average: "));
    DEBUG_PRINT(average);
    DEBUG_PRINT(F(" Highcount: "));
    DEBUG_PRINTLN(isum);
    for (byte i = 0; i < MYDCF77_TELEGRAMMLAENGE; i++) {      
        DEBUG_PRINT(F(" "));
        DEBUG_PRINT(_bits[i]);
        if (i == _bitsPointer) {
          DEBUG_PRINT(".");
        }
    }
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(_bitsPointer);
    DEBUG_PRINTLN();
    DEBUG_FLUSH();
}

/**
 * Ein Bit im Array zum Debuggen (Anzeigen) bekommen.
 */
byte MyDCF77::getBitAtPos(byte pos) {
    return _bits[pos];
}

/*
 * Vergangene Minuten seit der letzten erfolgreichen DCF-Auswertung bekommen.
 */
unsigned int MyDCF77::getDcf77LastSuccessSyncMinutes() {
//    return ( (millis() - _dcf77lastSyncTime) / 60000 );
    return ( ( (millis() - _dcf77lastSyncTime) / 60000) % 1440 );
}

/*
 * Zeitpunkt der letzten erfolgreich DCF-Auswertung zurücksetzen.
 */
void MyDCF77::setDcf77SuccessSync() {
    _dcf77lastSyncTime = millis();
}

/**
 * Die passende Eckled zum Debuggen bekommen.
 */
byte MyDCF77::getDcf77ErrorCorner() {
    return _errorCorner;
}

/**
 * Die passende Eckled zum Debuggen updaten.
 */
void MyDCF77::updateDcf77ErrorCorner(boolean signalIsInverted) {
    if (signal(signalIsInverted)) {
        if (!_toggleSignal) {
            _toggleSignal = true;
            _errorCorner++;
            // Überlauf von _errorCorner ist unproblematisch
        }
    } else {
        _toggleSignal = false;   
    }
}

/**
 * Decodierung des Telegramms...
 */
boolean MyDCF77::decode() {
    int c = 0; // bitcount for checkbit
    boolean ok = true;

    DEBUG_PRINTLN(F("Decoding telegram..."));
    DEBUG_FLUSH();

    if (_bits[0] != 0) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit M failed."));
        DEBUG_FLUSH();
    }

    if (_bits[20] != 1) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit S failed."));
        DEBUG_FLUSH();
    }

    if (_bits[17] == _bits[18]) {
        ok = false;
        DEBUG_PRINTLN(F("Check Z1 != Z2 failed."));
        DEBUG_FLUSH();
    }

    //
    // minutes
    //
    _minutes = 0;
    c = 0;
    for (byte i = 21; i <= 27; i++) {
      if (_bits[i]) {
        _minutes += _bits[i] * DCF77Factors[i-21];
        c++;
      } 
    }
    DEBUG_PRINT(F("Minutes: "));
    DEBUG_PRINTLN(_minutes);
    DEBUG_FLUSH();
    if ((c + _bits[28]) % 2 != 0) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit P1: minutes failed."));
        DEBUG_FLUSH();
    }

    //
    // hour
    //
    _hours = 0;
    c = 0;
    for (byte i = 29; i <= 34; i++) {
      if (_bits[i]) {
        _hours += _bits[i] * DCF77Factors[i-29];
        c++;
      } 
    }
    DEBUG_PRINT(F("Hours: "));
    DEBUG_PRINTLN(_hours);
    DEBUG_FLUSH();
    if ((c + _bits[35]) % 2 != 0) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit P2: hours failed."));
        DEBUG_FLUSH();
    }

    //
    // date
    //
    _date = 0;
    c = 0;
    for (byte i = 36; i <= 41; i++) {
      if (_bits[i]) {
        _date += _bits[i] * DCF77Factors[i-36];
        c++;
      } 
    }
    DEBUG_PRINT(F("Date: "));
    DEBUG_PRINTLN(_date);
    DEBUG_FLUSH();

    //
    // day of week
    //
    _dayOfWeek = 0;
    for (byte i = 42; i <= 44; i++) {
      if (_bits[i]) {
        _dayOfWeek += _bits[i] * DCF77Factors[i-42];
        c++;
      } 
    }
    DEBUG_PRINT(F("Day of week: "));
    DEBUG_PRINTLN(_dayOfWeek);
    DEBUG_FLUSH();

    //
    // month
    //
    _month = 0;
    for (byte i = 45; i <= 49; i++) {
      if (_bits[i]) {
        _month += _bits[i] * DCF77Factors[i-45];
        c++;
      } 
    }
    DEBUG_PRINT(F("Month: "));
    DEBUG_PRINTLN(_month);
    DEBUG_FLUSH();

    //
    // year
    //
    _year = 0;
    for (byte i = 50; i <= 57; i++) {
      if (_bits[i]) {
        _year += _bits[i] * DCF77Factors[i-50];
        c++;
      } 
    }
    DEBUG_PRINT(F("Year: "));
    DEBUG_PRINTLN(_year);
    DEBUG_FLUSH();
    if ((c + _bits[58]) % 2 != 0) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit P3: date failed."));
        DEBUG_FLUSH();
    }

    if (_minutes > 59) {
        DEBUG_PRINTLN(F("Minutes out of range."));
        ok = false;
    }
    if (_hours > 23) {
        DEBUG_PRINTLN(F("Hours out of range."));
        ok = false;
    }
    if (_date > 31) {
        DEBUG_PRINTLN(F("Date out of range."));
        ok = false;
    }
    if (_month > 12) {
        DEBUG_PRINTLN(F("Month out of range."));
        ok = false;
    }

    if (!ok) {
        // discard date...
        _minutes = 0;
        _hours = 0;
        _date = 0;
        _dayOfWeek = 0;
        _month = 0;
        _year = 0;
    }

    return ok;
}

/*
 * Das Bits-Array loeschen.
 */
void MyDCF77::clearBits() {
    for (byte i = 0; i < MYDCF77_TELEGRAMMLAENGE; i++) {
        _bits[i] = 0;
    }
    _bitsPointer = 0;
}

/*
 * Das Bins-Array loeschen.
 */
void MyDCF77::clearBins() {
    for (byte i = 0; i < MYDCF77_SIGNAL_BINS; i++) {
        _bins[i] = 0;
    }   
}

//
// Getter
//


