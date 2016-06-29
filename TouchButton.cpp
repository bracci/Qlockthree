#include "TouchButton.h"
#include "Configuration.h"


bool TouchButton::pressedRaw()
{
    return getState();
}


bool TouchButton::pressed()
{
    if (getState()) {
        if(millis() - _lastPressTime > BUTTON_TRESHOLD) {
            _lastPressTime = millis();
            return true;
        }
    }
    return false;
}

/**
 * Wurde der Taster gedrueckt?
 */
boolean TouchButton::getState() {
    if (lastRead == millis())
        return state;
    boolean _retVal = false;

    int t = touchRead(pin);
    samples++;

    const float alpha=0.1;

    // Moving exponential average
    idleAvg = (alpha * t) + (1.0 - alpha) * idleAvg;

    if (t > idleAvg + TOUCHTHRESHOLD && samples > TOUCHSAMPLES)
        samplesTouched++;
    else
        samplesTouched=0;

    if (t > idleAvg + TOUCHTHRESHOLD)
        Serial.printf("%d: t %d, tavg %.1f, %d -> %d\r\n", pin, t, idleAvg,(int) (t- idleAvg), samplesTouched);
    
    state = (samplesTouched > NUMTOUCHSAMPLES);
    return state;

}

