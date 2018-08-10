/*
 * REBL_Functions.h
 *
 *  Created on: Oct 20, 2015
 *      Author: David
 */

#ifndef REBL_FUNCTIONS_H_
#define REBL_FUNCTIONS_H_

#include "arduino.h"
#include "REBLDisplay.h"
#include "Defines.h"
#include "REBLInterface.h"
#include "REBLMenu.h"


#include "TimeLib.h"


#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )


boolean enterNumber(int& aVal, const __FlashStringHelper* aText, int aMin, int aMax, int aMult);
boolean enterString(char* aBuffer, uint8_t aLength);
void displayTime(time_t aTime);
boolean inputTime(time_t& var);
boolean displayDelay(unsigned int delSeconds);

#endif /* REBL_FUNCTIONS_H_ */
