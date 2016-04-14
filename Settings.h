/**
   Settings
   Die vom Benutzer getaetigten Einstellungen werden hier verwaltet
   und im EEPROM persistiert.

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  1.3
   @created  23.1.2013
   @updated  16.2.2015

   Versionshistorie:
   V 1.0:  - Erstellt.
   V 1.1:  - LDR-Modus aufgenommen.
   V 1.2:  - Brightness aufgenommen.
           - EnableAlarm aufgenommen.
           - DcfSignalIsInverted aufgenommen.
           - TimeShift aufgenommen.
   V 1.3:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
*/
#ifndef SETTINGS_H
#define SETTINGS_H

#include "Arduino.h"
#include "Colors.h"
#include "TimeStamp.h"

class Settings {
  public:
    enum eTransitions : byte {
      TRANSITION_MODE_NORMAL,
      TRANSITION_MODE_FADE,
      TRANSITION_MODE_MATRIX,
      TRANSITION_MODE_SLIDE,

      TRANSITION_MODE_MAX
    };

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

    void setColor(eColors color);
    eColors getColor();

    boolean getEnableAlarm();
    void setEnableAlarm(boolean enableAlarm);

    byte getTransitionMode();
    void setTransitionMode(byte transitionMode);

    boolean getDcfSignalIsInverted();
    void setDcfSignalIsInverted(boolean dcfSignalIsInverted);

    char getTimeShift();
    void setTimeShift(char timeShift);

    byte getJumpToNormalTimeout();
    void setJumpToNormalTimeout(byte jumpToNormalTimeout);

    byte getColorChangeRate();
    void setColorChangeRate(byte rate);

    void loadFromEEPROM();
    void saveToEEPROM();

    void resetToDefault();

    TimeStamp* getNightModeTime(bool onTime);

  private:
    byte _language;
    boolean _renderCornersCw;
    boolean _use_ldr;
    byte _brightness;
    boolean _enableAlarm;
    boolean _dcfSignalIsInverted;
    char _timeShift;
    eColors _color;
    byte _transitionMode;
    byte _event;
    TimeStamp* _nightModeTime[2];

    byte _jumpToNormalTimeout;
    byte _colorChangeRate;
};

#endif

