/**
 * IRTranslatorMooncandles
 * Remote-Klasse fuer die Mooncandles-Remote (z. B. http://www.amazon.de/dp/B006L5YO78).
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  7.2.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt..
 */
#include "IRTranslatorMooncandles.h"

// #define DEBUG
#include "Debug.h"

// Codes, mit #DEBUG im Hauptprogramm ausgelesen...
#define MOONCANDLES_ON            0x1FE48B7
#define MOONCANDLES_OFF           0x1FE58A7
#define MOONCANDLES_MODE          0x1FE7887
#define MOONCANDLES_4H            0x1FE807F
#define MOONCANDLES_8H            0x1FE40BF
#define MOONCANDLES_MULTI_COLOR   0x1FEC03F

#define MOONCANDLES_ROT           0x1FE20DF
#define MOONCANDLES_GRUEN         0x1FEA05F
#define MOONCANDLES_BLAU          0x1FE609F

#define MOONCANDLES_ORANGE        0x1FEE01F
#define MOONCANDLES_HELLGRUEN     0x1FE10EF
#define MOONCANDLES_HELLERES_BLAU 0x1FE906F

#define MOONCANDLES_GELB          0x1FE50AF
#define MOONCANDLES_TUERKIS       0x1FED827
#define MOONCANDLES_PINK          0x1FEF807

#define MOONCANDLES_WEISS         0x1FE30CF
#define MOONCANDLES_HELLBLAU      0x1FEB04F
#define MOONCANDLES_MAGENTA       0x1FE708F

void IRTranslatorMooncandles::printSignature() {
  DEBUG_PRINTLN(F("Mooncandles"));
}

byte IRTranslatorMooncandles::buttonForCode(unsigned long code) {
  switch (code) {
    case MOONCANDLES_ON:
      return REMOTE_BUTTON_RESUME;
    case MOONCANDLES_OFF:
      return REMOTE_BUTTON_BLANK;
    case MOONCANDLES_MODE:
      return REMOTE_BUTTON_MODE;
    case MOONCANDLES_4H:
      return REMOTE_BUTTON_HOUR_PLUS;
    case MOONCANDLES_8H:
      return REMOTE_BUTTON_MINUTE_PLUS;
    case MOONCANDLES_MULTI_COLOR:
      return REMOTE_BUTTON_EXTMODE;

    case MOONCANDLES_ROT:
      setColor(color_red);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_GRUEN:
      setColor(color_green);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_BLAU:
      setColor(color_darkBlue);
      return REMOTE_BUTTON_SETCOLOR;

    case MOONCANDLES_ORANGE:
      setColor(color_orange1);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_HELLGRUEN:
      setColor(color_turquois1);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_HELLERES_BLAU:
      setColor(color_purple1);
      return REMOTE_BUTTON_SETCOLOR;

    case MOONCANDLES_GELB:
      setColor(color_yellow);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_TUERKIS:
      setColor(color_turquois2);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_PINK:
      setColor(color_purple2);
      return REMOTE_BUTTON_SETCOLOR;

    case MOONCANDLES_WEISS:
      setColor(color_white);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_HELLBLAU:
      setColor(color_turquois3);
      return REMOTE_BUTTON_SETCOLOR;
    case MOONCANDLES_MAGENTA:
      setColor(color_purple3);
      return REMOTE_BUTTON_SETCOLOR;

    default:
      return REMOTE_BUTTON_UNDEFINED;
  }
}
