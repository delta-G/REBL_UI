#ifndef REBLInterface_h
#define REBLInterface_h

#include "Arduino.h"
#include "pins_arduino.h"
#include "Defines.h"


void initInterface();

void initButton();
void pollButton();
void clearButton();
boolean checkButton();
boolean checkButtonLongPress();
void buttonOn();
void buttonOff();
boolean isButtonOn();


void initEncoder();
void ISR_encoder_handler();
void encoderOn();
void encoderOff();
char checkRotaryEncoder();
void useRotaryEncoderASCII(char&);
boolean peekRotaryEncoder();
boolean isEncoderOn();




// call with mult == 0 allows only one step per call no matter how fast the encoder is turning
template<class T>
void useRotaryEncoder(T& var, int mult = 1)
{
  if(mult){
    var += (checkRotaryEncoder() * mult);
  } 
  else {
    int chek = checkRotaryEncoder();
    if (chek > 0) var += 1;
    if (chek < 0) var -= 1;
  }
}

template<class T>
void useRotaryEncoder(T& var, T mini, T maxi, int mult = 1)
{
  if(mult){
    var += (checkRotaryEncoder() * mult);
  } 
  else {
    int chek = checkRotaryEncoder();
    if (chek > 0) var += 1;
    if (chek < 0) var -= 1;
  }
  if (var > maxi){
    var = mini;
  } else if (var < mini) {
    var = maxi;
  }
  
}

#endif
