/**
* Event.cpp
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

#include "Event.h"

// #define DEBUG
#include "Debug.h"

void Event::show() {
  if (_txt != "")
    _effects->showTickerString(_txt, TICKER_SPEED);
  if (_effect < Effects::BITMAP_MIN) {
    switch (_effect)
    {
      case Effects::NO_EFFECT:
        break;
      case Effects::EFFECT_FIREWORK:
        _effects->showFireWork(5);
        _effects->showFireWork(2);
        _effects->showFireWork(8);
        break;
//      case Effects::EFFECT_HEART:
//        _effects->showHeart();
//        break;
      case Effects::EFFECT_CANDLE:
        _effects->showCandle();
        break;
      case Effects::EFFECT_LOVEU:
        _effects->showLoveU();
        break;
      default:
        ;
    }
  }
  if ((_effect >= Effects::BITMAP_MIN) && (_effect < Effects::ANI_BITMAP_MIN)) {
    _effects->showBitmap(_effect, DURATION_BM);
  }
  if (_effect >= Effects::ANI_BITMAP_MIN) {
    _effects->showAnimatedBitmap(_effect);
  }
}

byte Event::getMonth() {
  return _month;
}

byte Event::getDate() {
  return _date;
}
