/**
 * IRTranslator
 * Abstrakte Klasse fuer die Umsetzung von Fernbedienungs-Codes.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  7.2.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#ifndef IRTRANSLATOR_H
#define IRTRANSLATOR_H

#include "Arduino.h"
#include "Settings.h"
#include "Modes.h"
#include "Colors.h"

#define REMOTE_BUTTON_UNDEFINED    0
// diese Buttons braucht man haeufig und sollten alle Fernbedienungen koennen...
#define REMOTE_BUTTON_MODE         1
#define REMOTE_BUTTON_MINUTE_PLUS  2
#define REMOTE_BUTTON_HOUR_PLUS    3
#define REMOTE_BUTTON_BRIGHTER     4
#define REMOTE_BUTTON_DARKER       5
#define REMOTE_BUTTON_EXTMODE      6
#define REMOTE_BUTTON_TOGGLEBLANK  7
#define REMOTE_BUTTON_BLANK        8
#define REMOTE_BUTTON_RESUME       9
#define REMOTE_BUTTON_SETCOLOR     10
// ... weitere koennen dann die konkreten Implementierungen hinzufuegen.
#define REMOTE_BUTTON_START_YOUR_OWN_AT  100

// für CLT Remote
#define REMOTE_BUTTON_SAVE         100
#define REMOTE_BUTTON_SETMODE      101
#define REMOTE_BUTTON_REGION       102
#define REMOTE_BUTTON_LDR          103
#define REMOTE_BUTTON_TIME_H_PLUS  104
#define REMOTE_BUTTON_TIME_H_MINUS 105
#define REMOTE_BUTTON_TIME_M_PLUS  106
#define REMOTE_BUTTON_TIME_M_MINUS 107
#define REMOTE_BUTTON_TRANSITION   108
#define REMOTE_BUTTON_SECONDS      109

class IRTranslator {
  public:
    virtual void printSignature();
    virtual byte buttonForCode(unsigned long code);
    eColors getColor();
    Mode getMode();
    byte getTransition();

  protected:
    void setColor(eColors);
    void setMode(Mode mode);
    void setTransition(byte transition);

  private:
    byte _transition;
    eColors _color;
    Mode _mode;
};

#endif
