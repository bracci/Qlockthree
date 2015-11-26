/**
 * Modes.h
 * Modi Enum
 *
 * @mc       Arduino/UNO
 * @autor    Manuel Bracher / manuel.bracher@gmail.com
 * @version  1.0
 * @created  20.08.15
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */

#ifndef MODES_H
#define MODES_H

/**
 * Die Standard-Modi.
 */
typedef enum eMode : byte
{
  STD_MODE_NORMAL,
  STD_MODE_ALARM,
  STD_MODE_SECONDS,
  STD_MODE_BRIGHTNESS,
  STD_MODE_BLANK,
  STD_MODE_COUNT = STD_MODE_BLANK,
  // nicht manuell zu erreichender Modus...
  STD_MODE_NIGHT,


/**
 * Die erweiterten Modi.
 */
  EXT_MODE_START,
  EXT_MODE_LDR_MODE = EXT_MODE_START,
  EXT_MODE_CORNERS,
  EXT_MODE_ENABLE_ALARM,
  EXT_MODE_RAINBOW,
  EXT_MODE_TRANSITION,
#ifdef EVENTS
  EXT_MODE_EVENT,
#endif
  EXT_MODE_DCF_IS_INVERTED,
  EXT_MODE_LANGUAGE,
  EXT_MODE_TIMESET,
  EXT_MODE_TIME_SHIFT,
  EXT_MODE_TEST,
  EXT_MODE_DCF_DEBUG,
  EXT_MODE_COUNT = EXT_MODE_DCF_DEBUG
} Mode;

// Overload the ControlType++ operator
inline Mode& operator++(Mode& eDOW, int)  // <--- note -- must be a reference
{
  const int i = static_cast<int>(eDOW)+1;
  eDOW = static_cast<Mode>((i) % EXT_MODE_COUNT);
  return eDOW;
}

#endif // MODES_H
