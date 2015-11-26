/**
 * MyDCF77
 * Klasse fuer die Auswertung des DCF77-Zeitsignals.
 * Diese Klasse geht nicht von einem 'sauberen' Signal aus,
 * braucht aber eine externe 1-Sekunden-Basis. Sie ermittelt
 * die HIGH/LOWs statistisch.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  14.1.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:   - Erstellt.
 * V 1.1:   - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#include "MyDCF77.h"

// #define DEBUG
#include "Debug.h"

/**
 * Initialisierung mit dem Pin, an dem das Signal des Empfaengers anliegt
 */
MyDCF77::MyDCF77(byte signalPin, byte statusLedPin) {
    _signalPin = signalPin;
#ifndef MYDCF77_SIGNAL_IS_ANALOG
    pinMode(_signalPin, INPUT);
#endif

    _statusLedPin = statusLedPin;
    pinMode(_statusLedPin, OUTPUT);
    digitalWrite(_statusLedPin, LOW);

    clearBits();

    for (byte i = 0; i < MYDCF77_MEANCOUNT; i++) {
        _meanvalues[i] = MYDCF77_MEANSTARTVALUE;
    }
    _meanpointer = 0;
    _highcount = 0;
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
void MyDCF77::poll(boolean signalIsInverted) {
    if (signal(signalIsInverted)) {
        _highcount++;
    }
}

/**
 * Eine Sekunde startet. Muss von einem externen
 * Zeitgeber, z. B. einer RTC, aufgerufen werden.
 *
 * Zurueckgegeben wird ein Wahrheitswert.
 * TRUE bedeutet, das Zeittelegramm wurde korrekt ausgewertet, die Zeitdaten
 * koennen mit den Gettern abgerufen werden.
 * FALSE bedeutet, die Auswertung laeuft oder war falsch, die Getter liefern
 * alte Informationen.
 */
boolean MyDCF77::newSecond() {
    boolean retVal = false;

    if (_highcount != 0) {
        // Daten
        _meanvalues[_meanpointer] = _highcount;
        _meanpointer++;
        if (_meanpointer > MYDCF77_MEANCOUNT) {
            _meanpointer = 0;
        }

        unsigned long average = 0;
        for (byte i = 0; i < MYDCF77_MEANCOUNT; i++) {
            average += _meanvalues[i];
        }
        average /= MYDCF77_MEANCOUNT;

        DEBUG_PRINT(F("average = "));
        DEBUG_PRINT(average);
        DEBUG_PRINT(F("; highcount = "));
        DEBUG_PRINT(_highcount);

        if (_highcount > average) {
            _bits |= (uint64_t) 0x01 << _bitsPointer;
            DEBUG_PRINTLN(F("; HIGH"));
        } else {
            _bits |= (uint64_t) 0x00 << _bitsPointer;
            DEBUG_PRINTLN(F("; LOW"));
        }
        _bitsPointer++;
        if (_bitsPointer > MYDCF77_TELEGRAMMLAENGE) {
            _bitsPointer = 0;
        }
    } else {
        // Pause
        DEBUG_PRINTLN(F("PAUSE"));
        retVal = decode();
        _bitsPointer = 0;
        clearBits();
    }

    _highcount = 0;

    return retVal;
}

/**
 * Ein Bit im Array zum Debuggen (Anzeigen) bekommen.
 */
byte MyDCF77::getBitAtPos(byte pos) {
    return  (_bits >> pos) & 0x01;
}

/**
 * Den Pointer zum Debuggen (Anzeigen) bekommen.
 */
byte MyDCF77::getBitPointer() {
    return _bitsPointer;
}

/**
 * Decodierung des Telegramms...
 */
boolean MyDCF77::decode() {
    int c = 0; // bitcount for checkbit
    boolean ok = true;

    DEBUG_PRINTLN(F("Decoding telegram..."));
    DEBUG_FLUSH();

    if (!getBitAtPos(20)) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit S failed."));
        DEBUG_FLUSH();
    }

    if (getBitAtPos(17) == getBitAtPos(18)) {
        ok = false;
        DEBUG_PRINTLN(F("Check Z1 != Z2 failed."));
        DEBUG_FLUSH();
    }

    //
    // minutes
    //
    _minutes = 0;
    c = 0;
    if (getBitAtPos(21)) {
        c++;
        _minutes += 1;
    }
    if (getBitAtPos(22)) {
        c++;
        _minutes += 2;
    }
    if (getBitAtPos(23)) {
        c++;
        _minutes += 4;
    }
    if (getBitAtPos(24)) {
        c++;
        _minutes += 8;
    }
    if (getBitAtPos(25)) {
        c++;
        _minutes += 10;
    }
    if (getBitAtPos(26)) {
        c++;
        _minutes += 20;
    }
    if (getBitAtPos(27)) {
        c++;
        _minutes += 40;
    }
    DEBUG_PRINT(F("Minutes: "));
    DEBUG_PRINTLN(_minutes);
    DEBUG_FLUSH();
    if ((c + getBitAtPos(28)) % 2 != 0) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit P1: minutes failed."));
        DEBUG_FLUSH();
    }

    //
    // hour
    //
    _hours = 0;
    c = 0;
    if (getBitAtPos(29)) {
        c++;
        _hours += 1;
    }
    if (getBitAtPos(30)) {
        c++;
        _hours += 2;
    }
    if (getBitAtPos(31)) {
        c++;
        _hours += 4;
    }
    if (getBitAtPos(32)) {
        c++;
        _hours += 8;
    }
    if (getBitAtPos(33)) {
        c++;
        _hours += 10;
    }
    if (getBitAtPos(34)) {
        c++;
        _hours += 20;
    }
    DEBUG_PRINT(F("Hours: "));
    DEBUG_PRINTLN(_hours);
    DEBUG_FLUSH();
    if ((c + getBitAtPos(35)) % 2 != 0) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit P2: hours failed."));
        DEBUG_FLUSH();
    }

    //
    // date
    //
    _date = 0;
    c = 0;
    if (getBitAtPos(36)) {
        c++;
        _date += 1;
    }
    if (getBitAtPos(37)) {
        c++;
        _date += 2;
    }
    if (getBitAtPos(38)) {
        c++;
        _date += 4;
    }
    if (getBitAtPos(39)) {
        c++;
        _date += 8;
    }
    if (getBitAtPos(40)) {
        c++;
        _date += 10;
    }
    if (getBitAtPos(41)) {
        c++;
        _date += 20;
    }
    DEBUG_PRINT(F("Date: "));
    DEBUG_PRINTLN(_date);
    DEBUG_FLUSH();

    //
    // day of week
    //
    _dayOfWeek = 0;
    if (getBitAtPos(42)) {
        c++;
        _dayOfWeek += 1;
    }
    if (getBitAtPos(43)) {
        c++;
        _dayOfWeek += 2;
    }
    if (getBitAtPos(44)) {
        c++;
        _dayOfWeek += 4;
    }
    DEBUG_PRINT(F("Day of week: "));
    DEBUG_PRINTLN(_dayOfWeek);
    DEBUG_FLUSH();

    //
    // month
    //
    _month = 0;
    if (getBitAtPos(45)) {
        c++;
        _month += 1;
    }
    if (getBitAtPos(46)) {
        c++;
        _month += 2;
    }
    if (getBitAtPos(47)) {
        c++;
        _month += 4;
    }
    if (getBitAtPos(48)) {
        c++;
        _month += 8;
    }
    if (getBitAtPos(49)) {
        c++;
        _month += 10;
    }
    DEBUG_PRINT(F("Month: "));
    DEBUG_PRINTLN(_month);
    DEBUG_FLUSH();

    //
    // year
    //
    _year = 0;
    if (getBitAtPos(50)) {
        c++;
        _year += 1;
    }
    if (getBitAtPos(51)) {
        c++;
        _year += 2;
    }
    if (getBitAtPos(52)) {
        c++;
        _year += 4;
    }
    if (getBitAtPos(53)) {
        c++;
        _year += 8;
    }
    if (getBitAtPos(54)) {
        c++;
        _year += 10;
    }
    if (getBitAtPos(55)) {
        c++;
        _year += 20;
    }
    if (getBitAtPos(56)) {
        c++;
        _year += 40;
    }
    if (getBitAtPos(57)) {
        c++;
        _year += 80;
    }
    DEBUG_PRINT(F("Year: "));
    DEBUG_PRINTLN(_year);
    DEBUG_FLUSH();
    if ((c + getBitAtPos(58)) % 2 != 0) {
        ok = false;
        DEBUG_PRINTLN(F("Check-bit P3: date failed."));
        DEBUG_FLUSH();
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

/**
 * Das Zeittelegramm als String bekommen
 */
//char* MyDCF77::asString() {
//    _cDateTime[0] = 0;
//    char temp[5];
//
//    // build the string...
//    if (_hours < 10) {
//        sprintf(temp, "0%d:", _hours);
//        strncat(_cDateTime, temp, strlen(temp));
//    } else {
//        sprintf(temp, "%d:", _hours);
//        strncat(_cDateTime, temp, strlen(temp));
//    }
//
//    if (_minutes < 10) {
//        sprintf(temp, "0%d ", _minutes);
//        strncat(_cDateTime, temp, strlen(temp));
//    } else {
//        sprintf(temp, "%d ", _minutes);
//        strncat(_cDateTime, temp, strlen(temp));
//    }
//
//    if (_date < 10) {
//        sprintf(temp, "0%d.", _date);
//        strncat(_cDateTime, temp, strlen(temp));
//    } else {
//        sprintf(temp, "%d.", _date);
//        strncat(_cDateTime, temp, strlen(temp));
//    }
//
//    if (_month < 10) {
//        sprintf(temp, "0%d.", _month);
//        strncat(_cDateTime, temp, strlen(temp));
//    } else {
//        sprintf(temp, "%d.", _month);
//        strncat(_cDateTime, temp, strlen(temp));
//    }
//
//    sprintf(temp, "%d", _year);
//    strncat(_cDateTime, temp, strlen(temp));
//
//    return _cDateTime;
//}

/**
 * Das Bits-Array loeschen.
 */
void MyDCF77::clearBits() {
    _bits = 0;
    _bitsPointer = 0;
}

//
// Getter
//

byte MyDCF77::getMinutes() {
    return _minutes;
}

byte MyDCF77::getHours() {
    return _hours;
}

byte MyDCF77::getDate() {
    return _date;
}

byte MyDCF77::getDayOfWeek() {
    return _dayOfWeek;
}

byte MyDCF77::getMonth() {
    return _month;
}

byte MyDCF77::getYear() {
    return _year;
}
