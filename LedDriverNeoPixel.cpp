/**
 * LedDriverNeoPixel
 * Implementierung auf der Basis von WS2812B-Streifen wie sie die Adafruit-Neo-Pixel verwenden.
 *
 * Es lohnt sich in jedem Fall, den UeberGuide von Adafruit zu lesen:
 * https://learn.adafruit.com/adafruit-neopixel-uberguide/overview
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.2
 * @created  5.1.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Getter fuer Helligkeit nachgezogen.
 * V 1.2:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 *
 * Verkabelung: Einspeisung oben links, dann schlangenfoermig runter,
 * dann Ecke unten links, oben links, oben rechts, unten rechts.
 *
 */
#include "LedDriverNeoPixel.h"

// #define DEBUG
#include "Debug.h"

#define NUM_PIXEL 114

/**
 * Initialisierung.
 *
 * @param data Pin, an dem die Data-Line haengt.
 */
LedDriverNeoPixel::LedDriverNeoPixel(byte dataPin) {
  _strip = new Adafruit_NeoPixel(NUM_PIXEL, dataPin, NEO_GRB + NEO_KHZ800);
  _strip->begin();
  _wheelPos = 0;
  _transitionCounter = 0;
  _transitionCompleted = true;
  _lastColorUpdate = millis();
  _displayOn = false;
  _dirty = false;
  _demoTransition = false;
  setColor(250, 255, 200);
}

/**
 * init() wird im Hauptprogramm in init() aufgerufen.
 * Hier sollten die LED-Treiber in eine definierten
 * Ausgangszustand gebracht werden.
 */
void LedDriverNeoPixel::init() {
  setBrightness(50);
  clearData();
  wakeUp();
}

void LedDriverNeoPixel::printSignature() {
  Serial.println(F("NeoPixel - WS2812B"));
}

/**
 * Den Bildschirm-Puffer auf die LED-Matrix schreiben.
 *
 * @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
 *                  FALSE, wenn es ein Refresh-Aufruf war.
 */
void LedDriverNeoPixel::writeScreenBufferToMatrix(word matrix[16], boolean onChange) {
  boolean updateWheelColor = false;

  if (isRainbow()&&_transitionCompleted) {
    if ((millis() - _lastColorUpdate) > 300) {
      updateWheelColor = true;
      _lastColorUpdate = millis();
    }
  }

  if (!_transitionCompleted && (_transitionCounter > 0)) {
    _transitionCounter--;
  }
  else
  {
    _transitionCounter = 0;
  }

  if ((onChange || _dirty || _demoTransition || updateWheelColor || (((_transitionCounter == 0) || (TRANSITION_MODE_FADE == settings.getTransitionMode())) && !_transitionCompleted)) && _displayOn) {
    uint32_t color;
    uint32_t colorNew;
    uint32_t colorOld;
    uint32_t colorOverlay1;
    uint32_t colorOverlay2;
    byte brightnessOld;
    byte brightnessNew;

    _dirty = false;

    if (mode != STD_MODE_NORMAL) {
      _transitionCompleted = true;
      _demoTransition = false;
    }

    /*************
     * MATRIX
    **************/

    if (onChange || _demoTransition) {
      if (((helperSeconds == 0) || _demoTransition) && (mode == STD_MODE_NORMAL) && _transitionCompleted) {
        switch (settings.getTransitionMode()) {
          case TRANSITION_MODE_FADE:
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
            _transitionCounter = FADINGCOUNTERLOAD * 2;
            break;
          case TRANSITION_MODE_MATRIX:
          case TRANSITION_MODE_SLIDE:
            if (((rtc.getMinutes() % 5) == 0) || _demoTransition) {
              Transitions::resetTransition();
              for (byte i = 0; i < 11; i++) {
                _matrixOld[i] = 0;
                _matrixOverlay[i] = 0;
              }
              _transitionCompleted = false;
            }
            break;
          case TRANSITION_MODE_NORMAL:
            if (_demoTransition) {
              for (byte i = 0; i < 11; i++) {
                _matrixNew[i] = 0;
              }
              _transitionCompleted = false;
              _transitionCounter = NORMALCOUNTERLOAD;
            }
            break;
          default:
            break;
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
        case TRANSITION_MODE_MATRIX:
          _transitionCounter = MATRIXCOUNTERLOAD * 2;
          _transitionCompleted = Transitions::nextMatrixStep(_matrixOld, _matrixNew, _matrixOverlay, matrix);
          break;
        case TRANSITION_MODE_SLIDE:
          _transitionCounter = SLIDINGCOUNTERLOAD * 2;
          _transitionCompleted = Transitions::nextSlideStep(_matrixNew, matrix);
          break;
        case TRANSITION_MODE_NORMAL:
          _transitionCompleted = true;
          break;
      }
    }

    /*************
     * BRIGHTNESS
    **************/

    if ((TRANSITION_MODE_FADE == settings.getTransitionMode()) && !_transitionCompleted) {
      brightnessOld = map(_transitionCounter, 0, FADINGCOUNTERLOAD * 2, 0, _brightnessInPercent);
      brightnessNew = map(_transitionCounter, FADINGCOUNTERLOAD * 2, 0 , 0 , _brightnessInPercent);
      if (_transitionCounter == 0) {
        _transitionCompleted = true;
      }
    }
    else {
      brightnessOld = 0;
      brightnessNew = _brightnessInPercent;
    }

    /*************
     * COLOR
    **************/

    if (isRainbow()) {
      if (updateWheelColor) {
        if (_wheelPos >= 254) {
          _wheelPos = 0;
        }
        else {
          _wheelPos += 2;
        }
      }
      color = _wheel(_brightnessInPercent, _wheelPos);
      colorNew = _wheel(brightnessNew, _wheelPos);
      colorOld = _wheel(brightnessOld, _wheelPos);
      colorOverlay1 = 0;
      colorOverlay2 = 0;
    }
    else {
      color = _strip->Color(_brightnessScaleColor(_brightnessInPercent, getRed()), _brightnessScaleColor(_brightnessInPercent, getGreen()), _brightnessScaleColor(_brightnessInPercent, getBlue()));
      colorNew = _strip->Color(_brightnessScaleColor(brightnessNew, getRed()), _brightnessScaleColor(brightnessNew, getGreen()), _brightnessScaleColor(brightnessNew, getBlue()));
      colorOld = _strip->Color(_brightnessScaleColor(brightnessOld, getRed()), _brightnessScaleColor(brightnessOld, getGreen()), _brightnessScaleColor(brightnessOld, getBlue()));
      colorOverlay1 = 0;
      colorOverlay2 = 0;
    }

    if ( (settings.getTransitionMode() == TRANSITION_MODE_MATRIX) && !_transitionCompleted ) {
      colorOverlay1 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255), _brightnessScaleColor(_brightnessInPercent, 0));
      colorOverlay2 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.5), _brightnessScaleColor(_brightnessInPercent, 0));
      colorOld = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.1), _brightnessScaleColor(_brightnessInPercent, 0));
    }

    /*************
     * WRITE OUT
    **************/

    _strip->clear();

    for (byte y = 0; y < 10; y++) {
      for (byte x = 5; x < 16; x++) {
        word t = 1 << x;
        if ((settings.getTransitionMode() == TRANSITION_MODE_FADE) && ((_matrixOld[y] & t) == t) && ((_matrixNew[y] & t) == t)) {
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
      if ((settings.getTransitionMode() == TRANSITION_MODE_FADE) && ((_matrixOld[cornerLedCount[i]] & _matrixNew[cornerLedCount[i]] & 0b0000000000011111) > 0) ) {
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
 * Die Helligkeit des Displays anpassen.
 *
 * @param brightnessInPercent Die Helligkeit.
 */
void LedDriverNeoPixel::setBrightness(byte brightnessInPercent) {
  if ((brightnessInPercent != _brightnessInPercent) && _transitionCompleted) {
    _brightnessInPercent = brightnessInPercent;
    _dirty = true;
  }
}

/**
 * Die aktuelle Helligkeit bekommen.
 */
byte LedDriverNeoPixel::getBrightness() {
  return _brightnessInPercent;
}

/**
 * Anpassung der Groesse des Bildspeichers.
 *
 * @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
 *                     geschrieben werden?
 */
void LedDriverNeoPixel::setLinesToWrite(byte linesToWrite) {
}

/**
 * Das Display einschalten.
 */
void LedDriverNeoPixel::wakeUp() {
  _displayOn = true;
}

/**
 * Das Display ausschalten.
 */
void LedDriverNeoPixel::shutDown() {
  _strip->clear();
  _strip->show();
  _displayOn = false;
  _transitionCompleted = true;
}

/**
 * Den Dateninhalt des LED-Treibers loeschen.
 */
void LedDriverNeoPixel::clearData() {
  _strip->clear();
  _strip->show();
}

/**
 * Einen X/Y-koordinierten Pixel in der Matrix setzen.
 */
void LedDriverNeoPixel::_setPixel(byte x, byte y, uint32_t c) {
  _setPixel(x + (y * 11), c);
}

/**
 * Einen Pixel im Streifen setzten (die Eck-LEDs sind am Ende).
 */
void LedDriverNeoPixel::_setPixel(byte num, uint32_t c) {
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
        _strip->setPixelColor(110, c);
        break;
    }
  }
}

/**
 * Funktion fuer saubere 'Regenbogen'-Farben.
 * Kopiert aus den Adafruit-Beispielen (strand).
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
 * Hilfsfunktion fuer das Skalieren der Farben.
 */
byte LedDriverNeoPixel::_brightnessScaleColor(byte brightness, byte colorPart) {
  return map(brightness, 0, 100, 0, colorPart);
}
