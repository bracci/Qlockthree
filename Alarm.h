/**
 * Alarm
 * Klasse fuer die Weckfunktion.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  22.1.2013
 * @update   16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#ifndef ALARM_H
#define ALARM_H

#include "Arduino.h"
#include "TimeStamp.h"

class Alarm {
public:
    Alarm(byte speakerPin);

    TimeStamp* getAlarmTime();

    void activate();
    void deactivate();
    void buzz(boolean on);

    byte getShowAlarmTimeTimer();
    void setShowAlarmTimeTimer(byte seconds);
    void decShowAlarmTimeTimer();

    boolean isActive();

private:
    TimeStamp *_alarmTime;
    boolean _isActive;
    byte _showAlarmTimeTimer;
    byte _speakerPin;
};

#endif
