/**
 * Zahlen
 * Definition der Zahlen fuer die Sekundenanzeige der QLOCKTWO.
 * Die Zahlen sind wie die Woerter Bitmasken fuer die Matrix.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.3
 * @created  18.2.2011
 * @updated  18.1.2013
 *
 * Versionshistorie:
 * V 1.1:  - A/M fuer die Umschaltung zwischen LDR auto/manuell hinzugefuegt (Danke an Alexander).
 * V 1.2:  - Bitmaps in den PROGMEM ausgelagert.
 * V 1.3:  - Staben von V 1.1 in eigene Datei ausgelagert und das Alphabet komplettiert.
 */
#ifndef ZAHLEN_H
#define ZAHLEN_H

#include <avr/pgmspace.h>

//extern const char ziffern[][7] PROGMEM;
const char ziffern[][7] PROGMEM = {
  { // 0:0
    0b00001110,
    0b00010001,
    0b00010011,
    0b00010101,
    0b00011001,
    0b00010001,
    0b00001110
  }
  ,
  { // 1:1
    0b00000100,
    0b00001100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00001110
  }
  ,
  { // 2:2
    0b00001110,
    0b00010001,
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00011111
  }
  ,
  { // 3:3
    0b00011111,
    0b00000010,
    0b00000100,
    0b00000010,
    0b00000001,
    0b00010001,
    0b00001110
  }
  ,
  { // 4:4
    0b00000010,
    0b00000110,
    0b00001010,
    0b00010010,
    0b00011111,
    0b00000010,
    0b00000010
  }
  ,
  { // 5:5
    0b00011111,
    0b00010000,
    0b00011110,
    0b00000001,
    0b00000001,
    0b00010001,
    0b00001110
  }
  ,
  { // 6:6
    0b00000110,
    0b00001000,
    0b00010000,
    0b00011110,
    0b00010001,
    0b00010001,
    0b00001110
  }
  ,
  { // 7:7
    0b00011111,
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00001000
  }
  ,
  { // 8:8
    0b00001110,
    0b00010001,
    0b00010001,
    0b00001110,
    0b00010001,
    0b00010001,
    0b00001110
  }
  ,
  { // 9:9
    0b00001110,
    0b00010001,
    0b00010001,
    0b00001111,
    0b00000001,
    0b00000010,
    0b00001100
  }
};
//kleinere Zahlen f?r Eventzeitauswahl
const char ziffernB[][5] PROGMEM = {
  {0x1e, 0x12, 0x12, 0x12, 0x1e},
  {0x04, 0x0c, 0x04, 0x04, 0x0e},
  {0x1e, 0x02, 0x1e, 0x10, 0x1e},
  {0x1e, 0x02, 0x0e, 0x02, 0x1e},
  {0x12, 0x12, 0x1e, 0x02, 0x02},
  {0x1e, 0x10, 0x1e, 0x02, 0x1e},
  {0x1e, 0x10, 0x1e, 0x12, 0x1e},
  {0x1e, 0x02, 0x02, 0x02, 0x02},
  {0x1e, 0x12, 0x1e, 0x12, 0x1e},
  {0x1e, 0x12, 0x1e, 0x02, 0x1e}
};
#endif


