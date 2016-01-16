/**
 * Renderer
 * Diese Klasse rendert die Woerter auf die Matrix.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.5
 * @created  21.1.2013
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Spanisch hinzugefuegt.
 * V 1.2:  - setMinutes - hours auf char umgestellt, damit Zeitverschiebung geht...
 * V 1.3:  - Alle Deutsch-Varianten zusammengefasst, um Platz zu sparen.
 *         - Fehler im Italienischen behoben.
 * V 1.4:  - Stundenbegrenzung (die ja wegen der Zeitverschiebungsmoeglichkeit existiert) auf den Bereich 0 <= h <= 24 ausgeweitet, dank Tipp aus dem Forum.
 * V 1.5:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#ifndef RENDERER_H
#define RENDERER_H

#include "Arduino.h"

#define LANGUAGE_DE_DE 0
#define LANGUAGE_DE_SW 1
#define LANGUAGE_DE_BA 2
#define LANGUAGE_DE_SA 3
#define LANGUAGE_CH    4
#define LANGUAGE_CH_X  5
//#define LANGUAGE_EN    6
//#define LANGUAGE_FR    7
//#define LANGUAGE_IT    8
//#define LANGUAGE_NL    9
//#define LANGUAGE_ES    10
#define LANGUAGE_COUNT 5

class Renderer {
  public:
    static Renderer* getInstance() {
      static Renderer _instance;
      return &_instance;
    }

    void setMinutes(char hours, byte minutes, byte language, word matrix[16]);
    void setCorners(byte minutes, boolean cw, word matrix[16]);

    void cleanWordsForAlarmSettingMode(byte language, word matrix[16]);

    void scrambleScreenBuffer(word matrix[16]);
    void clearScreenBuffer(word matrix[16]);
    void setAllScreenBuffer(word matrix[16]);

  private:
    Renderer(){}
    ~Renderer() {}

    void setHours(byte hours, boolean glatt, byte language, word matrix[16]);

    // Spezialfaelle
    //    void FR_hours(byte hours, word matrix[16]);
    //    void IT_hours(byte hours, word matrix[16]);
    //    void ES_hours(byte hours, word matrix[16]);
};

#endif
