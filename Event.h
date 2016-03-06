/**
   Event.h
   Klasse für ein jährliches Event

   @mc       Arduino/UNO
   @autor    Manuel Bracher / manuel.bracher@gmail.com
   @version  1.0
   @created  02.01.15

   Versionshistorie:
   V 1.0:  - Erstellt.
*/

#ifndef EVENT_H
#define EVENT_H

#include "Effects.h"
#include "Colors.h"

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
    const char* getText();
    Effects::eEffects getEffect();
    eColors getColor();
  private:
    byte _month;
    byte _date;
    const char* _txt;
    Effects::eEffects _effect;
    eColors _color;

};

#endif //EVENT_H
