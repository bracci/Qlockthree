/**
 * ShiftRegister
 * Mit der ShiftRegister-Klasse wird das Ausgeben der Bitmuster auf die
 * 74HC595-Shift-Register gekapselt.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.7
 * @created  24.2.2011
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.1:  - Schnelle DigitalWrite-Methoden eingefuehrt.
 *           Ueber die Definition von SHIFTREGISTER_TURBO
 *           kann es eingeschaltet werden.
 * V 1.2:  - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
 * V 1.3:  - Optimierung hinsichtlich Speicherbedarf.
 * V 1.4:  - Verbessertes Debugging.
 * V 1.5:  - optimiertes Schreiben fuer die Faelle data==0 und data==65535.
 * V 1.6:  - shiftOutABit eiungefuehrt.
 * V 1.7:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
// #include "pins_arduino.h"
#include "ShiftRegister.h"
#include "Configuration.h"

// #define DEBUG
#include "Debug.h"

/**
 * Initialisierung mit den Pins fuer Serial-Data, Serial-Clock und Store-Clock (Latch)
 */
ShiftRegister::ShiftRegister(byte dataPin, byte clockPin, byte latchPin) {
#ifdef SHIFTREGISTER_TURBO
    DEBUG_PRINTLN(F("ShiftRegister is in TURBO-MODE."));
    DEBUG_FLUSH();
#else
    DEBUG_PRINTLN(F("ShiftRegister is in SLOW-MODE."));
    DEBUG_FLUSH();
#endif

    // slow version
    _dataPin = dataPin;
    _clockPin = clockPin;
    _latchPin = latchPin;
    pinMode(_latchPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);

    // fast version
    _dataBit = digitalPinToBitMask(dataPin);
    _dataPort = digitalPinToPort(dataPin);
    _dataOut = portOutputRegister(_dataPort);

    _clockBit = digitalPinToBitMask(clockPin);
    _clockPort = digitalPinToPort(clockPin);
    _clockOut = portOutputRegister(_clockPort);

    _latchBit = digitalPinToBitMask(latchPin);
    _latchPort = digitalPinToPort(latchPin);
    _latchOut = portOutputRegister(_latchPort);
}

/**
 * Ein WORD (16 Bit) ausgeben
 */
void ShiftRegister::shiftOut(word data) {
#ifdef OPTIMIZED_FOR_DARKNESS
    if (data == (word) 65535) {
        fastDigitalWriteToData(HIGH);
        for (byte i = 0; i < 16; i++) {
            fastDigitalWriteToClock(LOW);
            fastDigitalWriteToClock(HIGH);
        }
        return;
    }
    if (data == (word) 0) {
        fastDigitalWriteToData(LOW);
        for (byte i = 0; i < 16; i++) {
            fastDigitalWriteToClock(LOW);
            fastDigitalWriteToClock(HIGH);
        }
        return;
    }
#endif
#ifdef SHIFTREGISTER_TURBO
    for (byte b = 0; b < 16; b++) {
        fastDigitalWriteToClock(LOW);
        if (data & (1 << b)) {
            fastDigitalWriteToData(HIGH);
        } else {
            fastDigitalWriteToData(LOW);
        }
        fastDigitalWriteToClock(HIGH);
    }
#else
    for (byte b = 0; b < 16; b++) {
        digitalWrite(_clockPin, LOW);
        if (data & (1 << b)) {
            digitalWrite(_dataPin, HIGH);
        } else {
            digitalWrite(_dataPin, LOW);
        }
        digitalWrite(_clockPin, HIGH);
    }
#endif
}

/**
 * Ein einzelnes Bit rausshiften...
 */
void ShiftRegister::shiftOutABit(boolean bitIsSet) {
#ifdef SHIFTREGISTER_TURBO
    fastDigitalWriteToClock(LOW);
    if (bitIsSet) {
        fastDigitalWriteToData(HIGH);
    } else {
        fastDigitalWriteToData(LOW);
    }
    fastDigitalWriteToClock(HIGH);
#else
    digitalWrite(_clockPin, LOW);
    if (bitIsSet) {
        digitalWrite(_dataPin, HIGH);
    } else {
        digitalWrite(_dataPin, LOW);
    }
    digitalWrite(_clockPin, HIGH);
#endif
}

/**
 * Vorbereitung fuer die Ausgabe
 */
void ShiftRegister::prepareShiftregisterWrite() {
#ifdef SHIFTREGISTER_TURBO
    // fastDigitalWriteToData(LOW);
    // fastDigitalWriteToClock(LOW);
    fastDigitalWriteToLatch(LOW);
#else
    // digitalWrite(_dataPin, LOW);
    // digitalWrite(_clockPin, LOW);
    digitalWrite(_latchPin, LOW);
#endif
}

/**
 * Abschliessen der Ausgabe
 */
void ShiftRegister::finishShiftregisterWrite() {
#ifdef SHIFTREGISTER_TURBO
    // fastDigitalWriteToData(LOW);
    // fastDigitalWriteToClock(LOW);
    fastDigitalWriteToLatch(HIGH);
#else
    // digitalWrite(_dataPin, LOW);
    // digitalWrite(_clockPin, LOW);
    digitalWrite(_latchPin, HIGH);
#endif
}

/**
 * Digitale Ausgaenge im Turbo-Mode!
 */
void ShiftRegister::fastDigitalWriteToData(uint8_t val) {
    uint8_t oldSREG = SREG;
    cli();
    if (val == LOW) {
        *_dataOut &= ~_dataBit;
    } else {
        *_dataOut |= _dataBit;
    }
    sei();
    SREG = oldSREG;
}

void ShiftRegister::fastDigitalWriteToLatch(uint8_t val) {
    uint8_t oldSREG = SREG;
    cli();
    if (val == LOW) {
        *_latchOut &= ~_latchBit;
    } else {
        *_latchOut |= _latchBit;
    }
    sei();
    SREG = oldSREG;
}

void ShiftRegister::fastDigitalWriteToClock(uint8_t val) {
    uint8_t oldSREG = SREG;
    cli();
    if (val == LOW) {
        *_clockOut &= ~_clockBit;
    } else {
        *_clockOut |= _clockBit;
    }
    sei();
    SREG = oldSREG;
}
