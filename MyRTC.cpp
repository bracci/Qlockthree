/**
   MyRTC
   Klasse fuer den Zugriff auf die DS1307/DS3231 Echtzeituhr.

   @mc       Arduino/RBBB
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  2.1
   @created  1.3.2011
   @updated  16.2.2015

   Versionshistorie:
   V 1.1:  - dayOfMonth nach date umbenannt.
   V 1.2:  - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
   V 1.3:  - getMinutesOfDay eingefuehrt.
   V 1.4:  - getMinutesOf12HoursDay eingefuehrt.
   V 1.5:  - Optimierung hinsichtlich Speicherbedarf.
   V 1.6:  - Verbessertes Debugging.
   V 1.7:  - Multi-MCU-Faehigkeit hinzugefuegt.
   V 1.8:  - Auslesen verbessert. Falls die angeforderten 7 Bytes nicht kommen, verwerfen und neu anfordern.
   V 1.9:  - Macro zum Stellen der Uhr durch die Compile-Zeit von Kee-Labs geklaut und hier eingefuegt.
   V 2.0:  - DS1307 nach MyRTC umbenannt, weil es jetzt nicht mehr nur um die DS1307 geht.
           - Getrennte Logik fuer das Rachtencksignal (SQW) eingefuehrt, danke an Erich M.
   V 2.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
*/
#include <Wire.h> // Wire library fuer I2C
#include "MyRTC.h"

// #define DEBUG
#include "Debug.h"

/**
   Initialisierung mit der Adresse der DS1307
*/
MyRTC::MyRTC(int address, byte statusLedPin) {
  _address = address;
  _statusLedPin = statusLedPin;
  pinMode(_statusLedPin, OUTPUT);
  digitalWrite(_statusLedPin, LOW);
}

/**
   Die LED ein- oder ausschalten.
*/
void MyRTC::statusLed(boolean on) {
  if (on) {
    digitalWrite(_statusLedPin, HIGH);
  } else {
    digitalWrite(_statusLedPin, LOW);
  }
}

/**
   Die Uhrzeit auslesen und in den Variablen ablegen
*/
void MyRTC::readTime() {
  byte returnStatus, count, result, retries = 0;
  do {
    // Reset the register pointer
    Wire.beginTransmission(_address);
    Wire.write((uint8_t) 0x00);
    result = Wire.endTransmission(false); // false, damit der Bus nicht freigegeben wird und eventuell andere dazwischen kommen (in Multi-MCU-Umgebungen)
    DEBUG_PRINT(F("Wire.endTransmission(false) = "));
    DEBUG_PRINTLN(result);

    count = Wire.requestFrom(_address, 7);
    DEBUG_PRINT(F("Wire.requestFrom(_address, 7) = "));
    DEBUG_PRINTLN(count);
    DEBUG_FLUSH();

    if (count == 7) {
      // Success
      // A few of these need masks because certain bits are control bits
      _seconds = bcdToDec(Wire.read() & 0x7f);
      _minutes = bcdToDec(Wire.read());
      _hours = bcdToDec(Wire.read() & 0x3f); // Need to change this if 12 hour am/pm
      _dayOfWeek = bcdToDec(Wire.read());
      _date = bcdToDec(Wire.read());
      _month = bcdToDec(Wire.read());
      _year = bcdToDec(Wire.read());
    } else {
      // Fail
      // keine 7 Byte zurueck gekommen? Buffer verwerfen...
      for (int i = 0; i < count; i++) {
        Wire.read();
      }
      retries++;
    }

    result = Wire.endTransmission(true); // true, jetzt den Bus freigeben.
    DEBUG_PRINT(F("Wire.endTransmission(true) = "));
    DEBUG_PRINTLN(result);
  } while ((count != 7) && (retries < 8));

  if (retries == 8) {
    // Es konnte nichts gelesen werden
    _seconds = 11;
    _minutes = 11;
    _hours = 11;
    _dayOfWeek = 1;
    _date = 1;
    _month = 1;
    _year = 14;
  }

  DEBUG_PRINT(F("Time: "));
  DEBUG_PRINT(getHours());
  DEBUG_PRINT(F(":"));
  DEBUG_PRINT(getMinutes());
  DEBUG_PRINT(F(":"));
  DEBUG_PRINTLN(getSeconds());
  DEBUG_FLUSH();
}

/**
   Die Uhrzeit aus den Variablen in die DS1307 schreiben
*/
void MyRTC::writeTime() {
  Wire.beginTransmission(_address);
  Wire.write((uint8_t) 0x00); // 0 to bit 7 starts the clock
  Wire.write(decToBcd(_seconds));
  Wire.write(decToBcd(_minutes));
  Wire.write(decToBcd(_hours)); // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(_dayOfWeek));
  Wire.write(decToBcd(_date));
  Wire.write(decToBcd(_month));
  Wire.write(decToBcd(_year));
  // Wire.write(0b00010000); // 1Hz Rechteck auf SQW fuer den Display-Update-Interrupt...
  Wire.endTransmission();
}

/**
   SQW fuer DS1307.
*/
void MyRTC::enableSQWOnDS1307() {
  Wire.beginTransmission(_address);
  Wire.write(0x07); // Datenregister
  Wire.write(0b00010000); // enable 1HZ square wave output
  Wire.endTransmission();
}

/**
   SQW fuer DS3231.
*/
void MyRTC::enableSQWOnDS3231() {
  Wire.beginTransmission(_address);
  Wire.write(0x0E); // Datenregister
  Wire.write(0b0000000); // enable 1HZ square wave output
  Wire.endTransmission();
}

/**
   Konvertierung Dezimal zu "Binary Coded Decimal"
*/
byte MyRTC::decToBcd(byte val) {
  return ((val / 10 * 16) + (val % 10));
}

/**
   Konvertierung "Binary Coded Decimal" zu Dezimal
*/
byte MyRTC::bcdToDec(byte val) {
  return ((val / 16 * 10) + (val % 16));
}

/**
   Aus einem String zwei Stellen als Zahl bekommen.
   (Geklaut von Jee-Labs.)
*/
uint8_t MyRTC::conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

/**
   Geklaut von Jee-Labs.
   A convenient constructor for using "the compiler's time":
   DateTime now (__DATE__, __TIME__);
   NOTE: using PSTR would further reduce the RAM footprint
*/
void MyRTC::set(const char* date, const char* time) {
  // sample input: date = "Dec 26 2009", time = "12:34:56"
  _year = conv2d(date + 9);
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  switch (date[0]) {
    case 'J':
      if (date[1] == 'a') {
        _month = 1;
      } else if (date[2] == 'n') {
        _month = 6;
      } else {
        _month = 7;
      }
      break;
    case 'F':
      _month = 2;
      break;
    case 'A':
      _month = date[2] == 'r' ? 4 : 8;
      break;
    case 'M':
      _month = date[2] == 'r' ? 3 : 5;
      break;
    case 'S':
      _month = 9;
      break;
    case 'O':
      _month = 10;
      break;
    case 'N':
      _month = 11;
      break;
    case 'D':
      _month = 12;
      break;
    default:
      ;
  }
  _date = conv2d(date + 4);
  _hours = conv2d(time);
  _minutes = conv2d(time + 3);
  _seconds = conv2d(time + 6);
}

//
// Setter/Getter
//

void MyRTC::setSeconds(byte seconds) {
  _seconds = seconds;
}

void MyRTC::setMinutes(byte minutes) {
  _minutes = minutes;
}

void MyRTC::incMinutes() {
  _minutes++;
  if (_minutes > 59) {
    _minutes = 0;
  }
}

void MyRTC::decMinutes() {
  if (_minutes == 0) {
    _minutes = 59;
  }
  else
  {
    _minutes--;
  }
}

void MyRTC::setHours(byte hours) {
  _hours = hours;
}

void MyRTC::incHours() {
  _hours++;
  if (_hours > 23) {
    _hours = 0;
  }
}

void MyRTC::decHours() {
  if (_hours == 0) {
    _hours = 23;
  }
  else {
    _hours--;
  }
}

void MyRTC::setDayOfWeek(byte dayOfWeek) {
  _dayOfWeek = dayOfWeek;
}

void MyRTC::setDate(byte date) {
  _date = date;
}

void MyRTC::setMonth(byte month) {
  _month = month;
}

void MyRTC::setYear(byte year) {
  _year = year;
}

byte MyRTC::getSeconds() {
  return _seconds;
}

byte MyRTC::getMinutes() {
  return _minutes;
}

unsigned int MyRTC::getMinutesOfDay() {
  return _minutes + 60 * _hours;
}

// Minuten des Tages ohne die Beruecksichtigung von 12/24 Stunden
// (fuer den Wecker)...

unsigned int MyRTC::getMinutesOf12HoursDay() {
  int h = _hours;
  while (h > 12) {
    h -= 12;
  }
  return _minutes + 60 * h;
}

byte MyRTC::getHours() {
  return _hours;
}

byte MyRTC::getDayOfWeek() {
  return _dayOfWeek;
}

byte MyRTC::getDate() {
  return _date;
}

byte MyRTC::getMonth() {
  return _month;
}

byte MyRTC::getYear() {
  return _year;
}
