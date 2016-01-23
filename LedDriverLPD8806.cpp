/**
 * LedDriverLPD8806
 * Implementierung auf der Basis von LPD8806-Streifen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  9.2.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 *
 * Verkabelung: Einspeisung oben links, dann schlangenfoermig runter,
 * dann Ecke unten links, oben links, oben rechts, unten rechts.
 *
 * Achtung! LPD8806-Streifen koennen nur in Vielfachen von 2 getrennt werden! Daher bleiben am Rand LEDs uebrig (und dunkel)!
 *
 */
#include "LedDriverLPD8806.h"

// #define DEBUG
#include "Debug.h"

// eigentlich haben wir ja 115 LEDs, aber LPD8806 sind ja immer zweier...
#ifdef MATRIX_XXL
#define NUM_PIXEL 230
#else
#define NUM_PIXEL 130
#endif

/**
 * Initialisierung.
 *
 * @param data Pin, an dem die Data-Line haengt.
 */
LedDriverLPD8806::LedDriverLPD8806(byte dataPin, byte clockPin) {
  _strip = new LPD8806(NUM_PIXEL, dataPin, clockPin);
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
void LedDriverLPD8806::init() {
  setBrightness(50);
  clearData();
  wakeUp();
}

void LedDriverLPD8806::printSignature() {
  Serial.println(F("LPD8806"));
}

/**
 * Den Bildschirm-Puffer auf die LED-Matrix schreiben.
 *
 * @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
 *                  FALSE, wenn es ein Refresh-Aufruf war.
 */
void LedDriverLPD8806::writeScreenBufferToMatrix(word matrix[16], boolean onChange) {
  boolean updateWheelColor = false;

  if (isRainbow() && _transitionCompleted) {
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
      if (((helperSeconds == 0) || _demoTransition) && (mode == STD_MODE_NORMAL) && _transitionCompleted && !evtActive) {
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
            _transitionCounter = FADINGCOUNTERLOAD;
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
    }

    if (_transitionCompleted) {
      for (byte i = 0; i < 11; i++) {
        _matrixOld[i] = 0;
        _matrixNew[i] = matrix[i];
        _matrixOverlay[i] = 0;
      }
    }
    _demoTransition = false;

    if ((_transitionCounter == 0) && !_transitionCompleted) {
      switch (settings.getTransitionMode()) {
        case TRANSITION_MODE_MATRIX:
          _transitionCounter = MATRIXCOUNTERLOAD;
          _transitionCompleted = Transitions::nextMatrixStep(_matrixOld, _matrixNew, _matrixOverlay, matrix);
          break;
        case TRANSITION_MODE_SLIDE:
          _transitionCounter = SLIDINGCOUNTERLOAD;
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
      brightnessOld = map(max(FADINGCOUNTERLOAD / 2, _transitionCounter), FADINGCOUNTERLOAD / 2, FADINGCOUNTERLOAD, 0, _brightnessInPercent);
      brightnessNew = map(_transitionCounter, FADINGCOUNTERLOAD, 0 , 0 , _brightnessInPercent);
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
      color = _strip->Color(_brightnessScaleColor(_brightnessInPercent, getRed()), _brightnessScaleColor(_brightnessInPercent, getBlue()), _brightnessScaleColor(_brightnessInPercent, getGreen()));
      colorNew = _strip->Color(_brightnessScaleColor(brightnessNew, getRed()), _brightnessScaleColor(brightnessNew, getBlue()), _brightnessScaleColor(brightnessNew, getGreen()));
      colorOld = _strip->Color(_brightnessScaleColor(brightnessOld, getRed()), _brightnessScaleColor(brightnessOld, getBlue()), _brightnessScaleColor(brightnessOld, getGreen()));
      colorOverlay1 = 0;
      colorOverlay2 = 0;
    }

    if ( (settings.getTransitionMode() == TRANSITION_MODE_MATRIX) && !_transitionCompleted ) {
      colorOverlay1 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255));
      colorOverlay2 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.5));
      colorOld = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.1));
    }

    /*************
     * WRITE OUT
    **************/
    _clear();

    for (byte y = 0; y < 10; y++) {
      for (byte x = 5; x < 16; x++) {
        word t = 1 << x;
        if ((settings.getTransitionMode() == TRANSITION_MODE_FADE) && ((_matrixOld[y] & t) == t) && ((_matrixNew[y] & t) == t) ) {
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
void LedDriverLPD8806::setBrightness(byte brightnessInPercent) {
  if ((brightnessInPercent != _brightnessInPercent) && _transitionCompleted) {
    _brightnessInPercent = brightnessInPercent;
    _dirty = true;
  }
}

/**
 * Die aktuelle Helligkeit bekommen.
 */
byte LedDriverLPD8806::getBrightness() {
  return _brightnessInPercent;
}

/**
 * Anpassung der Groesse des Bildspeichers.
 *
 * @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
 *                     geschrieben werden?
 */
void LedDriverLPD8806::setLinesToWrite(byte linesToWrite) {
}

/**
 * Das Display ausschalten.
 */
void LedDriverLPD8806::shutDown() {
  _clear();
  _strip->show();
  _displayOn = false;
  _transitionCompleted = true;
}

/**
 * Das Display einschalten.
 */
void LedDriverLPD8806::wakeUp() {
  _displayOn = true;
}

/**
 * Den Dateninhalt des LED-Treibers loeschen.
 */
void LedDriverLPD8806::clearData() {
  _clear();
  _strip->show();
}

/**
 * Einen X/Y-koordinierten Pixel in der Matrix setzen.
 */
void LedDriverLPD8806::_setPixel(byte x, byte y, uint32_t c) {
  _setPixel(x + (y * 11), c);
}

/**
 * Einen Pixel im Streifen setzten (die Eck-LEDs sind am Ende).
 */
void LedDriverLPD8806::_setPixel(byte num, uint32_t c) {
#ifdef MATRIX_XXL
  if (num < 110) {
    if ((num / 11) % 2 == 0) {
      _strip->setPixelColor(num * 2, c);
      _strip->setPixelColor(num * 2 + 1, c);
    } else {
      _strip->setPixelColor(((num / 11) * 22) + 21 - ((num % 11) * 2), c);
      _strip->setPixelColor(((num / 11) * 22) + 20 - ((num % 11) * 2), c);
    }
  } else {
    switch (num) {
      case 110:
        _strip->setPixelColor(222, c);
        _strip->setPixelColor(223, c);
        break;
      case 111:
        _strip->setPixelColor(224, c);
        _strip->setPixelColor(225, c);
        break;
      case 112:
        _strip->setPixelColor(226, c);
        _strip->setPixelColor(227, c);
        break;
      case 113:
        _strip->setPixelColor(220, c);
        _strip->setPixelColor(221, c);
        break;
      case 114:
        _strip->setPixelColor(228, c);
        _strip->setPixelColor(229, c);
        break;
    }
  }
#else
  if (num < 110) {
    if ((num / 11) % 2 == 0) {
      _strip->setPixelColor(num + (num / 11), c);
    } else {
      _strip->setPixelColor(((num / 11) * 12) + 11 - (num % 11), c);
    }
  } else {
    switch (num) {
      case 110:
        _strip->setPixelColor(111 + 11, c);
        break;
      case 111:
        _strip->setPixelColor(112 + 12, c);
        break;
      case 112:
        _strip->setPixelColor(113 + 13, c);
        break;
      case 113:
        _strip->setPixelColor(110 + 10, c);
        break;
      case 114:
        _strip->setPixelColor(114 + 14, c);
        break;
    }
  }
#endif
  delay(1);
}

/**
 * Funktion fuer saubere 'Regenbogen'-Farben.
 * Kopiert aus den Adafruit-Beispielen (strand).
 */
uint32_t LedDriverLPD8806::_wheel(byte brightness, byte wheelPos) {
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
byte LedDriverLPD8806::_brightnessScaleColor(byte brightness, byte colorPart) {
  return map(brightness, 0, 100, 0, colorPart / 2); // LPD8806 kann nur 7 bit Farben! (also 0..127, nicht 0..255)
}

/**
 * Streifen loeschen.
 */
void LedDriverLPD8806::_clear() {
  for (byte i = 0; i < NUM_PIXEL; i++) {
    _strip->setPixelColor(i, 0);
  }
}
