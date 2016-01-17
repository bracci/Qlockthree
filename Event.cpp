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
    Effects::showTickerString(_txt, TICKER_SPEED);
  if (_effect < Effects::BITMAP_MIN) {
    switch (_effect)
    {
      case Effects::NO_EFFECT:
        break;
      case Effects::EFFECT_FIREWORK:
        Effects::showFireWork(5);
        Effects::showFireWork(2);
        Effects::showFireWork(8);
        break;
      case Effects::EFFECT_HEART:
        Effects::showHeart(DURATION_ANI_BM);
        break;
      case Effects::EFFECT_CANDLE:
        Effects::showCandle();
        break;
      case Effects::EFFECT_LOVEU:
        Effects::showLoveU();
        break;
      default:
        ;
    }
  }
  if ((_effect >= Effects::BITMAP_MIN) && (_effect < Effects::ANI_BITMAP_MIN)) {
    Effects::showBitmap(_effect, DURATION_BM);
  }
  if (_effect >= Effects::ANI_BITMAP_MIN) {
    Effects::showAnimatedBitmap(_effect, DURATION_ANI_BM);
  }
}

byte Event::getMonth() {
  return _month;
}

byte Event::getDate() {
  return _date;
}
