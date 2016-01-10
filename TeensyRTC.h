/**
 * TeensyRTC
 * Emuliert die MyRTC mit Teensys interner RTC
 *
 * @autor    Arne Schwabe
 * @version  0.1
 * @created  10.1.2016
 * @updated  10.1.2016
 *
 * Versionshistorie:
 * V0.1 First version
 */
#ifndef TEENSYRTC_H
#define TEENSYRTC_H

#include "MyRTC.h"
#include "Time.h"

inline time_t getTeensy3Time()
{
    return Teensy3Clock.get();
}

class TeensyRTC : public MyRTC {
public:
    TeensyRTC(byte statusLedPin) :  MyRTC::MyRTC(0x0, statusLedPin) {
        setSyncProvider(getTeensy3Time);
    };

    void readTime() {
        time_t t=now();
        _seconds = second(t);
        _minutes = minute(t);
        _hours = hour(t);
        _date = day(t);
        _month = month(t);
        _year = year(t);
        _dayOfWeek = weekday(t);
    };
    void writeTime() { setTime(_hours, _minutes, _seconds, _date, _month, _year);}
};

#endif
