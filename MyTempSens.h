/**
 * MyTempSens.h
 * Klasse fuer den Zugriff auf den Temperatursensor LM35 & LM335
 *
 * @mc       Arduino
 * @autor    Manuel Bracher
 * @version  1.0
 * @created  30.06.2016
 * @updated  
 *
 * Versionshistorie:
 * V 1.1:
 */
 
#ifndef MYTEMPSENS_H
#define MYTEMPSENS_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define NR_OF_SAMPLES 10

class MyTempSens {
public:
  MyTempSens(byte pin);
  void takeSample();
  byte getTempC();
  int getTempK();
  void initLM35();
  void initLM335();
private:
  byte _pin;
  int _tempSamplesSum;
  int _tempSamples[NR_OF_SAMPLES];
  byte _idx;
  byte _slope;
  int _offsetC;
};

#endif //MYTEMPSENS_H
