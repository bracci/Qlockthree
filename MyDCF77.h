/**
 * MyDCF77
 * Klasse fuer die Auswertung des DCF77-Zeitsignals.
 * Diese Klasse benötigt eine externe 1-Sekunden-Basis und einen
 * zusätzlichen Timer, der im Default-Fall alle 100ms ein Flag setzt.
 * Dieser feste Takt wird benötigt, um die zwingend erforderliche
 * Driftkorrektur durchführen zu können.
 * Diese Klasse geht nicht von einem 'sauberen' Signal aus,
 * und ermittelt die HIGH/LOWs statistisch.
 * 
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Mueller
 *           Vorlage von: Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.2
 * @created  21.3.2016
 * @updated  13.04.2016
 *
 * Versionshistorie:
 * V 1.0:   * Signalauswertealgoritmus komplett neu geschrieben! *
 *          Zuverlässigkeit der Zeitsynchronisation erhöht:
 *          - Drifts in der Phase werden durch eine automatische Offsetkorrektur bereinigt.
 *          - Damit ist die Erkennung der Schaltsekunde nach spätestens einer Minute
 *            zuverlässig möglich.
 *            Das Fehlen der korrekten Erkennung dieser Schaltsekunde in früheren Versionen
 *            verhinderte eine zuverlässige Zeitsynchronisation.
 *          - Deutlich exaktere Einstellung der Zeit dank Driftkorrektur möglich.        
 * V 1.1:   - Funktion für EXT_MODE_DCF_DEBUG eingeführt.
 *          - Umschaltung von Timer1 auf Timer2 in Header-Datei möglich.
 * V 1.2:   * Die Driftkorrektur benötigt keinen Timer mehr! *
 */
#ifndef MYDCF77_H
#define MYDCF77_H

#include "Arduino.h"
#include "Configuration.h"
#include "TimeStamp.h"

class MyDCF77 : public TimeStamp {

public:
    MyDCF77(byte signalPin, byte statusLedPin);

    void statusLed(boolean on);

    boolean poll(boolean signalIsInverted);

    unsigned int getDcf77LastSuccessSyncMinutes();
    void setDcf77SuccessSync();

    byte getBitAtPos(byte pos);
    void updateDcf77ErrorCorner(boolean signalIsInverted);
    byte getDcf77ErrorCorner();

    boolean signal(boolean signalIsInverted);

private:
    byte _signalPin;
    byte _statusLedPin;
    unsigned int _nPolls = 0;

    static byte DCF77Factors[];

    byte _bitsPointer;
    byte _bits[MYDCF77_TELEGRAMMLAENGE+1];
    
    unsigned int _bins[MYDCF77_SIGNAL_BINS];
    int _binsPointer = 0;
    int _binsOffset = 0;
    byte _driftTimer = 0;

    int _updateFromDCF77 = -1;

    unsigned long _dcf77LastTime;
    unsigned long _dcf77Freq;

    unsigned long _dcf77lastSyncTime = 0;
    boolean _toggleSignal = false;
    byte _errorCorner = 0;

    void newCycle();
    void OutputSignal(unsigned int average, unsigned int imax, unsigned int isum);

    boolean decode();

    void clearBits();
    void clearBins();
};

#endif

