/**
 * IRTranslatorCLT
 * Remote-Klasse fuer die Lunartec-Remote (NX6612-901 / http://www.pearl.de/a-NX6612-3350.shtml?vid=901).
 *
 * @mc       Arduino/RBBB
 * @autor    Manuel Bracher / bracher _AT_ win _DOT_ com
 * @version  1.1
 * @created  5.5.2015
 * @updated  5.5.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */
#include "IRTranslatorCLT.h"
#include "Colors.h"

// #define DEBUG
#include "Debug.h"

// Codes, mit #DEBUG im Hauptprogramm ausgelesen...
#define CLT_TIME           0xFFA05F
#define CLT_REGION         0xFF20DF
#define CLT_SECONDS        0xFF609F
#define CLT_ON_OFF         0xFFE01F

#define CLT_RED            0xFF906F
#define CLT_GREEN          0xFF10EF
#define CLT_DARKBLUE       0xFF50AF
#define CLT_PINK           0xFFD02F
#define CLT_WHITE          0xFFB04F
#define CLT_BRIGHTBLUE     0xFF30CF
#define CLT_YELLOW         0xFF708F
#define CLT_RGB            0xFFF00F

#define CLT_NORMAL         0xFFA857
#define CLT_FADE           0xFF28D7
#define CLT_MATRIX         0xFF6897
#define CLT_SLIDE          0xFFE817

#define CLT_STD_PLUS       0xFF9867
#define CLT_STD_MINUS      0xFF8877
#define CLT_MIN_PLUS       0xFF18E7
#define CLT_MIN_MINUS      0xFF08F7

#define CLT_BRIGHTER       0xFF58A7
#define CLT_DARKER         0xFF48B7

#define CLT_LDR            0xFFD827
#define CLT_SAVE           0xFFC837

void IRTranslatorCLT::printSignature() {
  DEBUG_PRINTLN(F("CLT leuchtbildshop"));
}

byte IRTranslatorCLT::buttonForCode(unsigned long code) {
  switch (code) {

    case CLT_TIME:
      setMode(STD_MODE_NORMAL);
      return REMOTE_BUTTON_SETMODE;
    case CLT_REGION:
      return REMOTE_BUTTON_REGION;
    case CLT_SECONDS:
      return REMOTE_BUTTON_SECONDS;
    case CLT_ON_OFF:
      return REMOTE_BUTTON_TOGGLEBLANK;
    case CLT_RED:
      setColor(color_red);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_GREEN:
      setColor(color_green);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_DARKBLUE:
      setColor(color_darkBlue);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_PINK:
      setColor(color_pink);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_WHITE:
      setColor(color_white);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_BRIGHTBLUE:
      setColor(color_brightBlue);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_YELLOW:
      setColor(color_yellow);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_RGB:
      setColor(color_rgb_continuous);
      return REMOTE_BUTTON_SETCOLOR;
    case CLT_NORMAL:
      setTransition(Settings::TRANSITION_MODE_NORMAL);
      return REMOTE_BUTTON_TRANSITION;
    case CLT_FADE:
      setTransition(Settings::TRANSITION_MODE_FADE);
      return REMOTE_BUTTON_TRANSITION;
    case CLT_MATRIX:
      setTransition(Settings::TRANSITION_MODE_MATRIX);
      return REMOTE_BUTTON_TRANSITION;
    case CLT_SLIDE:
      setTransition(Settings::TRANSITION_MODE_SLIDE);
      return REMOTE_BUTTON_TRANSITION;
    case CLT_STD_PLUS:
      return REMOTE_BUTTON_TIME_H_PLUS;
    case CLT_STD_MINUS:
      return REMOTE_BUTTON_TIME_H_MINUS;
    case CLT_MIN_PLUS:
      return REMOTE_BUTTON_TIME_M_PLUS;
    case CLT_MIN_MINUS:
      return REMOTE_BUTTON_TIME_M_MINUS;
    case CLT_BRIGHTER:
      return REMOTE_BUTTON_BRIGHTER;
    case CLT_DARKER:
      return REMOTE_BUTTON_DARKER;
    case CLT_LDR:
      return REMOTE_BUTTON_LDR;
    case CLT_SAVE:
      return REMOTE_BUTTON_EXTMODE;
    default:
      return REMOTE_BUTTON_UNDEFINED;
  }
}
