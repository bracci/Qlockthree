/**
 * Event.h
 * Klasse für ein jährliches Event
 *
 * @mc       Arduino/UNO
 * @autor    Manuel Bracher / manuel.bracher@gmail.com
 * @version  1.0
 * @created  02.01.15
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */

#ifndef EVENT_H
#define EVENT_H

#include "Effects.h"
#include "Configuration.h"
#include "Colors.h"

#if defined(LED_DRIVER_LPD8806) || defined(LED_DRIVER_NEOPIXEL)
#define DURATION_BM   5
#define TICKER_SPEED  8
#else
#define DURATION_BM   15
#define TICKER_SPEED  6
#endif

#define DURATION_ANI_BM  (DURATION_BM/5)


class Event {
  public:
    Event(byte month,
          byte date,
          const char* txt,
          Effects::eEffects effect,
          eColors color);

    void show();
    byte getMonth();
    byte getDate();

  private:
    byte _month;
    byte _date;
    const char* _txt;
    Effects::eEffects _effect;
    eColors _color;
};

#endif //EVENT_H
