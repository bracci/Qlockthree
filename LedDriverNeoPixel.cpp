/**
   LedDriverNeoPixel
   Implementierung auf der Basis von WS2812B-Streifen wie sie die Adafruit-Neo-Pixel verwenden.

   Es lohnt sich in jedem Fall, den UeberGuide von Adafruit zu lesen:
   https://learn.adafruit.com/adafruit-neopixel-uberguide/overview

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.2
   @created  5.1.2015
   @updated  16.2.2015

   Versionshistorie:
   V 1.0:  - Erstellt.
   V 1.1:  - Getter fuer Helligkeit nachgezogen.
   V 1.2:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.

   Verkabelung: Einspeisung oben links, dann schlangenfoermig runter,
   dann Ecke unten links, oben links, oben rechts, unten rechts.

*/
#include "LedDriverNeoPixel.h"

// #define DEBUG
#include "Debug.h"

#ifdef MATRIX_XXL
#define NUM_PIXEL 225
#else
#define NUM_PIXEL 115
#endif

/**
   Initialisierung.

   @param data Pin, an dem die Data-Line haengt.
*/
LedDriverNeoPixel::LedDriverNeoPixel(byte dataPin) {
  _strip = new Adafruit_NeoPixel(NUM_PIXEL, dataPin, NEO_GRB + NEO_KHZ800);
  _strip->begin();
  _wheelPos = 0;
  _transitionCounter = 0;
  _transitionCompleted = true;
  _lastColorUpdate = millis();
  _dirty = false;
  _demoTransition = false;
}

/**
   init() wird im Hauptprogramm in init() aufgerufen.
   Hier sollten die LED-Treiber in eine definierten
   Ausgangszustand gebracht werden.
*/
void LedDriverNeoPixel::init() {
  setBrightness(50);
  clearData();
  wakeUp();
}

void LedDriverNeoPixel::printSignature() {
  DEBUG_PRINTLN(F("NeoPixel - WS2812B"));
}

/**
   Den Bildschirm-Puffer auf die LED-Matrix schreiben.

   @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
                    FALSE, wenn es ein Refresh-Aufruf war.
*/
void LedDriverNeoPixel::writeScreenBufferToMatrix(word matrix[16], boolean onChange, eColors a_color) {
  boolean updateWheelColor = false;

  byte wheelPosIncrement = 0;
  
  if ((a_color == color_rgb_continuous) && _transitionCompleted) {
    if ((millis() - _lastColorUpdate) > ((1 + (10-settings.getColorChangeRate())) * 100)) {
      updateWheelColor = true;
      _lastColorUpdate = millis();
      wheelPosIncrement = 2;
    }
  }

  if (a_color == color_rgb_step) {
    if (!(rtc.getMinutes() % 5) && (helperSeconds == 0) &&  onChange) {
      updateWheelColor = true;
      _lastColorUpdate = millis();
      wheelPosIncrement = 200;
    }
  }

  if (!_transitionCompleted && (_transitionCounter > 0)) {
    _transitionCounter--;
  }
  else
  {
    _transitionCounter = 0;
  }

  if (onChange || _dirty || _demoTransition || updateWheelColor || (((_transitionCounter == 0) || (Settings::TRANSITION_MODE_FADE == settings.getTransitionMode())) && !_transitionCompleted)) {
    uint32_t color = 0;
    uint32_t colorNew = 0;
    uint32_t colorOld = 0;
    uint32_t colorOverlay1 = 0;
    uint32_t colorOverlay2 = 0;
    byte brightnessOld = 0;
    byte brightnessNew = 0;

    _dirty = false;

    if (mode != STD_MODE_NORMAL) {
      _transitionCompleted = true;
      _demoTransition = false;
    }

    /*************
       MATRIX
    **************/

    if (onChange || _demoTransition) {
      if (((helperSeconds == 0) || _demoTransition) && (mode == STD_MODE_NORMAL) && _transitionCompleted && !evtActive) {
        switch (settings.getTransitionMode()) {
          case Settings::TRANSITION_MODE_FADE:
            for (byte i = 0; i < 11; i++) {
              _matrixOld[i] = _matrixNew[i];
              if (_demoTransition) {
                _matrixNew[i] = 0;
              }
              else {
                _matrixNew[i] = matrix[i];
              }
              _matrixOverlay[i] = 0;
            }
            _transitionCompleted = false;
            _transitionCounter = FADINGCOUNTERLOAD;
            break;
          case Settings::TRANSITION_MODE_MATRIX:
          case Settings::TRANSITION_MODE_SLIDE:
            if (((rtc.getMinutes() % 5) == 0) || _demoTransition) {
              Transitions::resetTransition();
              for (byte i = 0; i < 11; i++) {
                _matrixOld[i] = 0;
                _matrixOverlay[i] = 0;
              }
              _transitionCompleted = false;
            }
            break;
          case Settings::TRANSITION_MODE_NORMAL:
            if (_demoTransition) {
              for (byte i = 0; i < 11; i++) {
                _matrixNew[i] = 0;
              }
              _transitionCompleted = false;
              _transitionCounter = NORMALCOUNTERLOAD;
            }
            break;
          default:
            ;
        }
      }
      if (_transitionCompleted) {
        for (byte i = 0; i < 11; i++) {
          _matrixOld[i] = 0;
          _matrixNew[i] = matrix[i];
          _matrixOverlay[i] = 0;
        }
      }
    }

    _demoTransition = false;

    if ((_transitionCounter == 0) && !_transitionCompleted) {
      switch (settings.getTransitionMode()) {
        case Settings::TRANSITION_MODE_MATRIX:
          _transitionCounter = MATRIXCOUNTERLOAD;
          _transitionCompleted = Transitions::nextMatrixStep(_matrixOld, _matrixNew, _matrixOverlay, matrix);
          break;
        case Settings::TRANSITION_MODE_SLIDE:
          _transitionCounter = SLIDINGCOUNTERLOAD;
          _transitionCompleted = Transitions::nextSlideStep(_matrixNew, matrix);
          break;
        case Settings::TRANSITION_MODE_NORMAL:
          _transitionCompleted = true;
          break;
        default:
          ;
      }
    }

    /*************
       BRIGHTNESS
    **************/

    if ((Settings::TRANSITION_MODE_FADE == settings.getTransitionMode()) && !_transitionCompleted) {
      brightnessOld = map(_transitionCounter, 0, FADINGCOUNTERLOAD, 0, _brightnessInPercent);
      brightnessNew = map(_transitionCounter, FADINGCOUNTERLOAD, 0 , 0 , _brightnessInPercent);
      if (_transitionCounter == 0) {
        _transitionCompleted = true;
      }
    }
    else {
      brightnessNew = _brightnessInPercent;
    }

    /*************
       COLOR
    **************/
    if (a_color <= color_single_max)
    {
      color = _strip->Color(_brightnessScaleColor(_brightnessInPercent, pgm_read_byte_near(&defaultColors[a_color].red)), _brightnessScaleColor(_brightnessInPercent, pgm_read_byte_near(&defaultColors[a_color].green)), _brightnessScaleColor(_brightnessInPercent, pgm_read_byte_near(&defaultColors[a_color].blue)));
      colorNew = _strip->Color(_brightnessScaleColor(brightnessNew, pgm_read_byte_near(&defaultColors[a_color].red)), _brightnessScaleColor(brightnessNew, pgm_read_byte_near(&defaultColors[a_color].green)), _brightnessScaleColor(brightnessNew, pgm_read_byte_near(&defaultColors[a_color].blue)));
      colorOld = _strip->Color(_brightnessScaleColor(brightnessOld, pgm_read_byte_near(&defaultColors[a_color].red)), _brightnessScaleColor(brightnessOld, pgm_read_byte_near(&defaultColors[a_color].green)), _brightnessScaleColor(brightnessOld, pgm_read_byte_near(&defaultColors[a_color].blue)));
    }
    else if ((a_color == color_rgb_continuous || a_color == color_rgb_step)) {
      if (updateWheelColor) {
        _wheelPos += wheelPosIncrement;
      }
      color = _wheel(_brightnessInPercent, _wheelPos);
      colorNew = _wheel(brightnessNew, _wheelPos);
      colorOld = _wheel(brightnessOld, _wheelPos);
    }

    if ( (settings.getTransitionMode() == Settings::TRANSITION_MODE_MATRIX) && !_transitionCompleted ) {
      colorOverlay1 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255), _brightnessScaleColor(_brightnessInPercent, 0));
      colorOverlay2 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.5), _brightnessScaleColor(_brightnessInPercent, 0));
      colorOld = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.1), _brightnessScaleColor(_brightnessInPercent, 0));
    }

    /*************
       WRITE OUT
    **************/

    _strip->clear();

    for (byte y = 0; y < 10; y++) {
      for (byte x = 5; x < 16; x++) {
        word t = 1 << x;
        if ((settings.getTransitionMode() == Settings::TRANSITION_MODE_FADE) && ((_matrixOld[y] & t) == t) && ((_matrixNew[y] & t) == t)) {
          _setPixel(15 - x, y, color);
        }
        else {
          if ((_matrixOverlay[y] & t) == t) {
            _setPixel(15 - x, y, colorOverlay1);
          }
          else if ((_matrixOverlay[y + 1] & t) == t) {
            _setPixel(15 - x, y, colorOverlay2);
          }
          else if ((_matrixOld[y] & t) == t) {
            _setPixel(15 - x, y, colorOld);
          }
          else if ((_matrixNew[y] & t) == t) {
            _setPixel(15 - x, y, colorNew);
          }
        }
      }
    }

    // wir muessen die Eck-LEDs und die Alarm-LED umsetzen...
    byte cornerLedCount[] = {1, 0, 3, 2, 4};
    for ( byte i = 0; i < 5; i++) {
      if ((settings.getTransitionMode() == Settings::TRANSITION_MODE_FADE) && ((_matrixOld[cornerLedCount[i]] & _matrixNew[cornerLedCount[i]] & 0b0000000000011111) > 0) ) {
        _setPixel(110 + i, color);
      }
      else {
        if (((_matrixOld[cornerLedCount[i]] & 0b0000000000010000) > 0) ) {
          _setPixel(110 + i, colorOld);
        }
        else if (((_matrixNew[cornerLedCount[i]] & 0b0000000000010000) > 0) ) {
          _setPixel(110 + i, colorNew);
        }
      }
    }
    _strip->show();
  }
}

/**
   Die Helligkeit des Displays anpassen.

   @param brightnessInPercent Die Helligkeit.
*/
void LedDriverNeoPixel::setBrightness(byte brightnessInPercent) {
  if ((brightnessInPercent != _brightnessInPercent) && _transitionCompleted) {
    _brightnessInPercent = brightnessInPercent;
    _dirty = true;
  }
}

/**
   Die aktuelle Helligkeit bekommen.
*/
byte LedDriverNeoPixel::getBrightness() {
  return _brightnessInPercent;
}

/**
   Anpassung der Groesse des Bildspeichers.

   @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
                       geschrieben werden?
*/
void LedDriverNeoPixel::setLinesToWrite(byte linesToWrite) {
}

/**
   Das Display einschalten.
*/
void LedDriverNeoPixel::wakeUp() {
}

/**
   Das Display ausschalten.
*/
void LedDriverNeoPixel::shutDown() {
  _strip->clear();
  _strip->show();
  _transitionCompleted = true;
}

/**
   Den Dateninhalt des LED-Treibers loeschen.
*/
void LedDriverNeoPixel::clearData() {
  _strip->clear();
  _strip->show();
}

/**
   Einen X/Y-koordinierten Pixel in der Matrix setzen.
*/
void LedDriverNeoPixel::_setPixel(byte x, byte y, uint32_t c) {
  _setPixel(x + (y * 11), c);
}

/**
   Einen Pixel im Streifen setzten (die Eck-LEDs sind am Ende).
*/
void LedDriverNeoPixel::_setPixel(byte num, uint32_t c) {
  #ifdef MATRIX_XXL
  if (num < 110) {
    if ((num / 11) % 2 == 0) {
      _strip->setPixelColor(num*2, c);
      _strip->setPixelColor(num*2+1, c);
    } else {
      _strip->setPixelColor(((num / 11) * 22) + 21 - (num % 11)*2, c);
      _strip->setPixelColor(((num / 11) * 22) + 21 - ((num % 11)*2 + 1), c);
    }
  } else {
    switch (num) {
      case 110:
        _strip->setPixelColor(221, c);
        break;
      case 111:
        _strip->setPixelColor(222, c);
        break;
      case 112:
        _strip->setPixelColor(223, c);
        break;
      case 113:
        _strip->setPixelColor(220, c);
        break;
      case 114:                         // die Alarm-LED
        _strip->setPixelColor(224, c);
        break;
      default:
        ;
    }
  }
  #else
  if (num < 110) {
    if ((num / 11) % 2 == 0) {
      _strip->setPixelColor(num, c);
    } else {
      _strip->setPixelColor(((num / 11) * 11) + 10 - (num % 11), c);
    }
  } else {
    switch (num) {
      case 110:
        _strip->setPixelColor(111, c);
        break;
      case 111:
        _strip->setPixelColor(112, c);
        break;
      case 112:
        _strip->setPixelColor(113, c);
        break;
      case 113:
        _strip->setPixelColor(110, c);
        break;
      case 114:                         // die Alarm-LED
        _strip->setPixelColor(114, c);
        break;
      default:
        ;
    }
  }
  #endif
}

/**
   Funktion fuer saubere 'Regenbogen'-Farben.
   Kopiert aus den Adafruit-Beispielen (strand).
*/
uint32_t LedDriverNeoPixel::_wheel(byte brightness, byte wheelPos) {
  if (wheelPos < 85) {
    return _strip->Color(_brightnessScaleColor(brightness, wheelPos * 3), _brightnessScaleColor(brightness, 255 - wheelPos * 3), _brightnessScaleColor(brightness, 0));
  } else if (wheelPos < 170) {
    wheelPos -= 85;
    return _strip->Color(_brightnessScaleColor(brightness, 255 - wheelPos * 3), _brightnessScaleColor(brightness, 0), _brightnessScaleColor(brightness, wheelPos * 3));
  } else {
    wheelPos -= 170;
    return _strip->Color(_brightnessScaleColor(brightness, 0), _brightnessScaleColor(brightness, wheelPos * 3), _brightnessScaleColor(brightness, 255 - wheelPos * 3));
  }
}

/**
   Hilfsfunktion fuer das Skalieren der Farben.
*/
byte LedDriverNeoPixel::_brightnessScaleColor(byte brightness, byte colorPart) {
  return map(brightness, 0, 100, 0, colorPart);
}

