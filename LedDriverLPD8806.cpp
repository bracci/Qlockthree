/**
   LedDriverLPD8806
   Implementierung auf der Basis von LPD8806-Streifen.

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.1
   @created  9.2.2015
   @updated  16.2.2015

   Versionshistorie:
   V 1.0:  - Erstellt.
   V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.

   Verkabelung: Einspeisung oben links, dann schlangenfoermig runter,
   dann Ecke unten links, oben links, oben rechts, unten rechts.

   Achtung! LPD8806-Streifen koennen nur in Vielfachen von 2 getrennt werden! Daher bleiben am Rand LEDs uebrig (und dunkel)!

*/
#include "LedDriverLPD8806.h"

// #define DEBUG
#include "Debug.h"

// eigentlich haben wir ja 115 LEDs, aber LPD8806 sind ja immer zweier...
#if defined(MATRIX_XXL) || defined(RGBW_LEDS)
#define NUM_PIXEL 115
#else
#define NUM_PIXEL 130
#endif

/**
   Initialisierung.

   @param data Pin, an dem die Data-Line haengt.
*/
LedDriverLPD8806::LedDriverLPD8806(byte dataPin, byte clockPin) {
#ifdef MATRIX_XXL
  #ifdef RGBW_LEDS
    _strip = new LPD8806RGBW_DBL(NUM_PIXEL, dataPin, clockPin);
  #else 
    _strip = new LPD8806DBL(NUM_PIXEL, dataPin, clockPin);
  #endif
#else
  #ifdef RGBW_LEDS
    _strip = new LPD8806RGBW(NUM_PIXEL, dataPin, clockPin);
  #else
    _strip = new LPD8806(NUM_PIXEL, dataPin, clockPin);
  #endif
#endif

  _strip->begin();
  _wheelPos = 0;
  _transitionCounter = 0;
  _transitionCompleted = true;
  _lastColorUpdate = millis();
  _dirty = false;
  _demoTransition = false;
  _lastLEDsOn = 0;
}

/**
   init() wird im Hauptprogramm in init() aufgerufen.
   Hier sollten die LED-Treiber in eine definierten
   Ausgangszustand gebracht werden.
*/
void LedDriverLPD8806::init() {
  setBrightness(50);
  clearData();
  wakeUp();
}

void LedDriverLPD8806::printSignature() {
  DEBUG_PRINTLN(F("LPD8806"));
}

/**
   Den Bildschirm-Puffer auf die LED-Matrix schreiben.

   @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
                    FALSE, wenn es ein Refresh-Aufruf war.
*/
void LedDriverLPD8806::writeScreenBufferToMatrix(word matrix[16], boolean onChange, eColors a_color) {
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
        case Settings::TRANSITION_MODE_MATRIX:
          _transitionCounter = map(_lastLEDsOn, 0, 110, MATRIXCOUNTERLOAD, MATRIXCOUNTERLOAD * 0.4);
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
      color = _strip->Color(_brightnessScaleColor(_brightnessInPercent, pgm_read_byte_near(&defaultColors[a_color].red)), _brightnessScaleColor(_brightnessInPercent, pgm_read_byte_near(&defaultColors[a_color].blue)), _brightnessScaleColor(_brightnessInPercent, pgm_read_byte_near(&defaultColors[a_color].green)));
      colorNew = _strip->Color(_brightnessScaleColor(brightnessNew, pgm_read_byte_near(&defaultColors[a_color].red)), _brightnessScaleColor(brightnessNew, pgm_read_byte_near(&defaultColors[a_color].blue)), _brightnessScaleColor(brightnessNew, pgm_read_byte_near(&defaultColors[a_color].green)));
      colorOld = _strip->Color(_brightnessScaleColor(brightnessOld, pgm_read_byte_near(&defaultColors[a_color].red)), _brightnessScaleColor(brightnessOld, pgm_read_byte_near(&defaultColors[a_color].blue)), _brightnessScaleColor(brightnessOld, pgm_read_byte_near(&defaultColors[a_color].green)));
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
      colorOverlay1 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255));
      colorOverlay2 = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.5));
      colorOld = _strip->Color(_brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 0), _brightnessScaleColor(_brightnessInPercent, 255 * 0.1));
    }

    /*************
       WRITE OUT
    **************/
    _clear();
    _lastLEDsOn = 0;
    
    for (byte y = 0; y < 10; y++) {
      for (byte x = 5; x < 16; x++) {
        word t = 1 << x;
        if ((settings.getTransitionMode() == Settings::TRANSITION_MODE_FADE) && ((_matrixOld[y] & t) == t) && ((_matrixNew[y] & t) == t) ) {
          _setPixel(15 - x, y, color);
        }
        else {
          if ((_matrixOverlay[y] & t) == t) {
            _setPixel(15 - x, y, colorOverlay1);
            _lastLEDsOn++;
          }
          else if ((_matrixOverlay[y + 1] & t) == t) {
            _setPixel(15 - x, y, colorOverlay2);
            _lastLEDsOn++;
          }
          else if ((_matrixOld[y] & t) == t) {
            _setPixel(15 - x, y, colorOld);
            _lastLEDsOn++;
          }
          else if ((_matrixNew[y] & t) == t) {
            _setPixel(15 - x, y, colorNew);
            _lastLEDsOn++;
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
void LedDriverLPD8806::setBrightness(byte brightnessInPercent) {
  if ((brightnessInPercent != _brightnessInPercent) && _transitionCompleted) {
    _brightnessInPercent = brightnessInPercent;
    _dirty = true;
  }
}

/**
   Die aktuelle Helligkeit bekommen.
*/
byte LedDriverLPD8806::getBrightness() {
  return _brightnessInPercent;
}

/**
   Anpassung der Groesse des Bildspeichers.

   @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
                       geschrieben werden?
*/
void LedDriverLPD8806::setLinesToWrite(byte linesToWrite) {
}

/**
   Das Display ausschalten.
*/
void LedDriverLPD8806::shutDown() {
  _clear();
  _strip->show();
  _transitionCompleted = true;
}

/**
   Das Display einschalten.
*/
void LedDriverLPD8806::wakeUp() {
}

/**
   Den Dateninhalt des LED-Treibers loeschen.
*/
void LedDriverLPD8806::clearData() {
  _clear();
  _strip->show();
}

/**
   Einen X/Y-koordinierten Pixel in der Matrix setzen.
*/
void LedDriverLPD8806::_setPixel(byte x, byte y, uint32_t c) {
#ifdef RGBW_LEDS
  _setPixel(y + (x * 10), c);
#else
  _setPixel(x + (y * 11), c);
#endif
}

/**
   Einen Pixel im Streifen setzten (die Eck-LEDs sind am Ende).
*/
void LedDriverLPD8806::_setPixel(byte num, uint32_t c) {
#if defined(MATRIX_XXL) //|| defined(RGBW_LEDS)
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
#elif defined(RGBW_LEDS)
  byte ledNum;
  if (num < 110) {
    if ((num / 10) % 2 == 0) {
      ledNum = num;
    } else {
      ledNum = ((num / 10) * 10) + 9 - (num % 10);
    }
    if (ledNum < 10)
    {
      _strip->setPixelColor(ledNum + 1, c);
    }
    else if (ledNum < 100)
    {
      _strip->setPixelColor(ledNum + 2, c);
    }
    else
    {
      _strip->setPixelColor(ledNum + 3, c);
    }
  } else {
    switch (num) {
      case 110:
        _strip->setPixelColor(0, c);
        break;
      case 111:
        _strip->setPixelColor(102, c);
        break;
      case 112:
        _strip->setPixelColor(113, c);
        break;
      case 113:
        _strip->setPixelColor(11, c);
        break;
      case 114:
        _strip->setPixelColor(114, c);
        break;
      default:
        ;
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
      default:
        ;
    }
  }
#endif
  delay(1);
}

/**
   Funktion fuer saubere 'Regenbogen'-Farben.
   Kopiert aus den Adafruit-Beispielen (strand).
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
   Hilfsfunktion fuer das Skalieren der Farben.
*/
byte LedDriverLPD8806::_brightnessScaleColor(byte brightness, byte colorPart) {
  return map(brightness, 0, 100, 0, colorPart / 2); // LPD8806 kann nur 7 bit Farben! (also 0..127, nicht 0..255)
}

/**
   Streifen loeschen.
*/
void LedDriverLPD8806::_clear() {
  for (byte i = 0; i < NUM_PIXEL; i++) {
    _strip->setPixelColor(i, 0);
  }
}
