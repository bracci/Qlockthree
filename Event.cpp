/**
  Event.cpp
  Klasse für ein jährliches Event

  @mc       Arduino/UNO
  @autor    Manuel Bracher / manuel.bracher@gmail.com
  @version  1.0
  @created  02.01.15

  Versionshistorie:
  V 1.0:  - Erstellt.
*/

#include "Event.h"

// #define DEBUG
#include "Debug.h"

Event::Event(byte month,
             byte date,
             const char* txt,
             Effects::eEffects effect,
             eColors color) :
  _month (month), _date (date),
  _txt (txt), _effect (effect),
  _color(color) {};

byte Event::getMonth() {
  return _month;
}

byte Event::getDate() {
  return _date;
}

const char*  Event::getText() {
  return _txt;
}

Effects::eEffects  Event::getEffect() {
  return _effect;
}

eColors  Event::getColor() {
  return _color;
}

