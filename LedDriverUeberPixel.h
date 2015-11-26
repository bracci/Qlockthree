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
#ifndef LED_DRIVER_UEBERPIXEL_H
#define LED_DRIVER_UEBERPIXEL_H

#include "Arduino.h"
#include <LedControl.h>
#include "LedDriver.h"

class LedDriverUeberPixel : public LedDriver {
public:
    LedDriverUeberPixel(byte data, byte clock, byte load);

    void init();

    void printSignature();

    void writeScreenBufferToMatrix(word matrix[16], boolean onChange);

    void setBrightness(byte brightnessInPercent);
    byte getBrightness();

    void setLinesToWrite(byte linesToWrite);

    void shutDown();
    void wakeUp();

    void clearData();

private:
    void _setPixel(byte x, byte y, boolean state);

    byte _brightnessInPercent;

    LedControl *_ledControl;
};

#endif
