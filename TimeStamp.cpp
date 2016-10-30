/**
 * TimeStamp
 * Klasse fuer die Kapselung eines Zeitstempels. Dadurch lassen sich
 * Zeiten aus der Echtzeituhr und von dem DCF77-Empfaenger
 * leichter verarbeiten und vergleichen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.7a
 * @created  2.3.2011
 * @updated  29.03.2016 (ergänzt durch A. Mueller)
 *
 * Versionshistorie:
 * V 1.1:  - Fehler in toString() behoben.
 * V 1.2:  - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
 * V 1.3:  - neuer Konstruktor, neue Methoden.
 * V 1.4:  - getMinutesOf12HoursDay eingefuehrt.
 * V 1.5:  - Optimierung hinsichtlich Speicherbedarf.
 * V 1.6:  - Verbessertes Debugging & leeren Konstruktor entfernt.
 * V 1.7:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 * V 1.7a: - Funktion getMinutesOfCentury() hinzugefügt
 */
#include "TimeStamp.h"

// #define DEBUG
#include "Debug.h"

TimeStamp::TimeStamp(byte minutes, byte hours, byte date, byte dayOfWeek, byte month, byte year) {
    set(minutes, hours, date, dayOfWeek, month, year);
}

byte TimeStamp::getMinutes() {
    return _minutes;
}

int TimeStamp::getMinutesOfDay() {
    return _minutes + 60 * _hours;
}

/**
 * Minuten des Tages ohne die Beruecksichtigung von 12/24 Stunden
 * (fuer den Wecker)...
 */
int TimeStamp::getMinutesOf12HoursDay(int minutesDiff) {
    int ret = _minutes + 60 * _hours + minutesDiff;

    while (ret > 12 * 60) {
        ret -= 12 * 60;
    }

    return ret;
}

unsigned long TimeStamp::getMinutesOfCentury() {
    /* Funktion gibt NICHT die Minuten des aktuellen Jahrhunderts aus,
     * sondern ermöglicht nur eine eindeutige Zuordnung. Das ist für
     * die DCF-Auswertung nötig.
     * Die Funktion reicht nicht aus, um Zeitvergleiche über eine 
     * Tagesgrenze hinaus durchzuführen (außer DCF-Auswertung).
     */
//    return ( ((( _year * 12 + _month ) * 31 + _date) * 24 + _hours) * 60 + _minutes );

    /* Exakte Berechnung der Minuten des aktuellen Jahrhunderts.
     * Diese Funktion braucht mehr Speicher, ist dafür aber auch für 
     * Zeitvergleiche über einen Tag hinaus geeignet, z.B. für den Countdown.
     * Alternativ kann auch die obige Funktion für den Countdown verwendet werden,
     * dann muss aber die Prüfung während eines Countdowns entfallen.
     */
    unsigned long retVal = 0;
    if (_year) retVal += ( (_year + 3) / 4 + _year * 365);
    switch (_month) {
        case 12: retVal += 30;
        case 11: retVal += 31;
        case 10: retVal += 30;
        case 9: retVal += 31;
        case 8: retVal += 31;
        case 7: retVal += 30;
        case 6: retVal += 31;
        case 5: retVal += 30;
        case 4: retVal += 31;
        case 3: retVal += 28;
                if (_year / 4) {
                    retVal++;
                }
        case 2: retVal += 31;
    }
    retVal = ((retVal + _date - 1) * 24 + _hours) * 60 + _minutes;
    return retVal;
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

void TimeStamp::setMinutes(byte minutes) {
    _minutes = minutes;
}

void TimeStamp::setHours(byte hours) {
    _hours = hours;
}

void TimeStamp::setDayOfWeek(byte dayOfWeek) {
    _dayOfWeek = dayOfWeek;
}

void TimeStamp::setDate(byte date) {
    _date = date;
    CheckDateValidity();
}

void TimeStamp::setMonth(byte month) {
    _month = month;
    CheckDateValidity();
}

void TimeStamp::setYear(byte year) {
    _year = year;
    CheckDateValidity();
}

void TimeStamp::set(byte minutes, byte hours, byte date, byte dayOfWeek, byte month, byte year) {
    _minutes = minutes;
    _hours = hours;
    _date = date;
    _dayOfWeek = dayOfWeek;
    _month = month;
    _year = year;
}

void TimeStamp::set(TimeStamp* timeStamp) {
    _minutes = timeStamp->_minutes;
    _hours = timeStamp->_hours;
    _date = timeStamp->_date;
    _dayOfWeek = timeStamp->_dayOfWeek;
    _month = timeStamp->_month;
    _year = timeStamp->_year;
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
 * Die Minuten um 5 Min erhoehen.
 */
void TimeStamp::incFiveMinutes() {
  for(byte i = 0; i < 5; i++){
    incMinutes();
  }
}

/**
 * Die Minuten dekrementieren.
 */
void TimeStamp::decMinutes() {
  if (_minutes == 0) {
    _minutes = 59;
  }
  else
  {
    _minutes--;
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
 * Die Stunden dekrementieren.
 */
void TimeStamp::decHours() {
  if (_hours == 0) {
    _hours = 23;
  }
  else {
    _hours--;
  }
}

/**
 * Die Stunden erhoehen.
 */
void TimeStamp::incYear(byte addYear) {
    setYear(_year + addYear);
}

/**
 * Die Stunden erhoehen.
 */
void TimeStamp::incMonth(byte addMonth) {
    setMonth(_month + addMonth);
}

/**
 * Die Stunden erhoehen.
 */
void TimeStamp::incDate(byte addDate) {
    setDate(_date + addDate);
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

void TimeStamp::CheckDateValidity() {
    if (_month < 1)
        _month = 1;
    while (_month > 12) {
        _month -= 12;
    }

    if (_date < 1)
        _date = 1;
    while (_date > getDaysOfMonth(_month, _year)) {
        _date -= getDaysOfMonth(_month, _year);
    }
    
    _year %= 100;
    
    CalculateAndSetDayOfWeek();
}

void TimeStamp::CalculateAndSetDayOfWeek() {
    // (Adopted) alghorithm by Schwerdtfeger
    // This alghorithm is only valid from 1st March 2000 to 31st December 2099
    byte g = _year;
    if (_month < 3) g--;
    byte e;
    switch (_month) {
        default: e = 0; break;
        case 2: 
        case 6:  e = 3; break;
        case 3:
        case 11: e = 2; break;
        case 4: 
        case 7:  e = 5; break;
        case 8:  e = 1; break;
        case 9: 
        case 12: e = 4; break;
        case 10: e = 6; break;
    }
    byte w = (_date + e + g + g/4) % 7;
    if (!w) w = 7;
    setDayOfWeek(w);
}

byte TimeStamp::getDaysOfMonth(byte month, byte year) {
    // Only valid for years from 2000 to 2099
    switch (month) {
        default: return 0;
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: 
            return 31;
            break;
        case 2:
            if (year % 4)
                return 28;
            else
                return 29;
            break;
       case 4:
       case 6:
       case 9:
       case 11:
            return 30;
            break;
    }
}
