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
#ifndef MYDCF77_H
#define MYDCF77_H

#include "Arduino.h"
#include "Configuration.h"

class MyDCF77 {
public:
    MyDCF77(byte signalPin, byte statusLedPin);

    void statusLed(boolean on);

    void poll(boolean signalIsInverted);
    boolean newSecond();

    byte getBitAtPos(byte pos);
    byte getBitPointer();

    byte getMinutes();
    byte getHours();

    byte getDate();
    byte getDayOfWeek();
    byte getMonth();
    byte getYear();

//    char* asString();

    boolean signal(boolean signalIsInverted);

private:
    byte _signalPin;
    byte _statusLedPin;

    word _highcount;
    word _meanvalues[MYDCF77_MEANCOUNT];
    byte _meanpointer;

    byte _minutes;
    byte _hours;

    byte _date;
    byte _dayOfWeek;
    byte _month;
    byte _year;

//    char _cDateTime[17];

    uint64_t _bits;
    byte _bitsPointer;

    boolean decode();

    void clearBits();

    static byte DCFFactors[];
};

#endif
