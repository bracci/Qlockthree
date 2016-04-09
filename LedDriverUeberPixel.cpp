/**
 * LedDriverUeberPixel
 * Implementierung auf der Basis von 4 MAX7219 wie es der Ueberpixel verwendet.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.4
 * @created  18.1.2013
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - printSignature() eingefuehrt.
 *         - Bennenung verbessert.
 * V 1.2:  - Anpassung auf Helligkeit 0-100%
 * V 1.3:  - Getter fuer Helligkeit nachgezogen.
 * V 1.4:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#include "LedDriverUeberPixel.h"

// #define DEBUG
#include "Debug.h"

/**
 * Initialisierung.
 *
 * @param data Pin, an dem die Data-Line haengt.
 * @param clock Pin, an dem die Clock-Line haengt.
 * @param latch Pin, an dem die Latch-Line haengt.
 */
LedDriverUeberPixel::LedDriverUeberPixel(byte data, byte clock, byte load) {
  _ledControl = new LedControl(data, clock, load, 4);
}

/**
 * init() wird im Hauptprogramm in init() aufgerufen.
 * Hier sollten die LED-Treiber in eine definierten
 * Ausgangszustand gebracht werden.
 */
void LedDriverUeberPixel::init() {
  setBrightness(100);
  wakeUp();
  _ledControl->setLed(0, 6, 5, true); // 1
  delay(250);
  _ledControl->setLed(1, 5, 5, true); // 2
  delay(250);
  _ledControl->setLed(2, 6, 5, true); // 3
  delay(250);
  _ledControl->setLed(3, 5, 5, true); // 4
  delay(250);
}

void LedDriverUeberPixel::printSignature() {
  DEBUG_PRINTLN(F("UeberPixel - MAX7219"));
}

/**
 * Den Bildschirm-Puffer auf die LED-Matrix schreiben.
 *
 * @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
 *                  FALSE, wenn es ein Refresh-Aufruf war.
 */
void LedDriverUeberPixel::writeScreenBufferToMatrix(word matrix[16], boolean onChange, eColors a_color) {
  if (onChange) {
    for (byte y = 0; y < 10; y++) {
      for (byte x = 5; x < 16; x++) {
        word t = 1 << x;
        if ((matrix[y] & t) == t) {
          _setPixel(15 - x, y, true);
        } else {
          _setPixel(15 - x, y, false);
        }
      }
    }
    // wir muessen die Eck-LEDs umsetzten...
    if ((matrix[1] & 0b0000000000011111) == 0b0000000000011111) {
      _ledControl->setLed(0, 6, 5, true); // 1
    } else {
      _ledControl->setLed(0, 6, 5, false); // 1
    }
    if ((matrix[0] & 0b0000000000011111) == 0b0000000000011111) {
      _ledControl->setLed(1, 5, 5, true); // 2
    } else {
      _ledControl->setLed(1, 5, 5, false); // 2
    }
    if ((matrix[3] & 0b0000000000011111) == 0b0000000000011111) {
      _ledControl->setLed(2, 6, 5, true); // 3
    } else {
      _ledControl->setLed(2, 6, 5, false); // 3
    }
    if ((matrix[2] & 0b0000000000011111) == 0b0000000000011111) {
      _ledControl->setLed(3, 5, 5, true); // 4
    } else {
      _ledControl->setLed(3, 5, 5, false); // 4
    }
  }
}

/**
 * Die Helligkeit des Displays anpassen.
 *
 * @param brightnessInPercent Die Helligkeit.
 */
void LedDriverUeberPixel::setBrightness(byte brightnessInPercent) {
  if (_brightnessInPercent != brightnessInPercent) {
    _brightnessInPercent = brightnessInPercent;

    DEBUG_PRINT(F("MAX7219: Brightness: "));
    DEBUG_PRINTLN(_brightnessInPercent);
    DEBUG_FLUSH();

    byte val = map(_brightnessInPercent, 0, 100, 1, 15);
    DEBUG_PRINT(F(" val: "));
    DEBUG_PRINTLN(val);
    DEBUG_FLUSH();
    for (byte i = 0; i < 4; i++) {
      _ledControl->setIntensity(i, val);
    }
  }
}

/**
 * Die aktuelle Helligkeit bekommen.
 */
byte LedDriverUeberPixel::getBrightness() {
  return _brightnessInPercent;
}

/**
 * Anpassung der Groesse des Bildspeichers.
 *
 * @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
 *                     geschrieben werden?
 */
void LedDriverUeberPixel::setLinesToWrite(byte linesToWrite) {
}

/**
 * Das Display ausschalten.
 */
void LedDriverUeberPixel::shutDown() {
  for (byte i = 0; i < 4; i++) {
    _ledControl->shutdown(i, true);
  }
}

/**
 * Das Display einschalten.
 */
void LedDriverUeberPixel::wakeUp() {
  for (byte i = 0; i < 4; i++) {
    _ledControl->shutdown(i, false);
  }
}

/**
 * Den Dateninhalt des LED-Treibers loeschen.
 */
void LedDriverUeberPixel::clearData() {
  for (byte i = 0; i < 4; i++) {
    _ledControl->clearDisplay(i);
  }
}

/**
 * Einen X/Y-koordinierten Pixel in der Matrix setzen.
 */
void LedDriverUeberPixel::_setPixel(byte x, byte y, boolean state) {
  // 1. MAX7219
  if ((x < 6) && (y < 5)) {
    _ledControl->setLed(0, x, y, state);
  }

  // 2. MAX7219
  if ((x < 6) && (y > 4) && (y < 10)) {
    _ledControl->setLed(2, x, y - 5, state);
  }

  // 3. MAX7219
  if ((x > 5) && (x < 11) && (y < 5)) {
    _ledControl->setLed(1, x - 6, y, state);
  }

  // 4. MAX7219
  if ((x > 5) && (x < 11) && (y > 4) && (y < 10)) {
    _ledControl->setLed(3, x - 6, y - 5, state);
  }
}
