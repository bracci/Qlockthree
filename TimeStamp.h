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
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "Arduino.h"

class TimeStamp {
public:
    TimeStamp(byte minutes, byte hours, byte date, byte dayOfWeek, byte month, byte year);

    void incMinutes();
    void decMinutes();
    void incFiveMinutes();
    void incHours();
    void decHours();
    void incDate(byte addDate = 1);
    void incMonth(byte addMonth = 1);
    void incYear(byte addYear = 1);

    byte getMinutes();
    int getMinutesOfDay();
    int getMinutesOf12HoursDay(int minutesDiff);
    byte getHours();

    byte getDate();
    byte getDayOfWeek();
    byte getMonth();
    byte getYear();
    unsigned long getMinutesOfCentury();

    void setMinutes(byte minutes);
    void setHours(byte hours);
    void setDate(byte date);
    void setDayOfWeek(byte dayOfWeek);
    void setMonth(byte month);
    void setYear(byte year);

    void set(byte minutes, byte hours, byte date, byte dayOfWeek, byte month, byte year);
    void set(TimeStamp* timeStamp);

    char* asString();

protected:
    byte getDaysOfMonth(byte month, byte year);
    void CalculateAndSetDayOfWeek();
    void CheckDateValidity();

    byte _minutes;
    byte _hours;

    byte _date;
    byte _dayOfWeek;
    byte _month;
    byte _year;

    char _cDateTime[17];
};

#endif
