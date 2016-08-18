/**
   LedDriverDefault
   Implementierung auf der Basis 74HC595 und UDN2981A.

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.4
   @created  18.1.2013
   @updated  18.1.2015

   Versionshistorie:
   V 1.0:  - Erstellt.
   V 1.1:  - printSignature() eingefuehrt.
           - Benennung verbessert.
   V 1.2:  - Anpassung Helligkeit 0-100%
   V 1.3:  - Getter fuer Helligkeit nachgezogen.
   V 1.4:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
*/
#include "LedDriverDefault.h"
#include "Configuration.h"

// #define DEBUG
#include "Debug.h"

// Hilfsvariable, um das Display auf den Kopf zu stellen
#ifdef UPSIDE_DOWN
#define DISPLAY_SHIFT  (_linesToWrite-1)-
#else
#define DISPLAY_SHIFT  0+
#endif

/**
   Initialisierung.

   @param data Pin, an dem die Data-Line haengt.
   @param clock Pin, an dem die Clock-Line haengt.
   @param latch Pin, an dem die Latch-Line haengt.
   @param outputEnable Pin, an dem OutputEnable haengt.
   @param linesToWrite Wieviel Zeilen muessen aus dem Bildspeicher uebernommen werden?
*/
LedDriverDefault::LedDriverDefault(byte data, byte clock, byte latch, byte outputEnable, byte linesToWrite) {
  _shiftRegister = new ShiftRegister(data, clock, latch);
  _outputEnablePin = outputEnable;
  pinMode(_outputEnablePin, OUTPUT);
  digitalWrite(_outputEnablePin, HIGH);
  _linesToWrite = linesToWrite;
  _transitionCompleted = true;
}

/**
   init() wird im Hauptprogramm in init() aufgerufen.
   Hier sollten die LED-Treiber in eine definierten
   Ausgangszustand gebracht werden.
*/
void LedDriverDefault::init() {
}

void LedDriverDefault::printSignature() {
#ifdef MOS_DRIVER
  DEBUG_PRINTLN(F("Default - 74HC595 MOS"));
#else
  DEBUG_PRINTLN(F("Default - 74HC595"));
#endif
}

/**
   Den Bildschirm-Puffer auf die LED-Matrix schreiben.

   @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
                    FALSE, wenn es ein Refresh-Aufruf war.
*/
void LedDriverDefault::writeScreenBufferToMatrix(word matrix[16], boolean onChange, eColors a_color) {

  if (!_transitionCompleted && (_transitionCounter > 0)) {
    _transitionCounter--;
  }
  else
  {
    _transitionCounter = 0;
  }

  /*************
    MATRIX
  **************/

  if (mode != STD_MODE_NORMAL) {
    _transitionCompleted = true;
  }

  /*************
     MATRIX
  **************/

  if (onChange) {
    if ((helperSeconds == 0) && (mode == STD_MODE_NORMAL) && _transitionCompleted && !evtActive) {
      switch (settings.getTransitionMode()) {
        case Settings::TRANSITION_MODE_FADE:
          for (byte i = 0; i < _linesToWrite; i++) {
            _matrixOld[i] = _matrixNew[i]; //Abbild der aktuellen Matrix in Vorversion rüberkopieren
            _matrixNew[DISPLAY_SHIFT i] = matrix[i]; //Abbild der neuen Matrix in aktuelle Matrix einkopieren, um sie beim nächsten überblenden zu haben
          }
          _transitionCounter = FADINGCOUNTERLOAD; // Überblendvariable mit Startwert laden
          _transitionCompleted = false;
          break;
        case Settings::TRANSITION_MODE_SLIDE:
          if ((rtc.getMinutes() % 5) == 0) {
            Transitions::resetTransition();
            for (byte i = 0; i < _linesToWrite; i++) {
              _matrixOld[i] = 0;
              _matrixNew[i] &= ~0b0000000000011111;
            }
            _transitionCompleted = false;
          }
          break;
        default:
          ;
      }
    }
    if (_transitionCompleted) {
      for (byte i = 0; i < _linesToWrite; i++) {
        _matrixOld[i] = 0; //Aktuelles Abbild ist nicht verwendbar, weil eventuell keine Uhrzeit
        _matrixNew[DISPLAY_SHIFT i] = matrix[i]; //Abbild der neuen Matrix in aktuelle Matrix einkopieren, um sie beim nächsten überblenden zu haben
      }
    }
  }

  if ((_transitionCounter == 0) && !_transitionCompleted) {
    switch (settings.getTransitionMode()) {
      case Settings::TRANSITION_MODE_SLIDE:
        _transitionCounter = SLIDINGCOUNTERLOAD;
        _transitionCompleted = Transitions::nextSlideStep(_matrixNew, matrix);
        break;
      default:
        ;
    }
  }

  word row = 1;

  if ((Settings::TRANSITION_MODE_FADE == settings.getTransitionMode()) && !_transitionCompleted) {
    _delayOldMatrix =  map(_transitionCounter, 0, FADINGCOUNTERLOAD, 1, ((_brightnessInPercent * PWM_DURATION) + 132)); //Summand ist Korrektur um die Zeit, die das Einschieben der 32 bit für "PWM dunkel" dauert
    _delayNewMatrix =  map(_transitionCounter, FADINGCOUNTERLOAD, 0, 1, ((_brightnessInPercent * PWM_DURATION) + 132));
    if (_transitionCounter == 0) {
      _transitionCompleted = true;
    }
  }
  else {
    _delayOldMatrix =  0;
    _delayNewMatrix =  (_brightnessInPercent * PWM_DURATION) + 132;
  }

  // Jetzt die Daten...
  // wir brauchen keine 16, 10 wuerden reichen... dann gehen aber nicht
  // mehr alle Modi! Also via Variable, die im Modus-Wechsel geaendert wird...
  for (byte k = 0; k < _linesToWrite; k++) {
#ifdef SKIP_BLANK_LINES
    if (matrix[k] != 0) {
#endif

      if ((Settings::TRANSITION_MODE_FADE == settings.getTransitionMode()) && !_transitionCompleted) { // Über OE einschalten und nach PWM-Anteil wieder ausschalten, wenn das Display aktiv ist
        // Alter Zeileninhalt
        // Zeile schreiben...
        _shiftRegister->prepareShiftregisterWrite();
#ifdef MOS_DRIVER
        _shiftRegister->shiftOut(_matrixOld[k]);
        _shiftRegister->shiftOut(~row);
#else
        _shiftRegister->shiftOut(~_matrixOld[k]);
        _shiftRegister->shiftOut(row);
#endif
        _shiftRegister->finishShiftregisterWrite();

        digitalWrite(_outputEnablePin, LOW);
        delayMicroseconds(_delayOldMatrix);
        digitalWrite(_outputEnablePin, HIGH);
      }
      // Neuer Zeileninhalt
      // Zeile überschreiben...
      _shiftRegister->prepareShiftregisterWrite();
#ifdef MOS_DRIVER
      _shiftRegister->shiftOut(_matrixNew[k]);
      _shiftRegister->shiftOut(~row);
#else
      _shiftRegister->shiftOut(~_matrixNew[k]);
      _shiftRegister->shiftOut(row);
#endif
      _shiftRegister->finishShiftregisterWrite();

      digitalWrite(_outputEnablePin, LOW); // über OE einschalten und nach PWM-Anteil wieder ausschalten, wenn das Display aktiv ist
      delayMicroseconds(_delayNewMatrix);
      digitalWrite(_outputEnablePin, HIGH); // bleibt danach ausgeschaltet

      // hier kann man versuchen, das Taktverhaeltnis zu aendern (Auszeit)...
      // delayMicroseconds mit Werten <= 3 macht Probleme...
      if (_brightnessInPercent < 97) {
        delayMicroseconds((100 - _brightnessInPercent) * PWM_DURATION);
      }

#ifdef SKIP_BLANK_LINES
    }
#endif

    row = row << 1;
  }
}

/**
   Die Helligkeit des Displays anpassen.

   @param brightnessInPercent Die Helligkeit.
*/
void LedDriverDefault::setBrightness(byte brightnessInPercent) {
  _brightnessInPercent = brightnessInPercent;
}

/**
   Die aktuelle Helligkeit bekommen.
*/
byte LedDriverDefault::getBrightness() {
  return _brightnessInPercent;
}

/**
   Anpassung der Groesse des Bildspeichers.

   @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
                       geschrieben werden?
*/
void LedDriverDefault::setLinesToWrite(byte linesToWrite) {
  _linesToWrite = linesToWrite;
}

/**
   Das Display ausschalten.
*/
void LedDriverDefault::shutDown() {
  digitalWrite(_outputEnablePin, HIGH);
}

/**
   Das Display einschalten.
*/
void LedDriverDefault::wakeUp() {
  digitalWrite(_outputEnablePin, LOW);
}

/**
   Den Dateninhalt des LED-Treibers loeschen.
*/
void LedDriverDefault::clearData() {
  _shiftRegister->prepareShiftregisterWrite();
#ifdef MOS_DRIVER
  _shiftRegister->shiftOut(0);
  _shiftRegister->shiftOut(65535);
#else
  _shiftRegister->shiftOut(65535);
  _shiftRegister->shiftOut(0);
#endif
  _shiftRegister->finishShiftregisterWrite();
}
