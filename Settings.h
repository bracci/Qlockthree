/**
 * Settings
 * Die vom Benutzer getaetigten Einstellungen werden hier verwaltet
 * und im EEPROM persistiert.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.3
 * @created  23.1.2013
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - LDR-Modus aufgenommen.
 * V 1.2:  - Brightness aufgenommen.
 *         - EnableAlarm aufgenommen.
 *         - DcfSignalIsInverted aufgenommen.
 *         - TimeShift aufgenommen.
 * V 1.3:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include "Arduino.h"

#define TRANSITION_MODE_NORMAL 0
#define TRANSITION_MODE_FADE   1
#define TRANSITION_MODE_MATRIX 2
#define TRANSITION_MODE_SLIDE  3
#define TRANSITION_MODE_COUNT  3

class Settings {
  public:
    Settings();

    byte getLanguage();
    void setLanguage(byte language);

    byte getEvent();
    void setEvent(byte event);

    boolean getRenderCornersCw();
    void setRenderCornersCw(boolean cw);

    boolean getUseLdr();
    void setUseLdr(boolean useLdr);

    byte getBrightness();
    void setBrightness(byte brightness);

    byte getRed();
    byte getGreen();
    byte getBlue();
    void setColor(byte red, byte green, byte blue);

    boolean getEnableAlarm();
    void setEnableAlarm(boolean enableAlarm);

    byte getTransitionMode();
    void setTransitionMode(byte transitionMode);

    boolean getDcfSignalIsInverted();
    void setDcfSignalIsInverted(boolean dcfSignalIsInverted);

    char getTimeShift();
    void setTimeShift(char timeShift);

    void loadFromEEPROM();
    void saveToEEPROM();

  private:
    byte _language;
    boolean _renderCornersCw;
    boolean _use_ldr;
    byte _brightness;
    boolean _enableAlarm;
    boolean _dcfSignalIsInverted;
    char _timeShift;
    byte _red;
    byte _green;
    byte _blue;
    byte _transitionMode;
byte _event;
};

#endif

