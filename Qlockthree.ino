/**
   Clockthree
   Die Firmware der Selbstbau-QLOCKTWO.

   @mc       Arduino/RBBB (ATMEGA328)
   @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
   @version  3.4.8
   @created  1.11.2011
   @updated  15.3.2015

   Versionshistorie:
   V 1.1:   - DCF77 auf reine Zeit ohne Strings umgestellt.
            - Ganzes Datum wird in die DS1307 geschrieben (wieder verworfen).
   V 1.2:   - Schnelles Schreiben der Shift-Register eingefuehrt.
   V 1.3:   - Auf eigene DCF77-Bibliothek umgestellt (MyDCF77) und DCF77Helper verbessert.
   V 1.4:   - Glimmen/Nachleuchten dank Tipp von Volker beseitigt (http://www.mikrocontroller.net/topic/209988).
            - Erneute Fehlerbereinigung in der DCF77-Bibliothek.
   V 1.5:   - Die Helligkeit laesst sich ueber die Variable 'brightness' einstellen (Bereich 1-9) bzw.
                ueber den Compiler-Schalter 'ENABLE_LDR' die Regelung einschalten.
   V 1.6:   - Fehler in der DCF77Helper-Bibliothek behoben.
   V 1.7:   - SKIP_BLANK_LINES eingefuehrt, damit hellere LEDs (Danke an Johannes).
   V 1.8:   - UPSIDE_DOWN eingefuehrt, damit man die Kabel beim Anoden-Multiplexer nicht kreuzen muss.
   V 1.9:   - Diverse Verbesserungen und Fehlerbehebungen, die das Flackern drastisch reduzieren.
   V 1.9.1: - Fehler behoben, der die Nutzung eines analogen Pins fuer den DCF77-Empfaenger verhindert hat.
   V 1.9.2: - Timing-Parameter fuer die PWM ueber DEFINE anpassbar.
            - LDR-Grenzwerte automatisch anpassbar.
            - Schreibfehler in setHoures()->setHours() behoben.
            - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
   V 1.9.3: - Glimmen wieder behoben.
   V 1.9.4: - Modus zum Einstellen der Helligkeit eingefuehrt.
   V 2.0:   - Sprachrelevante Dinge ausgelagert, so kann man mehr Sprachen haben und einfach durch einkommentieren aktivieren.
            - setWords in setMinutes und setMinutes in setCorners umbenannt, weil es mehr Sinn ergibt.
            - setCorners in eigene Datei ausgelagert, weil viele Bastler sich vertun und in der Routine Aenderungen vornehmen muessen.
            - LDR in eigene Klasse ausgelagert und Werte geglaettet. Am Anfang werden 1000 Werte gelesen, damit er sich einpegelt.
            - Signal vom DCF77-Empfaenger geglaettet, damit nicht ein einzelner falscher Peak das Telegramm zerstoert.
            - Status-LEDs koennen ueber DEFINEs ausgeschaltet werden.
            - DCF77- und SQW-LED blinken am Anfang drei Mal als 'Hello', damit man ohne Serial-Monitor sehen kann, ob der ATMEGA anlaeuft.
            - Serial-Output ist immer an, kleine Statusmeldung waehrend der Initialisierung, damit man beim Bespielen des ATMEGA ohne weitere
                Elektronik sehen kann, ob das Programm drauf ist und laeuft.
            - Aenderung, die ein Stellen der Zeit nach dem DCF77-Signal auch im Modus 'BLANK' ermoeglicht - in diesem Modus ist der Empfang
                weniger gestoert, da die LED-Matrix abgeschaltet ist.
            - Zeitgesteuertes Abschalten des Displays eingefuehrt (Stromsparen in der Nacht/Schlafzimmer/besserer Empfang des DCF77-Empfaengers).
            - Speaker auf D13 eingefuehrt.
            - Alarm-Mode eingefuehrt.
            - Bayrische Sprachvariante (DE_BA): 'viertel nach Zehn', aber 'dreiviertel Elf'.
   V 2.0.1: - DCF77-Tresholds angepasst.
   V 2.0.2: - Strings in PROGMEM (flash) ausgelagert.
   V 2.0.3: - SPEAKER_IS_BUZZER eingefuehrt, damit kann man sagen, ob man einen Lautsprecher oder Piezo-Buzzer als Alarm verwendet.
   V 2.0.4: - falsches BREAK dank Lars behoben.
   V 2.0.5: - Franzoesisch hinzugefuegt, Woerter_??.h's anschaulicher gemacht. Weitere Sprachen angelegt, aber noch nicht die Logik implementiert (Hilfe?!).
   V 2.0.6: - cleanWordsForAlarmSettingMode() eingefuehrt, damit das Stellen des Alarms sprachenunabhaengig ist.
            - Das DCF77-Signal kann per Compiler-Schalter invertiert werden.
   V 2.0.7: - Neuer Modus: Umschaltung LDR auto/manuell dank Alexander.
   V 2.0.8: - Genauigkeit verbessert, dank Peter (Interrupt auf FALLING). @Peter: Das Zurueckscheiben in die DS1307 passiert im Normalbetrieb ja
                nur beim Update ueber den DCF77-Empfaenger, daher habe ich es weggelassen.
   V 2.0.9: - SplitSideDown rausgenommen, diesen seltenen Fehler braucht kein Mensch.
   V 2.1:   - Neue Sprachen hinzugefuegt: Englisch und Italiensch. Spanisch ist angelegt, aber nicht implementiert, weil ich kein Wort Spanisch spreche.
            - Man kann jetzt einfach relativ weit oben im Code ueber Compiler-Schalter festlegen, welche Sprache man haben moechte.
   V 2.1.1: - Bitmaps in den PROGMEM ausgelagert. So kann man alle Buchstaben halten.
   V 2.1.2: - Bug im Alaram-Modus behoben.
   V 2.1.3: - Zusaetzlich zu den Ziffern auch Staben eingefuehrt, A/M-Modus darauf umgestellt (Vorbereitung auf Konfigurationsmoeglichkeit
                fuer alle Sprachen in einer Firmware)
   V 2.1.4: - Optimierung hinsichtlich Speicherbedarf.
   V 3.0:   - LedDriver-Klasse erstellt, welche die Shift-Register und UDN2981A als Led-Treiber kapseln. Dadurch kann man auch andere
                LED-Treiber nehmen (MAX7219, AS1130 etc.) und einfach eine passende Klasse von der LedDriver-Klasse vererben und in den Code einklinken.
            - Alle Sprachen im 'Renderer' zusammengefasst, so dass sie ohne die Firmware neu zu flashen umschaltbar werden.
            - Niederlaendisch hinzugefuegt (Danke an Wekog24 und Rudolf K.).
            - Eine kleine Debug-Klasse eingefuehrt, damit die #debug-Statements nicht so stark den Code zerhacken und er besser lesbar wird.
            - Beim Starten wird der freie Speicher und die Version auf der Console ausgegeben.
            - Eine Alarm-Klasse eingefuehrt, das macht den 'Hauptcode' nochmal besser lesbar.
            - Configuration.h eingefueht, in der alle wichtigen Compiler-Schalter zusammengefuehrt sind, dadurch gibt es eine zentrale Stelle
                fuer die Anpassungen (Danke fuer das Konzept an Markus K.).
            - Settings.h eingefuehrt, in der die Spracheinstellung gehalten wird und im EEPROM gespeichert wird (Danke fuer das Konzept an Markus K.).
            - Modes zum Einstellen der Sprache und des Eck-Modus eingefuehrt.
            - Extended Modes eingefuehrt.
            - Saechsisch als Sprache aufgenommen, danke an Sven.
            - Bug im setzten der Eck-LEDs fuer den Ueberpixel behoben.
            - Brightness ist normaler Mode, aber nur wenn ldr_auto == FALSE.
            - Automatische Alarm-Abschaltung nach MAX_BUZZ_TIME_IN_MINUTES.
            - Stellen der Uhr nur im erweiterten Modus moeglich (sonst verstellt man sie leicht bei der Verwendung als Wecker).
            - LDR-Modus wird im EEPROM abgelegt.
            - kleinere Korrekturen, die nur in erweiterten Compiler-Ausgaben zu Warnings gefuehrt haben.
            - Kleinere Optimierungen, Danke an Michael K.
            - Bug aufgrund eines moeglichen Ueberlaufs von millis() in Button.cpp behoben.
            - Verbesserungen in der Helligkeitssteuerung.
            - ENABLE_LDR aus Configuration.h entfernt, da sich der LDR ueber das erweiterte Setup ausschalten laesst.
   V 3.0.1: - Schweizer Layout angepasst.
   V 3.1:   - Spanisch implementiert.
            - Der LDR skaliert jetzt selbst, dann flackert es weniger bei unguenstigen Lichtverhaeltnissen.
   V 3.1.1. - Die Displayhelligkeit wird beim starten gesetzt.
            - Kleiner Fehler im Spanischen behoben.
            - Die alte Shiftregistermethode ist wieder aktiv, die schnelle kann in Configuration.h gesetzt werden.
   V 3.1.2. - Led-Driver-Klassen sagen, wer sie sind (fuer die Ausgabe der Konfiguration beim Start).
            - Klassen-Namen verbessert.
   V 3.2.   - Led-Driver fuer das Licht-Monster implementiert.
            - LDR-Klasse auf 0-100% umgebaut.
            - Brightness wird im EEPROM gesichert.
            - EnableAlarm und DCF77SignalIsInverted ins EEPROM ausgelagert und ueber das erweiterte Setup einstellbar.
            - Die Modi SCRAMBLE und ALL entfernt, dafuer den Modus TEST eingefuehrt. ALL ist fuer manche DisplayDriver gefaehrlich wegen des Stromverbrauchs. TEST schalte nacheinander alle LEDs einmal durch.
            - Moeglichkeit zur Zeitverschiebung eingefuehrt.
   V 3.2.1. - Alle Deutsch-Varianten zusammengefasst, um Platz zu sparen.
            - Fehler im Italienischen behoben.
            - Fehler mit Doppelbelegung einer Variable im Qlockthree.ino behoben.
            - Der LDR wird nur alle 250ms gelesen, dann flackert er weniger bei unguenstigen Verhaeltnissen.
   V 3.2.2. - DS1307 Multi-MCU-Faehig gemacht.
            - Initialisierung der DS1307 verbessert.
   V 3.2.3. - A0-Hack: Verbindet man die freie A0-Leitung mit PON vom DCF77-Empfaenger, schaltet diese den Empfaenger korrekt
                ein und aus, je nach Displayzustand. Ist PON fest mit GND verbunden, passiert nichts (schadet aber auch  nicht).
   V 3.2.4. - Der DCF-Empfaenger wird ueber A0 am Anfang eingeschaltet, dann nicht mehr ausgeschaltet.
            - Wird in den Blank-Modus geschaltet (Display aus), schaltet sich nach dem erfolgreichen Empfang der Zeit ueber DCF77
                das Display wieder ein (hilft beim Debuggen).
            - Der Nachtabschaltmodus ist ein eigener, nicht manuell erreichbarer Modus.
   V 3.3.0. - LedDriverNeoPixel fuer den Aufbau mit WS2812B-Streifen eingefuehrt (Verkabelungsschema ist beim LedDriverNeoPixel)...
            - ...dazu die Klasse Button angepasst, so dass sie auch gegen GND schalten kann (damit Unterstuetzung fuer das BBRTCAD).
            - Achtung! Wenn die NeoPixel-Library im Librarys-Ordner ist, kann ich keinen Pointer auf den Stripe (in LedDriverNeoPixel.h)
                definieren (ich weiss leider nicht warum, es kommt eine seltsame Compiler-Meldung). Wenn sie im Qlockthree-Ordner ist, geht es.
            - IR-Fernbedienungs-Unterstuetzung eingefuehrt. Achtung! Wegen Platzmangels alle IRs ausser der Sparkfun (NEC) auskommentiert. Siehe weitere
                Infos weiter unten und bei der IR-Fernbedienung!
            - Achtung! Weil ich die IR-Receiver-Library (https://github.com/shirriff/Arduino-IRremote/) entschlacken musste, ist sie hier
                im Qlockthree-Ordner. Wenn sie auch im Librarys-Ordner ist, schlaegt die Compilierung fehl! Wer andere Codes als die von der
                SparkFun-Fernbedienung (NEC) nehmen mechte, muss die entsprechenden Typen in der Datei IRremote.cpp so ab Zeile 427 wieder
                einkommentieren. Die Implementierung hier ist beispielhaft zu sehen, um eigene Ideen verwirklichen zu koennen.
            - Test-Modus geaendert, damit er mit der IR-Fernbedienung klappt. Die Eck-LEDs zaehlen im Sekunden-Takt hoch, die Spalten werden
                im Sekunden-Takt nach rechts durchgeschaltet.
            - DCF-Empfang verbessert, dank Tipp aus dem Forum (unguenstiger INTERNAL_PULLUP beim Signal-Pin; PULL-Widerstaende muessen in Hardware
                je nach verwendetem Empfaenger realisiert werden).
            - Flackern in Ext-Menues und Manual-Brightness behoben, falsch durchdachtes linesToWrite-Statement (interessiert nur DefaultLedDriver,
                alle anderen ignorieren es eh).
            - Fehler in der DS1307-Klasse behoben. Nicht immer kommen die angeforderten 7 Bytes, dann verwerfen und neu anfordern (und nach
                8 Versuchen abbrechen). Der I2C-Bus ist wohl speziell bei LED-Stripes gerne mal gestoert, dann kommen die beruehmten '85' Sekunden.
            - Mode fuer die Visualisierung des DCF77-Empfangs (im Ext-Menue nach dem LED-Test) eingefuehrt. Man kann damit abschaetzen, wie gut der
                DCF77-Empfang ist. Mit der gelben LED muessen die Ecken durchschalten. Schalten sie wild durcheinander, gibt es Fehlsignale.
                Das Problem ist, dass ja die LED-Matrix den Empfang stoert. So stoert natuerlich potentiell auch die Visualisierung den Empfang.
                Mit einem WortWecker hat das dennoch ganz gut geklappt. Mit dem experimentellen LedDriverNeoPixel nicht.
            - Kleinen Fehler im Italienischen behoben, danke an die diversen Hinweisgeber!
   V 3.3.1. - Stundenbegrenzung im Renderer (die ja wegen der Zeitverschiebungsmoeglichkeit existiert) auf den Bereich 0 <= h <= 24 ausgeweitet,
                dank Tipp aus dem Forum.
   V 3.4.0. - Die DCF77-Decoding-Klasse vollstaendig umgestellt. Alle Beispiele im Internet gehen davon aus, dass man keine Vergleichszeit hat.
                Da wir hier eine Echtzeituhr haben, kann man statistische Methoden zur Auswertung verwenden. Wenn das Signal optisch brauchbar aussieht (gelbe LED),
                Synchronisiert die Zeit in weniger als 4 Minuten. Aber es muss eben brauchbar aussehen. Kurzes Aufblinken 1 Mal pro Sekunde, eine
                Pause pro Minute.
   V 3.4.1. - Die Adafruit- und IR-Library umbenannt ("My" vorangestellt), damit sie nicht mit den offiziellen Librarys kollidieren. Warum sie hier lokal liegen,
                steht weiter oben.
   V 3.4.2. - LDR-Bugs behoben und -Klasse optimiert. Dazu auch Getter fuer die Helligkeit in den Display-Drivern eingefuehrt.
   V 3.4.3. - Vergessene DEBUGs auskommentiert, Check der DS1307 beim Start verbessert (Danke an Erich M.).
   V 3.4.4. - Verbesserungen in der RTC (setzten der Zeit durch Compile-Zeit moeglich).
            - Adafruits NeoPixel-Library muss jetzt im Librarys-Ordner liegen.
   V 3.4.5. - DS1307 nach MyRTC umbenannt, weil es nicht mehr nur um die DS1307 geht.
            - Einschalten des Rechtecksignals fuer DS1307 und DS3231 getrennt (Danke an Erich M.) - Konfiguration in der Configuration.h.
            - Unterstuetzung verschiedener IR-Fernbedienungen vereinfacht. Codes von NEC-compatiblen Fernbedienungen werden angezeigt, wenn eine Fernbedienung in
                der Configuration.h eingeschaltet ist (muss nicht die richtige sein) und #DEBUG an ist. Drei Beispiel-Remotes sind angelegt: Sparkfun,
                Mooncandles, Lunartec. Bei der Verwendung von Nicht-NEC-Fernbedienungen muss der entsprechende Part in MyIRremote.cpp einkommentiert werden,
                ein guter Start ist Zeile 428.
            - Im Zusammenhang mit den Fernbedienungen setColor() fuer die LED-Driver eingefuehrt. Damit kann man die Farben von RGB-Stripes aendern. Bei
                dem klassischen Aufbau mit einfarbeigen LEDs (75HC595; MAX7219 etc.) passiert natuerlich nichts.
            - Unterstuetzung fuer APA102-Stripes (DotStars) eingefuehrt, die Adafruit APA102-Library muss dazu im librarys-Ordner liegen.
            - FPS werden bei eingeschaltetem #DEBUG angezeigt.
            - Die Helligkeitsregelung laeuft jetzt sanfter. Timing-Werte fuer die verschiedenen LedDriver in der Configuration.h hinterlegt.
   V 3.4.6. - Unterstuetzung fuer LPD8806-Stripes eingefuehrt, die Adafruit LPD8806-Library muss dazu im librarys-Ordner liegen. Achtung! Zur Zeit hat die LPD8806-Library
                von Adafruit einen Bug. Es fehlt das klassische #ifndef __CLASSNAME__ #define __CLASSNAME__ [Klasssendefinition] #endif um die Klasse im .h-File.
   V 3.4.7. - Unterstuetzung fuer Buttons auf rein analoge Ports hinzugefuegt, wie sie der ATMEGA328 in SMD-Ausfuehrung hat.
            - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt, da sich die Firmware damit eh nicht mehr kompilieren laesst.
            - Library fuer MAX7219 (LedControl) ausgelagert, sie muss jetzt im Librarys-Ordner liegen.
   V 3.4.8. - HelperSeconds-Behandlung in Interrupt-Funktion verschoben, damit die nicht aufgrund von Tastendruecken hochgezaehlt werden, danke an Meikel.
*/
#include <Wire.h> // Wire library fuer I2C
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <SPI.h>
#include "Configuration.h"
#include "LedDriver.h"
#include "LedDriverDefault.h"
#include "LedDriverUeberPixel.h"
#include "LedDriverPowerShiftRegister.h"
#include "LedDriverNeoPixel.h"
#include "LedDriverDotStar.h"
#include "LedDriverLPD8806.h"
#include "IRTranslator.h"
#include "IRTranslatorSparkfun.h"
#include "IRTranslatorMooncandles.h"
#include "IRTranslatorLunartec.h"
#include "IRTranslatorCLT.h"
#include "MyIRremote.h"
#include "MyRTC.h"
#include "MyDCF77.h"
#include "Button.h"
#include "AnalogButton.h"
#include "LDR.h"
#include "DCF77Helper.h"
#include "Renderer.h"
#include "Staben.h"
#include "Alarm.h"
#include "Settings.h"
#include "Zahlen.h"
#include "Modes.h"
#include "MyTempSens.h"

#ifdef EVENTS
#include "Events.h"
#endif


#define FIRMWARE_VERSION "CLT OS V2.0, based on Qlockthree by Christian Ashoff"

/*
   Den DEBUG-Schalter gibt es in allen Bibiliotheken. Wird er eingeschaltet, werden ueber den
   'Serial-Monitor' der Arduino-Umgebung Informationen ausgegeben. Die Geschwindigkeit im
   Serial-Monitor muss mit der hier angegeben uebereinstimmen.
   Default: ausgeschaltet
*/
// #define DEBUG
#include "Debug.h"
// Die Geschwindigkeit der seriellen Schnittstelle. Default: 57600. Die Geschwindigkeit brauchen wir immer,
// da auch ohne DEBUG Meldungen ausgegeben werden!
#define SERIAL_SPEED 57600


/*
   Die persistenten (im EEPROM gespeicherten) Einstellungen.
*/
Settings settings;

/**
   Der Renderer, der die Woerter auf die Matrix ausgibt.
*/
Renderer renderer;

/**
   Der LED-Treiber fuer 74HC595-Shift-Register. Verwendet
   von der Drei-Lochraster-Platinen-Version und dem
   NachBau_V02-Board sowie dem WortWecker nach Christian.

   Data: 10; Clock: 12; Latch: 11; OutputEnable: 3
   LinesToWrite: 10
*/
#if defined (LED_DRIVER_DEFAULT)
LedDriverDefault ledDriver(10, 12, 11, 3, 10);

#define PIN_MODE 7
#define PIN_M_PLUS 5
#define PIN_H_PLUS 6

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER A1

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON A0

#define PIN_SQW_LED 4
#define PIN_DCF77_LED 8

#define PIN_SPEAKER 13

#define PIN_TEMP_SENS A2

/**
   Der LED-Treiber fuer 4 MAX7219-Treiber wie im Ueberpixel.
   Data: 10; Clock: 11; Load: 12
*/
#elif defined (LED_DRIVER_UEBERPIXEL)
LedDriverUeberPixel ledDriver(5, 6, 7);

#define PIN_MODE 8
#define PIN_M_PLUS 3
#define PIN_H_PLUS 4

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER A1

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON A0

#define PIN_SQW_LED 10
#define PIN_DCF77_LED 11

#define PIN_SPEAKER 13

#define PIN_TEMP_SENS A2

/**
   Der LED-Treiber fuer Power-Shift-Register.
   Data: 10; Clock: 11; Load: 12
*/
#elif defined (LED_DRIVER_POWER_SHIFT_REGISTER)
LedDriverPowerShiftRegister ledDriver(10, 12, 11, 3);

#define PIN_MODE 7
#define PIN_M_PLUS 5
#define PIN_H_PLUS 6

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER A1

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON A0

#define PIN_SQW_LED -1
#define PIN_DCF77_LED -1

#define PIN_SPEAKER -1

#define PIN_TEMP_SENS A2

/**
   Der LED-Treiber fuer NeoPixel-Stripes...
*/
#elif defined (LED_DRIVER_NEOPIXEL)
/**
   ...an einem BBRTCAD.
   Data: 6
*/
#ifdef BOARD_BBRTCAD
LedDriverNeoPixel ledDriver(6);

#define PIN_MODE 11
#define PIN_M_PLUS 13
#define PIN_H_PLUS 12

#define BUTTONS_PRESSING_AGAINST LOW

#define PIN_IR_RECEIVER A1

#define PIN_LDR A0
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 3

#define PIN_DCF77_PON 4

#define PIN_SQW_LED 9
#define PIN_DCF77_LED 10

#define PIN_SPEAKER -1

#define PIN_TEMP_SENS A2

/**
* ...an einer CLT.
* Data: 13
*/
#elif defined(BOARD_CLT)
LedDriverNeoPixel ledDriver(13);

#define PIN_MODE 7
#define PIN_M_PLUS 5
#define PIN_H_PLUS 6

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER 10

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON -1

#define PIN_SQW_LED 4
#define PIN_DCF77_LED 8

#define PIN_SPEAKER -1

#define PIN_TEMP_SENS A0

/**
 * ...an einem AMBBRTCB V1.0
 * Data: 0
 */
#elif defined(BOARD_AMBBRTCAD)
LedDriverNeoPixel ledDriver(0);

#define PIN_MODE 5
#define PIN_M_PLUS 7
#define PIN_H_PLUS 6

#define BUTTONS_PRESSING_AGAINST LOW

#define PIN_IR_RECEIVER A1

#define PIN_LDR A0                
#define IS_INVERTED false

#define PIN_SQW_SIGNAL 10
#define PIN_DCF77_SIGNAL 3

#define PIN_DCF77_PON -1

#define PIN_SQW_LED 1
#define PIN_DCF77_LED 2

#define PIN_SPEAKER -1

#define PIN_TEMP_SENS -1
#endif

/**
   Der LED-Treiber fuer DotStars-Stripes an einem BBRTCAD.
   Data: 6
   Clock: 7
*/
#elif defined (LED_DRIVER_DOTSTAR)
LedDriverDotStar ledDriver(6, 7);

#define PIN_MODE 11
#define PIN_M_PLUS 13
#define PIN_H_PLUS 12

#define BUTTONS_PRESSING_AGAINST LOW

#define PIN_IR_RECEIVER A1

#define PIN_LDR A0
#define IS_INVERTED false

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 3

#define PIN_DCF77_PON 4

#define PIN_SQW_LED 9
#define PIN_DCF77_LED 10

#define PIN_SPEAKER -1

#define PIN_TEMP_SENS -1

/**
  Der LED-Treiber fuer LPD8806-Stripes...
*/
#elif defined (LED_DRIVER_LPD8806)
/**
 * ...an einem BBRTCAD.
 * Data: 6
 * Clock: 7
 */
#ifdef BOARD_BBRTCAD
LedDriverLPD8806 ledDriver(6, 7);

#define PIN_MODE 11
#define PIN_M_PLUS 13
#define PIN_H_PLUS 12

#define BUTTONS_PRESSING_AGAINST LOW

#define PIN_IR_RECEIVER A1

#define PIN_LDR A0
#define IS_INVERTED false

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 3

#define PIN_DCF77_PON 4

#define PIN_SQW_LED 9
#define PIN_DCF77_LED 10

#define PIN_SPEAKER -1

#define PIN_TEMP_SENS -1

/**
  ...an einer CLT.
  Data: 13
  Clock: 11
*/
#elif defined(BOARD_CLT)
LedDriverLPD8806 ledDriver(13, 11);

#define PIN_MODE 7
#define PIN_M_PLUS 5
#define PIN_H_PLUS 6

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER 10

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON -1

#define PIN_SQW_LED 4
#define PIN_DCF77_LED 8

#define PIN_SPEAKER -1

#define PIN_TEMP_SENS -1
#endif

#endif

/**
   Der IR-Remote-Control-Receiver.
*/
#ifndef REMOTE_NO_REMOTE
IRrecv irrecv(PIN_IR_RECEIVER);
decode_results irDecodeResults;
#if defined (REMOTE_SPARKFUN)
IRTranslatorSparkfun irTranslator;
#elif defined (REMOTE_MOONCANDLES)
IRTranslatorMooncandles irTranslator;
#elif defined (REMOTE_LUNARTEC)
IRTranslatorLunartec irTranslator;
#elif defined (REMOTE_CLT)
IRTranslatorCLT irTranslator;
#endif
#endif

#ifdef REMOTE_BLUETOOTH
IRTranslatorCLT irTranslatorBT;
#endif

// Ueber die Wire-Library festgelegt:
// Arduino analog input 4 = I2C SDA
// Arduino analog input 5 = I2C SCL
// Werden zur Kommunikation mit der
// RTC verwendet.
/**
   Die Real-Time-Clock mit der Status-LED fuer das SQW-Signal.
*/
MyRTC rtc(0x68, PIN_SQW_LED);
volatile byte helperSeconds;

/**
   Der Funkempfaenger (DCF77-Signal der PTB Braunschweig).
*/
MyDCF77 dcf77(PIN_DCF77_SIGNAL, PIN_DCF77_LED);
DCF77Helper dcf77Helper;

/**
 * Tempsensor
 */
#ifndef TEMP_SENS_NONE
  MyTempSens tempSens(PIN_TEMP_SENS);
#endif

/**
   Variablen fuer den Alarm.
*/
#ifdef ALARM
Alarm alarm(PIN_SPEAKER);
#endif

/**
   Der Helligkeitssensor
*/
LDR ldr(PIN_LDR, IS_INVERTED);
unsigned long lastBrightnessCheck;

/**
   Die Helligkeit zum Anzeigen mit den Balken.
*/
byte brightnessToDisplay;

/**
   Die Tasten.
*/
Button minutesPlusButton(PIN_M_PLUS, BUTTONS_PRESSING_AGAINST);
Button hoursPlusButton(PIN_H_PLUS, BUTTONS_PRESSING_AGAINST);
Button extModeDoubleButton(PIN_M_PLUS, PIN_H_PLUS, BUTTONS_PRESSING_AGAINST);
Button modeChangeButton(PIN_MODE, BUTTONS_PRESSING_AGAINST);

// Startmode...
Mode mode = STD_MODE_NORMAL;
// Merker fuer den Modus vor der Abschaltung...
Mode lastMode = mode;

// Die Matrix, eine Art Bildschirmspeicher
word matrix[16];

// Hilfsvariable, da I2C und Interrupts nicht zusammenspielen
volatile boolean needsUpdateFromRtc = true;

// Fuer den Bildschirm-Test
byte testColumn;

// Fuer fps-Anzeige
#ifdef DEBUG
word frames = 0;
unsigned long lastFpsCheck = 0;
#endif

#ifdef USE_EXT_MODE_DCF_SYNC
// Fuer die DCF_DEBUG Anzeige
unsigned int dcf77ErrorMinutes;
#endif

// Zähler für fall back timer im Menu
byte fallBackCounter = 0;

// Indiziert, ob Event aktiv ist.
bool evtActive = false;

/**
   Aenderung der Anzeige als Funktion fuer den Interrupt, der ueber das SQW-Signal
   der Real-Time-Clock gesetzt wird. Da die Wire-Bibliothek benutzt wird, kann man
   den Interrupt nicht direkt benutzen, sondern muss eine Hilfsvariable setzen, die
   dann in loop() ausgewertet wird.
*/
void updateFromRtc() {
  needsUpdateFromRtc = true;

  helperSeconds++;

  if (fallBackCounter > 0) {
    if( mode != STD_MODE_BLANK ){
      updateFallBackCounter();
    }
  }

#ifndef TEMP_SENS_NONE
  tempSens.takeSample();
#endif
}

/**
   Den freien Specher abschaetzen.
   Kopiert von: http://playground.arduino.cc/Code/AvailableMemory
*/
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

/**
   Initialisierung. setup() wird einmal zu Beginn aufgerufen, wenn der
   Arduino Strom bekommt.
*/
void setup() {
  Serial.begin(SERIAL_SPEED);
  // Set Timeout of 50ms for Serial.parseInt()
  Serial.setTimeout(50);
  DEBUG_PRINTLN(F("Qlockthree is initializing..."));
  DEBUG_PRINTLN(F("... and starting in debug-mode..."));
  DEBUG_FLUSH();

  // DCF konfigurieren
  pinMode(PIN_DCF77_PON, OUTPUT);
  enableDcf(false);

#ifdef ALARM
  if (settings.getEnableAlarm()) {
    // als Wecker Display nicht abschalten...
    TimeStamp offTime(0, 0, 0, 0, 0, 0);
    TimeStamp onTime(0, 0, 0, 0, 0, 0);
  }
#endif

  // LED-Treiber initialisieren
  ledDriver.init();
  // Inhalt des Led-Treibers loeschen...
  ledDriver.clearData();
  // und Inhalt des Bildspeichers loeschen
  renderer.clearScreenBuffer(matrix);
  // wir brauchen nur 10 Zeilen...
  ledDriver.setLinesToWrite(10);

  // starte Wire-Library als I2C-Bus Master
  Wire.begin();

  // RTC-Interrupt-Pin konfigurieren
  pinMode(PIN_SQW_SIGNAL, INPUT);
  digitalWrite(PIN_SQW_SIGNAL, HIGH);

  // DCF77-LED drei Mal als 'Hello' blinken lassen
  // und Speaker piepsen kassen, falls ENABLE_ALARM eingeschaltet ist.
  for (byte i = 0; i < 3; i++) {
    dcf77.statusLed(true);
#ifdef ALARM
    if (settings.getEnableAlarm()) {
      alarm.buzz(true);
    }
#endif
    delay(100);
    dcf77.statusLed(false);
#ifdef ALARM
    if (settings.getEnableAlarm()) {
      alarm.buzz(false);
    }
#endif
    delay(100);
  }

  DEBUG_PRINT(F("Compiled: "));
  DEBUG_PRINT(F(__TIME__));
  DEBUG_PRINT(F(" / "));
  DEBUG_PRINTLN(F(__DATE__));

  // RTC starten...
  rtc.readTime();
  if ((rtc.getSeconds() >= 60) || (rtc.getMinutes() >= 60) || (rtc.getHours() >= 24) || (rtc.getYear() < 15)) {
    // Echtzeituhr enthaelt Schrott, neu mit erkennbaren Zahlen beschreiben...
    DEBUG_PRINT(F("Resetting RTC..."));
    rtc.set(11, 11, 1, 1, 1, 15);
    rtc.setSeconds(11);
  }

#ifdef TEMP_SENS_LM35
  DEBUG_PRINTLN(F("Temperatursensortyp ist LM35."));
  tempSens.initLM35();
#elif defined TEMP_SENS_LM335
  DEBUG_PRINTLN(F("Temperatursensortyp ist LM335."));
  tempSens.initLM335();
#endif

#ifdef DS1307
  DEBUG_PRINTLN(F("Uhrentyp ist DS1307."));
  rtc.enableSQWOnDS1307();
#elif defined DS3231
  DEBUG_PRINTLN(F("Uhrentyp ist DS3231."));
  rtc.enableSQWOnDS3231();
#else
  Definition_des_Uhrtyps_fehlt!
  In der Configuration.h muss der Uhrentyp angegeben werden!
#endif

  rtc.writeTime();
  helperSeconds = rtc.getSeconds();
  DEBUG_PRINT(F("RTC-Time: "));
  DEBUG_PRINT(rtc.getHours());
  DEBUG_PRINT(F(":"));
  DEBUG_PRINT(rtc.getMinutes());
  DEBUG_PRINT(F(":"));
  DEBUG_PRINT(helperSeconds);
  DEBUG_PRINT(F(" RTC-Date: "));
  DEBUG_PRINT(rtc.getDate());
  DEBUG_PRINT(F("."));
  DEBUG_PRINT(rtc.getMonth());
  DEBUG_PRINT(F("."));
  DEBUG_PRINTLN(rtc.getYear());

  // Den Interrupt konfigurieren,
  // nicht mehr CHANGE, das sind 2 pro Sekunde,
  // RISING ist einer pro Sekunde, das reicht.
  // Auf FALLING geandert, das signalisiert
  // den Sekundenwechsel, Danke an Peter.
  attachInterrupt(0, updateFromRtc, FALLING);

  // Werte vom LDR einlesen und vermuellen, da die ersten nichts taugen...
  for (int i = 0; i < 1000; i++) {
    analogRead(PIN_LDR);
  }

  // rtcSQWLed-LED drei Mal als 'Hello' blinken lassen
  // und Speaker piepsen kassen, falls ENABLE_ALARM eingeschaltet ist.
  for (byte i = 0; i < 3; i++) {
    rtc.statusLed(true);
#ifdef ALARM
    if (settings.getEnableAlarm()) {
      alarm.buzz(true);
    }
#endif
    delay(100);
    rtc.statusLed(false);
#ifdef ALARM
    if (settings.getEnableAlarm()) {
      alarm.buzz(false);
    }
#endif
    delay(100);
  }

  // ein paar Infos ausgeben
  DEBUG_PRINTLN(F("... done and ready to rock!"));

  Serial.print(F("Version: "));
  Serial.println(FIRMWARE_VERSION);

  DEBUG_PRINT(F("Driver: "));
  ledDriver.printSignature();

#ifndef REMOTE_NO_REMOTE
  DEBUG_PRINT(F("Remote: "));
  irTranslator.printSignature();
  irrecv.enableIRIn();
#else
  DEBUG_PRINTLN(F("Remote: disabled."));
#endif
#ifdef ALARM
  if (settings.getEnableAlarm()) {
    DEBUG_PRINTLN(F("Alarm is enabled"));
  }
#endif

  if (settings.getDcfSignalIsInverted()) {
    DEBUG_PRINTLN(F("DCF77-Signal is inverted."));
  }

  DEBUG_PRINT(F("Free ram: "));
  DEBUG_PRINT(freeRam());
  DEBUG_PRINTLN(F(" bytes."));
  DEBUG_FLUSH();

  // DCF77-Empfaenger einschalten...
  enableDcf(true);
  // Display einschalten...
  ledDriver.wakeUp();
  ledDriver.setBrightness(settings.getBrightness());

#ifdef LED_TEST_INTRO_ENABLED
  unsigned long initMillis = millis();
  while((millis() - initMillis) < 5000){
    renderer.setAllScreenBuffer(matrix);
    ledDriver.writeScreenBufferToMatrix(matrix, true, eColors::color_white);
  }
#endif
}

/**
   loop() wird endlos auf alle Ewigkeit vom Microcontroller durchlaufen
*/
void loop() {
  //
  // FPS
  //
#ifdef DEBUG
  frames++;
  if (lastFpsCheck > millis()) {
    // wir hatten einen Ueberlauf...
    lastFpsCheck = millis();
  }
  if (lastFpsCheck + 1000 < millis()) {
    DEBUG_PRINT("FPS: ");
    DEBUG_PRINTLN(frames);
    lastFpsCheck = millis();
    frames = 0;
  }
#endif

  //
  // Dimmung.
  //
  if (settings.getUseLdr()) {
    if (millis() < lastBrightnessCheck) {
      // wir hatten einen Ueberlauf...
      lastBrightnessCheck = millis();
    }
    if (lastBrightnessCheck + LDR_CHECK_RATE < millis()) { // langsam nachsehen...
      byte lv = ldr.value();
      if (ledDriver.getBrightness() > lv) {
        ledDriver.setBrightness(ledDriver.getBrightness() - 1);
      } else if (ledDriver.getBrightness() < lv) {
        ledDriver.setBrightness(ledDriver.getBrightness() + 1);
      }
      lastBrightnessCheck = millis();
    }
  }

  //
  // needsUpdateFromRtc wird via Interrupt gesetzt ueber fallende
  // Flanke des SQW-Signals von der RTC.
  // Oder falls eine Tasten-Aktion eine sofortige Aktualisierung des Displays braucht.
  //
  if (needsUpdateFromRtc) {
    needsUpdateFromRtc = false;

    //
    // Zeit einlesen... 
    // ... nur alle 60 Sekunden, um die Minuten und evtl. Stunden zu aktualisieren.
    //
    if (helperSeconds > 59) {
      rtc.readTime();
      helperSeconds = rtc.getSeconds();
    }         

    //
    // Bildschirmpuffer beschreiben...
    //
    switch (mode) {
      case STD_MODE_NORMAL:
        // Event Abfrage
#ifdef EVENTS
        for (byte evtID = 0; evtID < nbrOfEvts; evtID++) {
          if ((rtc.getDate() == events[evtID].getDate()) & (rtc.getMonth() == events[evtID].getMonth())) {
            switch (settings.getEvent()) {
              case 0:
                while (!(rtc.getMinutes() % 5)) {
                  evtActive = true;
                  events[evtID].show();
                  rtc.readTime();
                }
                break;
              case 1:
                while (!(rtc.getMinutes() % 15)) {
                  evtActive = true;
                  events[evtID].show();
                  rtc.readTime();
                }
                break;
              case 2:
                while (!(rtc.getMinutes() % 30)) {
                  evtActive = true;
                  events[evtID].show();
                  rtc.readTime();
                }
                break;
              case 3:
                while (!(rtc.getMinutes() % 45)) {
                  evtActive = true;
                  events[evtID].show();
                  rtc.readTime();
                }
                break;
              case 4:
                while (!(rtc.getMinutes() % 60)) {
                  evtActive = true;
                  events[evtID].show();
                  rtc.readTime();
                }
                break;
              default:
                break;
            }
            evtActive = false;
          }
        }
#endif
      case EXT_MODE_TIMESET:
        renderer.clearScreenBuffer(matrix);
        renderer.setMinutes(rtc.getHours() + settings.getTimeShift(), rtc.getMinutes(), settings.getLanguage(), matrix);
        renderer.setCorners(rtc.getMinutes(), settings.getRenderCornersCw(), matrix);
        break;
#ifdef USE_EXT_MODE_TIME_SHIFT
      case EXT_MODE_TIME_SHIFT:
        renderer.clearScreenBuffer(matrix);
        if (settings.getTimeShift() < 0) {
          for (byte x = 0; x < 3; x++) {
            ledDriver.setPixelInScreenBuffer(x, 1, matrix);
          }
        } else if (settings.getTimeShift() > 0) {
          for (byte x = 0; x < 3; x++) {
            ledDriver.setPixelInScreenBuffer(x, 1, matrix);
          }
          for (byte y = 0; y < 3; y++) {
            ledDriver.setPixelInScreenBuffer(1, y, matrix);
          }
        }
        for (byte i = 0; i < 7; i++) {
          matrix[3 + i] |= pgm_read_byte_near(&(ziffern[abs(settings.getTimeShift()) % 10][i])) << 5;
          if (abs(settings.getTimeShift()) > 9) {
            matrix[3 + i] |= pgm_read_byte_near(&(ziffern[1][i])) << 10;
          }
        }
        break;
#endif
#ifdef ALARM
      case STD_MODE_ALARM:
        renderer.clearScreenBuffer(matrix);
        if (alarm.getShowAlarmTimeTimer() == 0) {
          renderer.setMinutes(rtc.getHours() + settings.getTimeShift(), rtc.getMinutes(), settings.getLanguage(), matrix);
          renderer.setCorners(rtc.getMinutes(), settings.getRenderCornersCw(), matrix);
          renderer.activateAlarmLed(matrix); // Alarm-LED
        } else {
          renderer.setMinutes(alarm.getHours() + settings.getTimeShift(), alarm.getMinutes(), settings.getLanguage(), matrix);
          renderer.setCorners(alarm.getMinutes(), settings.getRenderCornersCw(), matrix);
          renderer.cleanWordsForAlarmSettingMode(settings.getLanguage(), matrix); // ES IST weg
          if (alarm.getShowAlarmTimeTimer() % 2 == 0) {
            renderer.activateAlarmLed(matrix); // Alarm-LED
          }
          alarm.decShowAlarmTimeTimer();
        }
        break;
#endif
#ifdef USE_STD_MODE_DATE
      case STD_MODE_DATE:
        renderer.clearScreenBuffer(matrix);
        // Anzeige des Datums
        for (byte i = 0; i < 5; i++) {
          matrix[0 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getDate() / 10][i])) << 11;
          matrix[0 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getDate() % 10][i])) << 6;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getMonth() / 10][i])) << 11;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getMonth() % 10][i])) << 6;
        }
        
        ledDriver.setPixelInScreenBuffer(10, 4, matrix);
        ledDriver.setPixelInScreenBuffer(10, 9, matrix);
        break;
#endif
#ifdef USE_STD_MODE_TEMP
      case STD_MODE_TEMP:
      Serial.println(tempSens.getTempC());
        renderer.clearScreenBuffer(matrix);
        for (byte i = 0; i < 7; i++) {
          matrix[1 + i] |= pgm_read_byte_near(&(ziffern[tempSens.getTempC() / 10][i])) << 11;
          matrix[1 + i] |= pgm_read_byte_near(&(ziffern[tempSens.getTempC() % 10][i])) << 5;
        }
        matrix[0] |= 0b0000000000011111;
        break;
#endif
      case STD_MODE_SECONDS:
        renderer.clearScreenBuffer(matrix);
        for (byte i = 0; i < 7; i++) {
          matrix[1 + i] |= pgm_read_byte_near(&(ziffern[helperSeconds / 10][i])) << 11;
          matrix[1 + i] |= pgm_read_byte_near(&(ziffern[helperSeconds % 10][i])) << 5;
        }
        break;
      case EXT_MODE_LDR_MODE:
        renderer.clearScreenBuffer(matrix);
        if (settings.getUseLdr()) {
          for (byte i = 0; i < 5; i++) {
            renderer.setMenuText("A", Renderer::TEXT_POS_MIDDLE, matrix);
          }
        } else {
          for (byte i = 0; i < 5; i++) {
            renderer.setMenuText("M", Renderer::TEXT_POS_MIDDLE, matrix);
          }
        }
        break;
      case STD_MODE_BLANK:
      case STD_MODE_NIGHT:
        renderer.clearScreenBuffer(matrix);
        break;
      case STD_MODE_BRIGHTNESS:
        renderer.clearScreenBuffer(matrix);
        brightnessToDisplay = map(settings.getBrightness(), 1, 100, 0, 9);
        for (byte xb = 0; xb < brightnessToDisplay; xb++) {
          for (byte yb = 0; yb <= xb; yb++) {
            matrix[9 - yb] |= 1 << (14 - xb);
          }
        }
        break;
#ifdef USE_EXT_MODE_CORNERS
      case EXT_MODE_CORNERS:
        renderer.clearScreenBuffer(matrix);
        if (settings.getRenderCornersCw()) {
          renderer.setMenuText("CW", Renderer::TEXT_POS_MIDDLE, matrix);
        } else {
          renderer.setMenuText("C", Renderer::TEXT_POS_TOP, matrix);
          renderer.setMenuText("CW", Renderer::TEXT_POS_BOTTOM, matrix);
        }
        break;
#endif
#ifdef ALARM
      case EXT_MODE_ENABLE_ALARM:
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("AL", Renderer::TEXT_POS_TOP, matrix);
        if (settings.getEnableAlarm()) {
          renderer.setMenuText("EN", Renderer::TEXT_POS_BOTTOM, matrix);
        } else {
          renderer.setMenuText("DA", Renderer::TEXT_POS_BOTTOM, matrix);
        }
        break;
#endif
#ifdef EVENTS
      // Eventwiederholung einstelleng
      case EXT_MODE_EVENT:
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("EV", Renderer::TEXT_POS_TOP, matrix);
        switch (settings.getEvent()) {
          case 0:
            renderer.setMenuText("5", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case 1:
            renderer.setMenuText("15", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case 2:
            renderer.setMenuText("30", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case 3:
            renderer.setMenuText("45", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case 4:
            renderer.setMenuText("60", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
        }
        break;
#endif
      case EXT_MODE_TRANSITION:
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("TM", Renderer::TEXT_POS_TOP, matrix);
        switch (settings.getTransitionMode()) {
          case Settings::TRANSITION_MODE_NORMAL:
            renderer.setMenuText("NO", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case Settings::TRANSITION_MODE_FADE:
            renderer.setMenuText("FD", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case Settings::TRANSITION_MODE_MATRIX:
            renderer.setMenuText("MX", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case Settings::TRANSITION_MODE_SLIDE:
            renderer.setMenuText("SD", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
        }
        break;
#if defined(RGB_LEDS) || defined(RGBW_LEDS)
      case EXT_MODE_COLOR:
        renderer.clearScreenBuffer(matrix);
        if (settings.getColor() <= color_single_max) {
          renderer.setMenuText("C", Renderer::TEXT_POS_TOP, matrix);
          for (byte i = 0; i < 5; i++) {
            matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[settings.getColor() / 10][i])) << 10;
            matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[settings.getColor() % 10][i])) << 5;
          }
        }
        else {
          renderer.setMenuText("CC", Renderer::TEXT_POS_TOP, matrix);
          for (byte i = 0; i < 5; i++) {
            matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[(settings.getColor() - color_single_max) / 10][i])) << 10;
            matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[(settings.getColor() - color_single_max) % 10][i])) << 5;
          }
        }
        break;
      case EXT_MODE_COLOR_CHANGE:
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("CR", Renderer::TEXT_POS_TOP, matrix);
        for (byte i = 0; i < 7; i++) {
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[settings.getColorChangeRate() / 10][i])) << 11;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[settings.getColorChangeRate() % 10][i])) << 5;
        }
        break;
#endif
      case EXT_MODE_JUMP_TIMEOUT:
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("FB", Renderer::TEXT_POS_TOP, matrix);
        for (byte i = 0; i < 7; i++) {
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[settings.getJumpToNormalTimeout() / 10][i])) << 11;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[settings.getJumpToNormalTimeout() % 10][i])) << 5;
        }
        break;
      ///**
      //   Hier definiert man die Ab- und Anschaltzeiten fuer das Display. Die Abschaltung des
      //   Displays verbessert den Empfang des DCF77-Empfaengers. Und hilft, falls die Uhr im
      //   Schlafzimmer haengt.
      //   Sind alle Werte auf 0 wird das Display nie abgeschaltet. Nach einer Minute kann man das
      //   Display manuell wieder ein- / ausschalten.
      //   Achtung! Wenn sich die Uhr nachmittags abschaltet ist sie in der falschen Tageshaelfte!
      //*/
      case EXT_MODE_NIGHT_OFF:
        renderer.clearScreenBuffer(matrix);
        if (!fallBackCounter) {
          renderer.setMenuText("NO", Renderer::TEXT_POS_TOP, matrix);
          renderer.setMenuText("FF", Renderer::TEXT_POS_BOTTOM, matrix);
        }
        else
        {
          renderer.clearScreenBuffer(matrix);
          renderer.setMinutes(settings.getNightModeTime(false)->getHours(), settings.getNightModeTime(false)->getMinutes(), settings.getLanguage(), matrix);
          renderer.cleanWordsForAlarmSettingMode(settings.getLanguage(), matrix); // ES IST weg
          if ( (settings.getNightModeTime(false)->getHours() >= 12) ) {
            renderer.setCorners(1, settings.getRenderCornersCw(), matrix);
          }
        }
        break;
      case EXT_MODE_NIGHT_ON:
        renderer.clearScreenBuffer(matrix);
        if (!fallBackCounter) {
          renderer.setMenuText("N", Renderer::TEXT_POS_TOP, matrix);
          renderer.setMenuText("ON", Renderer::TEXT_POS_BOTTOM, matrix);
        }
        else
        {
          renderer.clearScreenBuffer(matrix);
          renderer.setMinutes(settings.getNightModeTime(true)->getHours(), settings.getNightModeTime(true)->getMinutes(), settings.getLanguage(), matrix);
          renderer.cleanWordsForAlarmSettingMode(settings.getLanguage(), matrix); // ES IST weg
          if ( (settings.getNightModeTime(true)->getHours() >= 12) ) {
            renderer.setCorners(1, settings.getRenderCornersCw(), matrix);
          }
        }
        break;

      case EXT_MODE_DCF_IS_INVERTED:
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("RS", Renderer::TEXT_POS_TOP, matrix);
        if (settings.getDcfSignalIsInverted()) {
          renderer.setMenuText("IN", Renderer::TEXT_POS_BOTTOM, matrix);
        } else {
          renderer.setMenuText("NO", Renderer::TEXT_POS_BOTTOM, matrix);
        }
        break;

#ifdef USE_EXT_MODE_DATE_MANUALLY
      case EXT_MODE_YEARSET:   // Einstellung Jahr
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("YY", Renderer::TEXT_POS_TOP, matrix);
          for (byte i = 0; i < 5; i++) {
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getYear() % 10][i])) << 5;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getYear() / 10][i])) << 10;
         }
        break;    

      case EXT_MODE_MONTHSET:   // Einstellung Monat
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("MM", Renderer::TEXT_POS_TOP, matrix);
         if (rtc.getMonth() > 9) {
          for (byte i = 0; i < 5; i++) {
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getMonth() % 10][i])) << 5;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getMonth() / 10][i])) << 10;
          }
          } else {
          for (byte i = 0; i < 5; i++) {
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getMonth() % 10][i])) << 7;           
         }
        }
        break;      

      case EXT_MODE_DATESET:   // Einstellung Tag
        renderer.clearScreenBuffer(matrix);
        renderer.setMenuText("DD", Renderer::TEXT_POS_TOP, matrix);
         if (rtc.getDate() > 9) {
          for (byte i = 0; i < 5; i++) {
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getDate() % 10][i])) << 5;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getDate() / 10][i])) << 10;
          }
          } else {
          for (byte i = 0; i < 5; i++) {
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[rtc.getDate() % 10][i])) << 7;           
         }
        }
        break;      
#endif        
        
      case EXT_MODE_LANGUAGE:
        renderer.clearScreenBuffer(matrix);
        switch (settings.getLanguage()) {
#ifdef ENABLE_LANGUAGE_DE
          case LANGUAGE_DE_DE:
            renderer.setMenuText("DE", Renderer::TEXT_POS_MIDDLE, matrix);
            break;
          case LANGUAGE_DE_SW:
            renderer.setMenuText("DE", Renderer::TEXT_POS_TOP, matrix);
            renderer.setMenuText("SW", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case LANGUAGE_DE_BA:
            renderer.setMenuText("DE", Renderer::TEXT_POS_TOP, matrix);
            renderer.setMenuText("BA", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
          case LANGUAGE_DE_SA:
            renderer.setMenuText("DE", Renderer::TEXT_POS_TOP, matrix);
            renderer.setMenuText("SA", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
#endif
#ifdef ENABLE_LANGUAGE_CH
          case LANGUAGE_CH:
            renderer.setMenuText("CH", Renderer::TEXT_POS_MIDDLE, matrix);
            break;
          case LANGUAGE_CH_X:
            renderer.setMenuText("CH", Renderer::TEXT_POS_TOP, matrix);
            renderer.setMenuText("X", Renderer::TEXT_POS_BOTTOM, matrix);
            break;
#endif
#ifdef ENABLE_LANGUAGE_EN
          case LANGUAGE_EN:
            renderer.setMenuText("EN", Renderer::TEXT_POS_MIDDLE, matrix);
            break;
#endif
#ifdef ENABLE_LANGUAGE_FR
          case LANGUAGE_FR:
            renderer.setMenuText("FR", Renderer::TEXT_POS_MIDDLE, matrix);
            break;
#endif
#ifdef ENABLE_LANGUAGE_IT
          case LANGUAGE_IT:
            renderer.setMenuText("IT", Renderer::TEXT_POS_MIDDLE, matrix);
            break;
#endif
#ifdef ENABLE_LANGUAGE_NL
          case LANGUAGE_NL:
            renderer.setMenuText("NL", Renderer::TEXT_POS_MIDDLE, matrix);
            break;
#endif
#ifdef ENABLE_LANGUAGE_ES
          case LANGUAGE_ES:
            renderer.setMenuText("ES", Renderer::TEXT_POS_MIDDLE, matrix);
            break;
#endif
          default:
            ;
        }
        break;

#ifdef USE_EXT_MODE_TEST
      case EXT_MODE_TEST:
        renderer.clearScreenBuffer(matrix);
        renderer.setCorners(helperSeconds % 5, settings.getRenderCornersCw(), matrix);
#ifdef ALARM
        if (settings.getEnableAlarm()) {
          renderer.activateAlarmLed(matrix); // Alarm-LED
        }
#endif
        for (byte i = 0; i < 11; i++) {
          ledDriver.setPixelInScreenBuffer(testColumn, i, matrix);
        }
        testColumn++;
        if (testColumn > 10) {
          testColumn = 0;
        }
        break;
#endif
#ifdef USE_EXT_MODE_DCF_SYNC
      case EXT_MODE_DCF_SYNC:
        // Anzeige des letzten erfolgreichen DCF-Syncs (samplesOK) in Stunden:Minuten
        renderer.clearScreenBuffer(matrix);
        dcf77ErrorMinutes = dcf77.getDcf77LastSuccessSyncMinutes();
        for (byte i = 0; i < 5; i++) {
          matrix[0 + i] |= pgm_read_byte_near(&(ziffernB[dcf77ErrorMinutes / 60 / 10][i])) << 11;
          matrix[0 + i] |= pgm_read_byte_near(&(ziffernB[dcf77ErrorMinutes / 60 % 10][i])) << 6;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[dcf77ErrorMinutes % 60 / 10][i])) << 11;
          matrix[5 + i] |= pgm_read_byte_near(&(ziffernB[dcf77ErrorMinutes % 60 % 10][i])) << 6;
        }
        ledDriver.setPixelInScreenBuffer(10, 1, matrix);
        ledDriver.setPixelInScreenBuffer(10, 3, matrix);
        break;
#endif

#ifdef USE_EXT_MODE_DCF_DEBUG
      case EXT_MODE_DCF_DEBUG:
        renderer.clearScreenBuffer(matrix);
        renderer.setCorners(dcf77.getDcf77ErrorCorner(), settings.getRenderCornersCw(), matrix);
        break;
#endif
      default:
        break;
    }

    // Update mit onChange = true, weil sich hier (aufgrund needsUpdateFromRtc) immer was geaendert hat.
    // Entweder weil wir eine Sekunde weiter sind, oder weil eine Taste gedrueckt wurde.
    ledDriver.writeScreenBufferToMatrix(matrix, true, settings.getColor());
  }
#ifdef USE_EXT_MODE_DCF_DEBUG
  if (mode == EXT_MODE_DCF_DEBUG) {
    byte currentErrorCorner = dcf77.getDcf77ErrorCorner();
    dcf77.updateDcf77ErrorCorner(settings.getDcfSignalIsInverted());
    if (currentErrorCorner != dcf77.getDcf77ErrorCorner()) {
      needsUpdateFromRtc = true;
    }
  }
#endif
  
  /*
     Tasten abfragen (Code mit 3.3.0 ausgelagert, wegen der Fernbedienung)
  */// M+ und H+ im STD_MODE_NORMAL gedrueckt?
  if ((mode == STD_MODE_NORMAL) && extModeDoubleButton.pressed()) {
    doubleStdModeNormalPressed();
  }
  //
  // M+ und H+ im STD_MODE_BLANK gedrueckt?
  if ((mode == STD_MODE_BLANK) && extModeDoubleButton.pressed()) {
    doubleExtModePressed();
  }
#ifdef EVENTS
  // M+ und H+ im STD_MODE_SECONDS gedrueckt?
  if ((mode == STD_MODE_SECONDS) && extModeDoubleButton.pressed()) {
    doubleEvtModePressed();
  }
#endif

  // Taste Minuten++ (brighness++) gedrueckt?
  if (minutesPlusButton.pressed()) {
    minutePlusPressed();
  }

  // Taste Stunden++ (brightness--) gedrueckt?
  if (hoursPlusButton.pressed()) {
    hourPlusPressed();
  }

  // Taste Moduswechsel gedrueckt?
  if (modeChangeButton.pressed()) {
    modePressed();
  }

  /*
     Tasten der Fernbedienung abfragen...
  */

#ifdef REMOTE_BLUETOOTH
  unsigned int lastIrCodeBT = 0;
  while (Serial.available() > 0) {
    lastIrCodeBT = irTranslatorBT.buttonForCode(Serial.parseInt());
    Serial.read();
  }
  if (lastIrCodeBT != 0) {
    remoteAction(lastIrCodeBT, &irTranslatorBT);
  }
#endif

#ifndef REMOTE_NO_REMOTE
  unsigned long lastIrCode = 0;
  if (irrecv.decode(&irDecodeResults)) {
    lastIrCode = irTranslator.buttonForCode(irDecodeResults.value);
    irrecv.resume();
  }
  if (lastIrCode != 0) {
    DEBUG_PRINT(F("Decoded successfully as "));
    DEBUG_PRINTLN2(irDecodeResults.value, HEX);
    remoteAction(lastIrCode, &irTranslator);
  }
#endif

  /*
     Display zeitgesteuert abschalten?
     Das Verbessert den DCF77-Empfang bzw. ermoeglicht ein dunkles Schlafzimmer.
  */
  if ((settings.getNightModeTime(true)->getMinutesOfDay() != 0) && (settings.getNightModeTime(false)->getMinutesOfDay() != 0)) {
    if ((mode < EXT_MODE_START) && (mode != STD_MODE_NIGHT) && (settings.getNightModeTime(false)->getMinutesOfDay() == rtc.getMinutesOfDay()) && (helperSeconds == 0)) {
      mode = STD_MODE_NIGHT;
      ledDriver.shutDown();
    }
    if ((mode == STD_MODE_NIGHT) && (settings.getNightModeTime(true)->getMinutesOfDay() == rtc.getMinutesOfDay()) && (helperSeconds == 0)) {
      mode = lastMode;
      ledDriver.wakeUp();
    }
  }

#ifdef ALARM
  /*
     Alarm?
  */
  if ((mode == STD_MODE_ALARM) && (alarm.getShowAlarmTimeTimer() == 0) && !alarm.isActive()) {
    if (alarm.getMinutesOf12HoursDay(0) == rtc.getMinutesOf12HoursDay(0)) {
      alarm.activate();
    }
  }
  if (alarm.isActive()) {
    // Nach 10 Minuten automatisch abschalten, falls der Wecker alleine rumsteht und die Nachbarn nervt...
    if (alarm.getMinutesOf12HoursDay(MAX_BUZZ_TIME_IN_MINUTES) == rtc.getMinutesOf12HoursDay(0)) {
      alarm.deactivate();
      alarm.buzz(false);
      setMode(STD_MODE_NORMAL);
    }
    // Krach machen...
    if (helperSeconds % 2 == 0) {
      alarm.buzz(true);
    } else {
      alarm.buzz(false);
    }
  }
#endif

  /*
     Die Matrix auf die LEDs multiplexen, hier 'Refresh-Zyklen'.
  */
  if ((mode != STD_MODE_BLANK) && (mode != STD_MODE_NIGHT)) {
    ledDriver.writeScreenBufferToMatrix(matrix, false, settings.getColor());
  }

  /*
     Status-LEDs ausgeben
  */
#ifdef ENABLE_DCF_LED
  dcf77.statusLed(dcf77.signal(settings.getDcfSignalIsInverted()));
#endif
#ifdef ENABLE_SQW_LED
  rtc.statusLed(digitalRead(PIN_SQW_SIGNAL) == HIGH);
#endif

/*
   DCF77-Empfaenger anticken...
*/
  if (dcf77.poll(settings.getDcfSignalIsInverted()))
    manageNewDCF77Data();
}

/**
   Was soll ausgefuehrt werden, wenn die H+ und M+ -Taste zusammen gedrueckt wird?
*/
// Im Mode STD_MODE_NORMAL
void doubleStdModeNormalPressed() {
  needsUpdateFromRtc = true;
  DEBUG_PRINTLN(F("Minutes plus AND hours plus pressed in STD_MODE_NORMAL..."));
  DEBUG_FLUSH();
  if ( isCurrentTimeInNightRange() )
  {
    mode = STD_MODE_NIGHT;
    ledDriver.shutDown();
    DEBUG_PRINTLN(F("Entering STD_MODE_NIGHT..."));
    DEBUG_FLUSH();
  }
}

// Im Mode STD_MODE_BLANK
void doubleExtModePressed() {
  needsUpdateFromRtc = true;
  DEBUG_PRINTLN(F("Minutes plus AND hours plus pressed in STD_MODE_BLANK..."));
  DEBUG_FLUSH();
  unsigned long initialMillis = millis();
  while (minutesPlusButton.pressedRaw() && hoursPlusButton.pressedRaw()) {
    if ((millis() - initialMillis) > 5000) {
      setMode(STD_MODE_NORMAL);
      settings.resetToDefault();
      settings.saveToEEPROM();
      ledDriver.wakeUp();
      return;
    }
  }
  setMode(EXT_MODE_START);
  ledDriver.wakeUp();
  DEBUG_PRINT(F("Entering EXT_MODEs, mode is now "));
  DEBUG_PRINT(mode);
  DEBUG_PRINTLN(F("..."));
  DEBUG_FLUSH();
}

#ifdef EVENTS
void doubleEvtModePressed() {
  static byte i = 0;
  disableFallBackCounter();
  needsUpdateFromRtc = true;
  DEBUG_PRINTLN(F("Minutes plus AND hours plus pressed in STD_MODE_BLANK..."));
  DEBUG_FLUSH();

  if (nbrOfEvts > 0) {
    events[i].show();
    i++;
  }
  if (i >= nbrOfEvts) {
    i = 0;
  }
  enableFallBackCounter(settings.getJumpToNormalTimeout());
  
  DEBUG_PRINT(F("Entering EXT_MODEs, mode is now "));
  DEBUG_PRINT(mode);
  DEBUG_PRINTLN(F("..."));
  DEBUG_FLUSH();
}
#endif

/**
   Was soll ausgefuehrt werden, wenn die Mode-Taste gedrueckt wird?
*/
void modePressed() {
  needsUpdateFromRtc = true;

  // Displaytreiber einschalten, wenn BLANK verlassen wird
  if (mode == STD_MODE_BLANK) {
    DEBUG_PRINTLN(F("LED-Driver: WakeUp"));
    DEBUG_FLUSH();
    ledDriver.wakeUp();
  }
  
#ifdef ALARM
  if (alarm.isActive()) {
    alarm.deactivate();
    mode = STD_MODE_NORMAL;
  } else {
    switch (mode) {
      // Durch Drücken der MODE-Taste den Nachtmodus verlassen
      case STD_MODE_NIGHT:
        setDisplayToToggle();
        break;
      default:
        mode++;
        break;
    }
  }
#else
  switch (mode) {
    // Durch Drücken der MODE-Taste den Nachtmodus verlassen
    case STD_MODE_NIGHT:
      setDisplayToToggle();
      break;
    default:
      mode++;
      break;
  }
#endif

  // Brightness ueberspringen, wenn LDR verwendet wird.
  if (settings.getUseLdr() && (mode == STD_MODE_BRIGHTNESS)) {
    mode++;
  }
#ifdef ALARM
  // Alarm ueberspringen, wenn kein Alarm enabled ist.
  if (!settings.getEnableAlarm() && (mode == STD_MODE_ALARM)) {
      mode++;
  }
#endif

  if ((mode == STD_MODE_COUNT) || (mode == EXT_MODE_COUNT)) {
    mode = STD_MODE_NORMAL;
  }

#ifdef ALARM
  if (mode == STD_MODE_ALARM) {
    // wenn auf Alarm gewechselt wurde, fuer 10 Sekunden die
    // Weckzeit anzeigen.
    alarm.setShowAlarmTimeTimer(10);
  }
#endif

  disableFallBackCounter();

  switch (mode) {
    case STD_MODE_SECONDS:
    case STD_MODE_BRIGHTNESS:
#ifdef USE_STD_MODE_DATE
    case STD_MODE_DATE:
#endif
#ifdef USE_STD_MODE_TEMP
    case STD_MODE_TEMP:
#endif
      // Timeout für den automatischen Rücksprung von STD_MODE_SECONDS,
      // STD_MODE_DATE und STD_MODE_BRIGHTNESS zurücksetzen
      enableFallBackCounter(settings.getJumpToNormalTimeout());
      break;
    default:
      disableFallBackCounter();
  }


  DEBUG_PRINT(F("Change mode pressed, mode is now "));
  DEBUG_PRINT(mode);
  DEBUG_PRINTLN(F("..."));
  DEBUG_FLUSH();

  // Displaytreiber ausschalten, wenn BLANK
  if (mode == STD_MODE_BLANK) {
    DEBUG_PRINTLN(F("LED-Driver: ShutDown"));
    DEBUG_FLUSH();
    ledDriver.shutDown();
  }

  if( (mode != STD_MODE_BLANK) && (mode != STD_MODE_NIGHT) ) {
    lastMode = mode;  
  }

  // Werte speichern (die Funktion speichert nur bei geaenderten Werten)...
  settings.saveToEEPROM();
}

/**
   Was soll ausgefuehrt werden, wenn die H+-Taste gedrueckt wird?
*/
void hourPlusPressed() {
  needsUpdateFromRtc = true;

  DEBUG_PRINTLN(F("Hours plus pressed..."));
  DEBUG_FLUSH();

  switch (mode) {
    case EXT_MODE_TIMESET:
      incDecHours(true);
      break;
#ifdef USE_EXT_MODE_TIME_SHIFT
    case EXT_MODE_TIME_SHIFT:
      if (settings.getTimeShift() > -13) {
        settings.setTimeShift(settings.getTimeShift() - 1);
      }
      break;
#endif
#ifdef USE_EXT_MODE_DATE_MANUALLY
    case EXT_MODE_YEARSET:
      rtc.incYear(10);
      rtc.writeTime();
    break;

    case EXT_MODE_MONTHSET:
      rtc.incMonth(10);
      rtc.writeTime();
    break;

    case EXT_MODE_DATESET:
      rtc.incDate(10);
      rtc.writeTime();
    break;
#endif
#ifdef ALARM
    case STD_MODE_ALARM:
      alarm.incHours();
      alarm.setShowAlarmTimeTimer(10);
      DEBUG_PRINT(F("A is now "));
      DEBUG_PRINTLN(alarm.asString());
      DEBUG_FLUSH();
      break;
#endif
    case STD_MODE_BRIGHTNESS:
      // RESET counter
      enableFallBackCounter(settings.getJumpToNormalTimeout());
      setDisplayDarker();
      break;
    case EXT_MODE_LDR_MODE:
      settings.setUseLdr(!settings.getUseLdr());
      if (!settings.getUseLdr()) {
        settings.setBrightness(ledDriver.getBrightness());
      }
      DEBUG_PRINT(F("LDR is now "));
      DEBUG_PRINTLN(settings.getUseLdr());
      DEBUG_FLUSH();
      break;
#ifdef USE_EXT_MODE_CORNERS
    case EXT_MODE_CORNERS:
      settings.setRenderCornersCw(!settings.getRenderCornersCw());
      break;
#endif
#ifdef ALARM
    case EXT_MODE_ENABLE_ALARM:
      settings.setEnableAlarm(!settings.getEnableAlarm());
      break;
#endif
#ifdef EVENTS
    // Eventwiederholungszeit Auswahl
    case EXT_MODE_EVENT:
      if (settings.getEvent() == 0) {
        settings.setEvent(4);
      }
      else {
        settings.setEvent(settings.getEvent() - 1);
      }
      break;
#endif
    case EXT_MODE_TRANSITION:
      if (settings.getTransitionMode() == 0) {
        settings.setTransitionMode(Settings::TRANSITION_MODE_MAX - 1);
      } else {
        settings.setTransitionMode(settings.getTransitionMode() - 1);
#if !defined(RGB_LEDS) && !defined(RGBW_LEDS)
        if (settings.getTransitionMode() == Settings::TRANSITION_MODE_MATRIX) {
          settings.setTransitionMode(settings.getTransitionMode() - 1);
        }
#endif
      }
      break;
#if defined(RGB_LEDS) || defined(RGBW_LEDS)
    case EXT_MODE_COLOR:
      if (settings.getColor() == 0)
      {
        settings.setColor(color_max);
      }
      else
      {
        settings.setColor((eColors)(settings.getColor() - 1));
      }
      break;
    case EXT_MODE_COLOR_CHANGE:
      if (settings.getColorChangeRate() > 0) {
        settings.setColorChangeRate(settings.getColorChangeRate() - 1);
      }
      break;
#endif
    case EXT_MODE_JUMP_TIMEOUT:
      if (settings.getJumpToNormalTimeout() > 0) {
        settings.setJumpToNormalTimeout(settings.getJumpToNormalTimeout() - 1);
      }
      break;
    case EXT_MODE_NIGHT_OFF:
      if (fallBackCounter > 0) {
        settings.getNightModeTime(false)->incHours();
      }
      enableFallBackCounter(FALL_BACK_TIME_NIGHT_MODE);
      break;
    case EXT_MODE_NIGHT_ON:
      if (fallBackCounter > 0) {
        settings.getNightModeTime(true)->incHours();
      }
      enableFallBackCounter(FALL_BACK_TIME_NIGHT_MODE);
      break;
    case EXT_MODE_DCF_IS_INVERTED:
      settings.setDcfSignalIsInverted(!settings.getDcfSignalIsInverted());
      break;
    case EXT_MODE_LANGUAGE:
      if (settings.getLanguage() == 0) {
        settings.setLanguage(LANGUAGE_COUNT-1);
      } else {
        settings.setLanguage(settings.getLanguage() - 1);
      }
      break;
    default:
      break;
  }
}

/**
   Was soll ausgefuehrt werden, wenn die M+-Taste gedrueckt wird?
*/
void minutePlusPressed() {
  needsUpdateFromRtc = true;

  DEBUG_PRINTLN(F("Minutes plus pressed..."));
  DEBUG_FLUSH();

  switch (mode) {
    case EXT_MODE_TIMESET:
      incDecMinutes(true);
      break;
#ifdef USE_EXT_MODE_TIME_SHIFT
    case EXT_MODE_TIME_SHIFT:
      if (settings.getTimeShift() < 13) {
        settings.setTimeShift(settings.getTimeShift() + 1);
      }
      break;
#endif
#ifdef USE_EXT_MODE_DATE_MANUALLY
    case EXT_MODE_YEARSET:
      rtc.incYear();
      rtc.writeTime();
      break;

    case EXT_MODE_MONTHSET:
      rtc.incMonth();
      rtc.writeTime();
      break;

    case EXT_MODE_DATESET:
      rtc.incDate();
      rtc.writeTime();
      break;
#endif
#ifdef ALARM
    case STD_MODE_ALARM:
      alarm.incMinutes();
      alarm.setShowAlarmTimeTimer(10);
      DEBUG_PRINT(F("A is now "));
      DEBUG_PRINTLN(alarm.asString());
      DEBUG_FLUSH();
      break;
#endif
    case STD_MODE_BRIGHTNESS:
      // RESET counter
      enableFallBackCounter(settings.getJumpToNormalTimeout());
      setDisplayBrighter();
      break;
    case EXT_MODE_LDR_MODE:
      settings.setUseLdr(!settings.getUseLdr());
      if (!settings.getUseLdr()) {
        settings.setBrightness(ledDriver.getBrightness());
      }
      DEBUG_PRINT(F("LDR is now "));
      DEBUG_PRINTLN(settings.getUseLdr());
      DEBUG_FLUSH();
      break;
#ifdef USE_EXT_MODE_CORNERS
    case EXT_MODE_CORNERS:
      settings.setRenderCornersCw(!settings.getRenderCornersCw());
      break;
#endif
#ifdef ALARM
    case EXT_MODE_ENABLE_ALARM:
      settings.setEnableAlarm(!settings.getEnableAlarm());
      break;
#endif
#ifdef EVENTS
    // Eventwiederholungszeit Auswahl
    case EXT_MODE_EVENT:
      settings.setEvent(settings.getEvent() + 1);
      if (settings.getEvent() > 4) {
        settings.setEvent(0);
      }
      break;
#endif
    case EXT_MODE_TRANSITION:
      if (settings.getTransitionMode() == Settings::TRANSITION_MODE_MAX - 1) {
        settings.setTransitionMode(0);
      } else {
        settings.setTransitionMode(settings.getTransitionMode() + 1);
#if !defined(RGB_LEDS) && !defined(RGBW_LEDS)
        if (settings.getTransitionMode() == Settings::TRANSITION_MODE_MATRIX) {
          settings.setTransitionMode(settings.getTransitionMode() + 1);
        }
#endif
      }
      break;
#if defined(RGB_LEDS) || defined(RGBW_LEDS)
    case EXT_MODE_COLOR:
      if (settings.getColor() == color_max)
      {
        settings.setColor((eColors)0);
      }
      else
      {
        settings.setColor((eColors)(settings.getColor() + 1));
      }
      if (settings.getColor() > color_single_max){
        ledDriver.resetWheelPos();
      }
      break;
    case EXT_MODE_COLOR_CHANGE:
      if (settings.getColorChangeRate() < 10) {
        settings.setColorChangeRate(settings.getColorChangeRate() + 1);
      }
      break;
#endif
    case EXT_MODE_JUMP_TIMEOUT:
      if (settings.getJumpToNormalTimeout() < 99) {
        settings.setJumpToNormalTimeout(settings.getJumpToNormalTimeout() + 1);
      }
      break;
    case EXT_MODE_NIGHT_OFF:
      if (fallBackCounter > 0) {
        settings.getNightModeTime(false)->incFiveMinutes();
      }
      enableFallBackCounter(FALL_BACK_TIME_NIGHT_MODE);
      break;
    case EXT_MODE_NIGHT_ON:
      if (fallBackCounter > 0) {
        settings.getNightModeTime(true)->incFiveMinutes();
      }
      enableFallBackCounter(FALL_BACK_TIME_NIGHT_MODE);
      break;
    case EXT_MODE_DCF_IS_INVERTED:
      settings.setDcfSignalIsInverted(!settings.getDcfSignalIsInverted());
      break;
    case EXT_MODE_LANGUAGE:
      settings.setLanguage(settings.getLanguage() + 1);
      if (settings.getLanguage() >= LANGUAGE_COUNT) {
        settings.setLanguage(0);
      }
      break;
    default:
      break;
  }
}

/**
   Den DCF77-Empfaenger ein-/ausschalten.
*/
void enableDcf(boolean enable) {
  if (enable) {
    DEBUG_PRINTLN(F("DCF77-Empfaenger aufgeweckt."));
    DEBUG_FLUSH();
    digitalWrite(PIN_DCF77_PON, LOW);
  } else {
    DEBUG_PRINTLN(F("DCF77-Empfaenger schlafen gelegt."));
    DEBUG_FLUSH();
    digitalWrite(PIN_DCF77_PON, HIGH);
  }
}

/**
   Korrekte Daten (auf Basis der Pruefbits) vom DCF-Empfaenger
   bekommen. Sicherheitshalber gegen Zeitabstaende der RTC pruefen.
*/
void manageNewDCF77Data() {
  DEBUG_PRINT(F("Captured: "));
  DEBUG_PRINTLN(dcf77.asString());
  DEBUG_FLUSH();

  rtc.readTime();
  dcf77Helper.addSample(&dcf77, &rtc);
  // Stimmen die Abstaende im Array?
  // Pruefung mit Datum!
  if (dcf77Helper.samplesOk()) {
    rtc.set(&dcf77);
    resetSeconds();
    DEBUG_PRINTLN(F("DCF77-Time written to RTC."));
    DEBUG_FLUSH();
#ifdef USE_EXT_MODE_DCF_SYNC
    dcf77.setDcf77SuccessSync();
#endif
#ifdef AUTO_JUMP_BLANK
    // falls im manuellen Dunkel-Modus, Display wieder einschalten... (Hilft bei der Erkennung, ob der DCF-Empfang geklappt hat).
    if ((mode == STD_MODE_BLANK) || ((mode == STD_MODE_NIGHT) && !isCurrentTimeInNightRange())) {
      setMode(STD_MODE_NORMAL);
      ledDriver.wakeUp();
    }
#endif
  } else {
    DEBUG_PRINTLN(F("DCF77-Time trashed because wrong distances between timestamps."));
    DEBUG_FLUSH();
  }
}

/**
   Das Display toggeln (aus-/einschalten).
*/
void setDisplayToToggle() {
  if ((mode != STD_MODE_BLANK) && (mode != STD_MODE_NIGHT)) {
    setDisplayToBlank();
  } else {
    setDisplayToResume();
  }
}

/**
   Das Display ausschalten.
*/
void setDisplayToBlank() {
  mode = STD_MODE_BLANK;
  doubleStdModeNormalPressed();
  ledDriver.shutDown();
  DEBUG_PRINTLN(F("LED-Driver: ShutDown"));
  DEBUG_FLUSH();
}

/**
   Das Display einschalten.
*/
void setDisplayToResume() {
  mode = lastMode;
  ledDriver.wakeUp();
  DEBUG_PRINTLN(F("LED-Driver: WakeUp"));
  DEBUG_FLUSH();
}

/**
   Das Display manuell heller machen.
*/
void setDisplayBrighter() {
  if ((!settings.getUseLdr()) && (settings.getBrightness() < LDR_MAX_PERCENT)) {
    int8_t b = settings.getBrightness() + (LDR_MAX_PERCENT - LDR_MIN_PERCENT) / 9;
    if (b > LDR_MAX_PERCENT) {
      b = LDR_MAX_PERCENT;
    }
    setDisplayBrightness(b);
  }
}

/**
   Das Display dunkler machen.
*/
void setDisplayDarker() {
  if (!settings.getUseLdr() && (settings.getBrightness() > LDR_MIN_PERCENT)) {
    int8_t i = settings.getBrightness() - (LDR_MAX_PERCENT - LDR_MIN_PERCENT) / 9;
    if (i < LDR_MIN_PERCENT) {
      i = LDR_MIN_PERCENT;
    }
    setDisplayBrightness(i);
  }
}

/**
 * Die Helligkeit des Display einstellen.
 */
void setDisplayBrightness(byte brightness) {
    settings.setBrightness(brightness);
    settings.saveToEEPROM();
    ledDriver.setBrightness(brightness);
}

void enableFallBackCounter(byte timeoutSec) {
  fallBackCounter = timeoutSec;
}

void disableFallBackCounter(void) {
  fallBackCounter = 0;
}

/**
   Automatisches Zurückschalten von einer definierten Anzeige auf die Zeitanzeige nach einer
   festgelegten Zeitspanne jumpToNormalTimeout. Ist dieser Wert == 0, so findet kein
   automatischer Rücksprung statt.
   Achtung!: Als Ursprungsmodus (von dem Zurückgesprungen werden soll) nur Standardmodi verwenden.
*/
void updateFallBackCounter(void)
{
  fallBackCounter--;
  if (!fallBackCounter) {
    if ( (mode != EXT_MODE_NIGHT_OFF) && (mode != EXT_MODE_NIGHT_ON) ) {
      setMode(STD_MODE_NORMAL);
    }
  }
}

void incDecMinutes(boolean inc) {
  if (inc) {
    rtc.incMinutes();
  }
  else {
    rtc.decMinutes();
  }
  resetSeconds();
  DEBUG_PRINT(F("M is now "));
  DEBUG_PRINTLN(rtc.getMinutes());
  DEBUG_FLUSH();
}

void incDecHours(boolean inc) {
  if (inc) {
    rtc.incHours();
  }
  else {
    rtc.decHours();
  }
  resetSeconds();
  DEBUG_PRINT(F("H is now "));
  DEBUG_PRINTLN(rtc.getHours());
  DEBUG_FLUSH();
}

void resetSeconds() {
  rtc.setSeconds(0);
  rtc.writeTime();
  rtc.readTime();
  helperSeconds = rtc.getSeconds();
}

#if defined(REMOTE_BLUETOOTH) || !defined(REMOTE_NO_REMOTE)
void remoteAction(unsigned int irCode, IRTranslator* irTranslatorGeneric) {
  needsUpdateFromRtc = true;
  switch (irCode) {
    case REMOTE_BUTTON_TOGGLEBLANK:
      setDisplayToToggle();
      break;
    case REMOTE_BUTTON_BLANK:
      setDisplayToBlank();
      break;
    case REMOTE_BUTTON_RESUME:
      setDisplayToResume();
      break;
    case REMOTE_BUTTON_MODE:
      modePressed();
      break;
  }

  if ( (mode != STD_MODE_BLANK) && 
       (mode != STD_MODE_NIGHT) ) { 
    switch (irCode) {
      case REMOTE_BUTTON_MINUTE_PLUS:
        minutePlusPressed();
        break;
      case REMOTE_BUTTON_HOUR_PLUS:
        hourPlusPressed();
        break;
      case REMOTE_BUTTON_BRIGHTER:
        if (!settings.getUseLdr()) {
          if (STD_MODE_BRIGHTNESS == mode) {
            setDisplayBrighter();
          }
          else {
            setMode(STD_MODE_BRIGHTNESS);
          }
        }
        break;
      case REMOTE_BUTTON_DARKER:
        if (!settings.getUseLdr()) {
          if (STD_MODE_BRIGHTNESS == mode) {
            setDisplayDarker();
          }
          else {
            setMode(STD_MODE_BRIGHTNESS);
          }
        }
        break;
      case REMOTE_BUTTON_EXTMODE:
        if(mode < EXT_MODE_START) {
          setMode(EXT_MODE_START);
        }
        else {
          modePressed();
        }
        break;
      case REMOTE_BUTTON_SETCOLOR:
        if ((irTranslatorGeneric->getColor() == color_rgb_continuous) && (settings.getColor() == color_rgb_continuous)){
          settings.setColor(eColors::color_rgb_step);
        }
        else{
          settings.setColor(irTranslatorGeneric->getColor());
        }
#if defined(RGB_LEDS) || defined(RGBW_LEDS)
        if ( settings.getColor() > color_single_max){
          setMode(EXT_MODE_COLOR);
        }
#endif
        ledDriver.resetWheelPos();
        break;
      case REMOTE_BUTTON_SAVE:
        settings.saveToEEPROM();
        break;
      case REMOTE_BUTTON_SETMODE:
        setMode(irTranslatorGeneric->getMode());
        break;
      case REMOTE_BUTTON_REGION:
        if (EXT_MODE_LANGUAGE == mode) {
          minutePlusPressed();
        }
        else {
          setMode(EXT_MODE_LANGUAGE);
        }
        break;
      case REMOTE_BUTTON_SECONDS:
      #ifdef USE_STD_MODE_DATE
        if ( mode == STD_MODE_SECONDS )
        {
          setMode(STD_MODE_DATE);
        }
        else
        {
      #endif
          setMode(STD_MODE_SECONDS);
      #ifdef USE_STD_MODE_DATE
        }
      #endif
        break;
      case REMOTE_BUTTON_LDR:
        if (EXT_MODE_LDR_MODE == mode) {
          settings.setUseLdr(!settings.getUseLdr());
          if (!settings.getUseLdr()) {
            settings.setBrightness(ledDriver.getBrightness());
          }
        }
        else {
          setMode(EXT_MODE_LDR_MODE);
        }
        break;
      case REMOTE_BUTTON_TIME_H_PLUS:
        if(mode == STD_MODE_NORMAL) {
        incDecHours(true);
        } else if (mode >= EXT_MODE_START) {
          hourPlusPressed();
        }
        break;
      case REMOTE_BUTTON_TIME_H_MINUS:
        if (mode == STD_MODE_NORMAL) {
          incDecHours(false);
        }
        break;
      case REMOTE_BUTTON_TIME_M_PLUS:
        if(mode == STD_MODE_NORMAL) {
          incDecMinutes(true);
        } else if (mode >= EXT_MODE_START) {
          minutePlusPressed();
        }
        break;
      case REMOTE_BUTTON_TIME_M_MINUS:
        if (mode == STD_MODE_NORMAL) {
          incDecMinutes(false);
        }
        break;
      case REMOTE_BUTTON_TRANSITION:
        settings.setTransitionMode(irTranslatorGeneric->getTransition());
        ledDriver.demoTransition();
        break;
      default:
        break;
    }
  }

  if ( (irCode != REMOTE_BUTTON_TIME_H_PLUS) &&
       (irCode != REMOTE_BUTTON_TIME_M_PLUS) &&
       (irCode != REMOTE_BUTTON_HOUR_PLUS)   &&
       (irCode != REMOTE_BUTTON_MINUTE_PLUS) &&
       (irCode != REMOTE_BUTTON_MODE)        &&
       (irCode != REMOTE_BUTTON_EXTMODE) ) {
      switch (mode) {
        case STD_MODE_SECONDS:
          // Timeout für den automatischen Rücksprung von STD_MODE_SECONDS,
          // STD_MODE_DATE und STD_MODE_BRIGHTNESS zurücksetzen
        case STD_MODE_BRIGHTNESS:
#ifdef USE_STD_MODE_DATE
        case STD_MODE_DATE:
#endif
        case EXT_MODE_LANGUAGE:
        case EXT_MODE_LDR_MODE:
          enableFallBackCounter(settings.getJumpToNormalTimeout());
          break;
#if defined(RGB_LEDS) || defined(RGBW_LEDS)
        case EXT_MODE_COLOR:
          enableFallBackCounter(2);
          break;
#endif
        default:
          disableFallBackCounter();
          break;
      }
  }

  settings.saveToEEPROM();
}
#endif

void setMode(Mode a_mode) {
  mode = a_mode;
  lastMode = mode;
}

bool isCurrentTimeInNightRange() {
  return ( ( (settings.getNightModeTime(false)->getMinutesOfDay() < settings.getNightModeTime(true)->getMinutesOfDay()) && 
           ( (rtc.getMinutesOfDay() > settings.getNightModeTime(false)->getMinutesOfDay()) && 
             (rtc.getMinutesOfDay() < settings.getNightModeTime(true)->getMinutesOfDay()) ) ) ||
         ( (settings.getNightModeTime(false)->getMinutesOfDay() > settings.getNightModeTime(true)->getMinutesOfDay()) &&
           ( (rtc.getMinutesOfDay() > settings.getNightModeTime(false)->getMinutesOfDay()) ||
             (rtc.getMinutesOfDay() < settings.getNightModeTime(true)->getMinutesOfDay()) ) ) );
}



