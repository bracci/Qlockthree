/**
 * TimeStamp
 * Klasse fuer die Kapselung eines Zeitstempels. Dadurch lassen sich
 * Zeiten aus der Echtzeituhr und von dem DCF77-Empfaenger
 * leichter verarbeiten und vergleichen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.7
 * @created  2.3.2011
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.1:  - Fehler in toString() behoben.
 * V 1.2:  - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
 * V 1.3:  - neuer Konstruktor, neue Methoden.
 * V 1.4:  - getMinutesOf12HoursDay eingefuehrt.
 * V 1.5:  - Optimierung hinsichtlich Speicherbedarf.
 * V 1.6:  - Verbessertes Debugging & leeren Konstruktor entfernt.
 * V 1.7:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#include "TimeStamp.h"

// #define DEBUG
#include "Debug.h"

TimeStamp::TimeStamp(byte minutes, byte hours, byte date, byte dayOfWeek, byte month, byte year) {
    set(minutes, hours, date, dayOfWeek, month, year);
}

TimeStamp::TimeStamp(MyDCF77 dcf77) {
    setFrom(dcf77);
}

TimeStamp::TimeStamp(MyRTC rtc) {
    setFrom(rtc);
}

byte TimeStamp::getMinutes() {
    return _minutes;
}

unsigned int TimeStamp::getMinutesOfDay() {
    return _minutes + 60 * _hours;
}

/**
 * Minuten des Tages ohne die Beruecksichtigung von 12/24 Stunden
 * (fuer den Wecker)...
 */
unsigned int TimeStamp::getMinutesOf12HoursDay(int minutesDiff) {
    int ret = _minutes + 60 * _hours + minutesDiff;

    while (ret > 12 * 60) {
        ret -= 12 * 60;
    }

    return ret;
}

byte TimeStamp::getHours() {
    return _hours;
}

byte TimeStamp::getDate() {
    return _date;
}

byte TimeStamp::getDayOfWeek() {
    return _dayOfWeek;
}

byte TimeStamp::getMonth() {
    return _month;
}

byte TimeStamp::getYear() {
    return _year;
}

void TimeStamp::setFrom(MyDCF77 dcf77) {
    _minutes = dcf77.getMinutes();
    _hours = dcf77.getHours();
    _date = dcf77.getDate();
    _dayOfWeek = dcf77.getDayOfWeek();
    _month = dcf77.getMonth();
    _year = dcf77.getYear();
}

void TimeStamp::setFrom(MyRTC rtc) {
    _minutes = rtc.getMinutes();
    _hours = rtc.getHours();
    _date = rtc.getDate();
    _dayOfWeek = rtc.getDayOfWeek();
    _month = rtc.getMonth();
    _year = rtc.getYear();
}

void TimeStamp::set(byte minutes, byte hours, byte date, byte dayOfWeek, byte month, byte year) {
    _minutes = minutes;
    _hours = hours;
    _date = date;
    _dayOfWeek = dayOfWeek;
    _month = month;
    _year = year;
}

/**
 * Die Minuten erhoehen.
 */
void TimeStamp::incMinutes() {
    _minutes++;
    if (_minutes > 59) {
        _minutes = 0;
    }
}

/**
 * Die Stunden erhoehen.
 */
void TimeStamp::incHours() {
    _hours++;
    if (_hours > 23) {
        _hours = 0;
    }
}

/**
 * Die Zeit als String bekommen
 */
char* TimeStamp::asString() {
    _cDateTime[0] = 0;
    char temp[5];

    // build the string...
    if (_hours < 10) {
        sprintf(temp, "0%d:", _hours);
        strncat(_cDateTime, temp, strlen(temp));
    } else {
        sprintf(temp, "%d:", _hours);
        strncat(_cDateTime, temp, strlen(temp));
    }

    if (_minutes < 10) {
        sprintf(temp, "0%d ", _minutes);
        strncat(_cDateTime, temp, strlen(temp));
    } else {
        sprintf(temp, "%d ", _minutes);
        strncat(_cDateTime, temp, strlen(temp));
    }

    if (_date < 10) {
        sprintf(temp, "0%d.", _date);
        strncat(_cDateTime, temp, strlen(temp));
    } else {
        sprintf(temp, "%d.", _date);
        strncat(_cDateTime, temp, strlen(temp));
    }

    if (_month < 10) {
        sprintf(temp, "0%d.", _month);
        strncat(_cDateTime, temp, strlen(temp));
    } else {
        sprintf(temp, "%d.", _month);
        strncat(_cDateTime, temp, strlen(temp));
    }

    sprintf(temp, "%d", _year);
    strncat(_cDateTime, temp, strlen(temp));

    return _cDateTime;
}
