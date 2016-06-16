/**
 * Configuration
 * Die Compiler-Einstellungen der Firmware an einer zentralen Stelle.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.5
 * @created  23.1.2013
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt
 * V 1.1:  - ENABLE_LDR und ENABLE_DCF entfernt.
 * V 1.2:  - PWM_DURATION von 50 auf 75 erhoeht.
 * V 1.3:  - MAX_BRIGHTNESS entfernt. Der LDR kann jetzt individuell skalieren.
 *         - PWM_DURATION zurueck auf 50.
 * V 1.4:  - ENABLE_ALARM jetzt im EEPROM.
 *         - PWM_DURATION an neue LDR-Klasse angepasst.
 *         - DCF77_SIGNAL_IS_INVERTED jetzt im EEPROM.
 * V 1.5:  - Diverse Config-Moeglichkeiten fuer die verschiedenen LED-Driver eingefuehrt.
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/*
 * Selten gebrauchte externe Modi einschalten
 */
// Datumanzeige
//#define USE_STD_MODE_DATE         
// Anzeige der vergangenen Stunden und Minuten seit der letzten erfolgreichen DCF-Synchronisation im Menü EXT_MODE_DCF_DEBUG (Standard: eingeschaltet).
#define USE_EXT_MODE_DCF_SYNC
// Einstellmöglichkeit Minuten LEDs im/gegen Uhrzeigersinn
//#define USE_EXT_MODE_CORNERS
// Einstellmöglichkeit Zeitverschiebung zum DCF Signal
#define USE_EXT_MODE_TIME_SHIFT
// LED Test spaltenweise
//#define USE_EXT_MODE_TEST
// DCF Debuganzeige
#define USE_EXT_MODE_DCF_DEBUG
// Datum manuell einstellen
#define USE_EXT_MODE_DATE_MANUALLY

// Im Menu auswählbare Sprachen aktivieren.
#define ENABLE_LANGUAGE_DE
#define ENABLE_LANGUAGE_CH
#define ENABLE_LANGUAGE_EN
#define ENABLE_LANGUAGE_FR
#define ENABLE_LANGUAGE_IT
#define ENABLE_LANGUAGE_NL
#define ENABLE_LANGUAGE_ES

/*
 * Zusätzliche Schalter, um neue Zusatzfunktionen ein- bzw. auszuschalten.
 */
// Automatischer Rücksprung von STD_MODE_BLANK, wenn DCF-Synchronisation erfolgreich war (Standard: eingeschaltet).
#define AUTO_JUMP_BLANK

// Automatischer Rücksprung von Abschalt-/Einschaltzeit zu EXT_MODE_NIGHT_OFF/EXT_MODE_NIGHT_ON 
#define FALL_BACK_TIME_NIGHT_MODE 5

// Schaltet für jede Eckled und das Alarmsymbol (Glockensymbol) nur die dazu passende Kathode und
// nicht immer alle ein. Dies verhindert das Glimmen bei richtiger Verdrahtung. Funktioniert noch nicht. (Standard: ausgeschaltet)
//#define USE_INDIVIDUAL_CATHODES

// Beim Einschalten leuchten alle LEDs während 5s
#define LED_TEST_INTRO_ENABLED

/*
 * Alarmfunktion einschalten?
 * Dazu muss ein Lautsprecher an D13 und GND und eine weitere 'Eck-LED' an die 5te Reihe.
 * Dann gibt es einen neuen Modus, direkt nach der normalen Zeitanzeige. Die neue LED
 * blinkt nach dem Moduswechsel und der Alarm ist eingeschaltet. Drueckt man jetzt M+ oder H+ stellt man
 * die Alarmzeit ein, angedeutet durch die blinkende Alarm-LED. Druckt man 10 Sekunden
 * keine Taste, hoert das Blinken auf und die normale Zeit wird wieder angezeigt.
 * Bei erreichen des Alarms piept der Lautpsrecher auf D13. Zum Ausschalten muss der
 * Modus-Taster gedrueckt werden.
 * Weiter unten kommen drei weitere DEFINEs:
 * - SPEAKER ist der Pin, an dem der Lautsprecher haengt.
 * - SPEAKER_FREQUENCY ist die Tonhoehe, wenn der Speaker ein Lautpsrecher ist.
 * - SPEAKER_IS_BUZZER: wenn einkommentiert wird davon ausgegangen, dass am Pin SPEAKER ein Buzzer haengt (Reichelt: SUMMER TDB 05).
 */
//#define ALARM
#define SPEAKER_FREQUENCY 200000
#define MAX_BUZZ_TIME_IN_MINUTES 10
#define SPEAKER_IS_BUZZER

/*
 * Die Status-LEDs koennen hier durch auskommentieren ausgeschaltet werden.
 * Default: eingeschaltet
 */
#define ENABLE_DCF_LED
#define ENABLE_SQW_LED

/*
 * Welches Board wird benutzt? Wird aktuell nur für das Pinmapping von LPD8806 und Neopixel gebraucht
 */
//#define BOARD_DEFAULT
#define BOARD_CLT
//#define BOARD_BBRTCAD
//#define BOARD_AMBBRTCAD

/*
 * Welcher LED-Treiber soll benutzt werden?
 */
//#define LED_DRIVER_DEFAULT
// #define LED_DRIVER_UEBERPIXEL
// #define LED_DRIVER_POWER_SHIFT_REGISTER
// #define LED_DRIVER_NEOPIXEL
// #define LED_DRIVER_DOTSTAR
 #define LED_DRIVER_LPD8806

/*
 * wenn LED_DRIVER_DEFAULT dann normalerweise Hardware mit UDN, 
 * wenn Hardware mit einzelnen FETs dann MODS_DRIVER  setzen
 */
#ifdef LED_DRIVER_DEFAULT
   //#define MOS_DRIVER
#endif

#if defined(LED_DRIVER_LPD8806) || defined(LED_DRIVER_NEOPIXEL)
//#define RGB_LEDS
#define RGBW_LEDS
//#define MATRIX_XXL
#endif

/*
 * Welche Uhr soll benutzt werden?
 */
#define DS1307
// #define DS3231

/*
 * Welcher Temperatursensor soll benutzt werden?
 */
#define TEMP_SENS_NONE
//#define TEMP_SENS_LM35
//#define TEMP_SENS_LM335

#ifndef TEMP_SENS_NONE
  // Temperaturanzeige
  #define USE_STD_MODE_TEMP
#endif 

/*
 * Welche IR-Fernbedienung soll benutzt werden?
 */
//#define REMOTE_NO_REMOTE
//#define REMOTE_SPARKFUN
//#define REMOTE_MOONCANDLES
//#define REMOTE_LUNARTEC
#define REMOTE_CLT

// kann separat oder zusätzlich zu einer IR-Fernbedienung genutzt werden
#define REMOTE_BLUETOOTH

// Möglichkeit um Events (Geburts-, Jahres-, Feiertage) mit Laufschrift und Effekt zu konfigurieren
//#define EVENTS

/*
 *
 * Seltener zu aendernde Einstellungen...
 *
 */

// ------------------ Default Display Driver ---------------------
/*
 * Dieser Schalter erhoeht die Helligkeit bei der Verwendung des Default-LED-Treibers (Shift-Register und UDN2981A),
 * indem er nicht beleuchtete Zeilen ueberspringt. (Tipp von Johannes)
 * Default: ausgeschaltet
 */
// #define SKIP_BLANK_LINES
/*
 * Die Zeit, die mit LDR::MAX_BRIGHTNESS multipliziert auf
 * eine Zeile aufgeteilt wird in Mikrosekunden.
 * (default = 8)
 */
#define PWM_DURATION 8
/*
 * Dieser Schalter stellt die Anzeige auf den Kopf, falls man die Kabel beim Anoden-
 * multiplexer nicht kreuzen moechte oder es vergessen hat.
 * Default: ausgeschaltet
 */
// #define UPSIDE_DOWN
/*
 * Schnelle Datenausgabe?
 * Default: Eingeschaltet.
 */
#define SHIFTREGISTER_TURBO
/*
 * Noch optimiertes Schreiben von 0 und FFFF?
 * Default: Ausgeschaltet
 */
// #define OPTIMIZED_FOR_DARKNESS

// ------------------ Tasten ---------------------
/*
 * Die Zeit in Millisekunden, innerhalb derer Prellungen der Taster nicht als Druecken zaehlen.
 * (Und damit auch die Tastaturwiederholrate)
 * Default: 300
 */
#define BUTTON_TRESHOLD 300

// ------------------ DCF77-Empfaenger ---------------------
/*
 * Fuer wieviele DCF77-Samples muessen die Zeitabstaende stimmen, damit das DCF77-Telegramm als gueltig zaehlt?
 * Default: 3 (min. 3 Minuten Synchronisationszeit).
 */
#define DCF77HELPER_MAX_SAMPLES 3
/*
 * Bei einer Verwendung eines analogen Pins die naechsten zwei Zeilen einkommentieren.
 * Default: ausgeschaltet
 */
// #define MYDCF77_SIGNAL_IS_ANALOG
// #define MYDCF77_ANALOG_SIGNAL_TRESHOLD 600
/*
 * Die Timing-Werte fuer den DCF77-Empfaenger.
 * Default: 1700, 185, 85.
 * Entfaellt mit der V 3.4.0
 */
// #define MYDCF77_SYNC_PAUSE_TRESHOLD 1700
// #define MYDCF77_200MS_TRESHOLD 185
// #define MYDCF77_100MS_TRESHOLD 85
/*
 * Die Telegrammlaenge.
 * Default: 59
 */
#define MYDCF77_TELEGRAMMLAENGE 59

/*
 * Die Sekunde wird in die entsprechende Zahl
 * von Signalbins unterteilt, um das Wegdriften
 * der internen RTC quantifizieren zu können.
 * Dieser Wert entspricht einer Auflösung.
 * ACHTUNG! Wird diese Zahl zu groß gewählt (> 30), 
 * überschreitet sie die Frequenz der loop().
 * Eine Steigerung der Auflösung ist dann nicht mehr
 * möglich, die Drift wird beliebig und es kann 
 * keine Zeiteinstellung via DCF mehr erfolgen.
 * Werte < 4 sind ebenso sinnlos.
 * Default: 11  (sinnvoll: 5 - 25)
 */
#define MYDCF77_SIGNAL_BINS 11
/*
 * Dieser Wert gibt an wie viele Sekunden
 * die Drift bestimmt, bevor sie korrigiert wird.
 * ACHTUNG! Ein zu großer Wert verhindert eine
 * wirksame Driftkorrektur. Bei einem zu kleinen
 * Wert hingegen wird eine erfolgreiche
 * Zeitsynchronisation praktisch unmöglich.
 * Default: 30  (sinnvoll: 10 - 300)
 */
#define MYDCF77_DRIFT_CORRECTION_TIME 30
/*
 * Ist das Signal invertiert (z.B. ELV-Empfaenger)?
 * Default: ausgeschaltet.
 * Jetzt im EEPROM! Hier hat es keine Bedeutung mehr!
 * #define MYDCF77_SIGNAL_IS_INVERTED
 */

/*
 * Startwerte fuer den Mittelwert.
 */
#ifdef LED_DRIVER_DEFAULT
#define MYDCF77_MEANSTARTVALUE 7
#endif
#ifdef LED_DRIVER_UEBERPIXEL
#define MYDCF77_MEANSTARTVALUE 1300
#endif
#ifdef LED_DRIVER_POWER_SHIFT_REGISTER
#define MYDCF77_MEANSTARTVALUE 10
#endif
#ifdef LED_DRIVER_NEOPIXEL
#define MYDCF77_MEANSTARTVALUE 450
#endif
#ifdef LED_DRIVER_DOTSTAR
#define MYDCF77_MEANSTARTVALUE 1280
#endif
#ifdef LED_DRIVER_LPD8806
#define MYDCF77_MEANSTARTVALUE 1280
#endif
// ------------------ Lichtabhaengiger Widerstand ---------------------
/*
 * Sollen die Grenzwerte vom LDR automatisch angepasst werden? Bei einem Neustart der QlockTwo kann
 * das Display flackern, dann muss man einmal ueber den LDR 'wischen', damit er verschiedene
 * Messwerte bekommt. Ohne AUTOSCALE werden die manuellen Min-/Max-Werte genommen.
 * Default: eingeschaltet.
 */
#define LDR_AUTOSCALE
#define LDR_MANUAL_MIN 0
#define LDR_MANUAL_MAX 1023
/*
 * Die Menge der Werte fuer die Glaettung. Mehr Werte bringen bessere Ergebnisse aber
 * brauchen auch mehr Zeit (und mehr Speicher! / Gleitender Mittelwert)
 * Ab der Version 3.4.2 obsolet.
 * Default: 32
 * #define LDR_MEAN_COUNT 32
 */
/*
 * Der Hysterese-Trashold.
 * Default: 50
 */
#define LDR_HYSTERESE 50
/*
 * Die LDR-Werte werden auf Prozent gemappt.
 * Hier koennen diese Werte beschnitten werden,
 * falls man eine minimale oder maximale
 * Helligkeit vorgeben moechte.
 */
#define LDR_MIN_PERCENT 5
#define LDR_MAX_PERCENT 100
/*
 * LDR-Check-Raten. Dieser Wert beeinflusst, wie schnell
 * sich die Displayhelligkeit an neue LDR-Werte anpasst
 * und ist vom LedDriver abhaengig.
 * Der Wert ist der Trashold in Millisekunden, bevor eine
 * neue Anpassung stattfindet.
 */
#ifdef LED_DRIVER_DEFAULT
#define LDR_CHECK_RATE 1
#endif
#ifdef LED_DRIVER_UEBERPIXEL
#define LDR_CHECK_RATE 7
#endif
#ifdef LED_DRIVER_POWER_SHIFT_REGISTER
#define LDR_CHECK_RATE 10
#endif
#ifdef LED_DRIVER_NEOPIXEL
#define LDR_CHECK_RATE 10
#endif
#ifdef LED_DRIVER_DOTSTAR
#define LDR_CHECK_RATE 10
#endif
#ifdef LED_DRIVER_LPD8806
#define LDR_CHECK_RATE 10
#endif

#endif
