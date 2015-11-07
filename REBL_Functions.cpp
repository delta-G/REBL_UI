/*
 * REBL_Functions.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: David
 */
#include "REBL_Functions.h"

boolean enterNumber(int& aVal, const __FlashStringHelper* aText, int aMin, int aMax, int aMult) {
	static int state = 0;
	static int valueOut;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		valueOut = aVal;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {
			encoderOff();
			buttonOff();
			aVal = valueOut;
			state = 0;
			return true;
		}
		useRotaryEncoder(valueOut, aMin, aMax, aMult);
		displayLineLeft(0, aText);
		char numbuf[6];
		sprintf_P(numbuf, PSTR("%d"), valueOut);
		displayLineRight(1, numbuf);
		break;
	}

	} // end switch(state)

	if (reblMenu.isCancelled()) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}
	return false;
}

boolean enterString(char* aBuffer, uint8_t aLength) {

	static int charsEntered = 0;
	if (!isEncoderOn()) {
		encoderOn();
	}
	if (!isButtonOn()) {
		buttonOn();
	}

	if (checkButton()) {
		charsEntered++;
		if (charsEntered >= aLength) {
			charsEntered = 0;  // setup for next time
			return true;  // finished the job
		}
	} else {
		useRotaryEncoderASCII(aBuffer[charsEntered]);
	}

	if (reblMenu.isCancelled()) {
		encoderOff();
		buttonOff();
		return true;
	}

	return false;
}

boolean inputTime(time_t& var) {

	static int state = 0;
	static tmElements_t tmElem;
	breakTime(var, tmElem);

	switch (state) {

	case 0: {      // get set up
		encoderOn();
		buttonOn();
		state++;
		break;
	}
	case 1: {  // input the hours
		if (checkButton()) {
			state++;
			break;
		}
		int setHour = tmElem.Hour;   // Have to do this for wraparound
		useRotaryEncoder(setHour, 0, 23);  // Otherwise this uses unsigned vars.
		tmElem.Hour = setHour;
		setCursor(0, 1);
		break;
	}
	case 2: {  // input the minutes
		if (checkButton()) {
			state++;
			break;
		}
		int setMinute = tmElem.Minute;
		useRotaryEncoder(setMinute, 0, 59);
		tmElem.Minute = setMinute;
		setCursor(0, 4);
		break;
	}
	case 3: {   // input the year
		if (checkButton()) {
			state++;
			break;
		}
		useRotaryEncoder(tmElem.Year, 1);
		setCursor(1, 9);
		break;
	}
	case 4: {  // input the month

		if (checkButton()) {
			state++;
			break;
		}
		useRotaryEncoder(tmElem.Month, (byte) 1, (byte) 12); // OK to use unsigned vars since 0 will wrap around.
		setCursor(1, 1);
		break;
	}
	case 5: {  // input the day

		if (checkButton()) {
			state++;
			break;
		}
		byte monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if (LEAP_YEAR(tmElem.Year)) {
			monthDays[1] = 29;
		}
		useRotaryEncoder(tmElem.Day, (byte) 1, monthDays[tmElem.Month - 1]);
		setCursor(1, 4);
		break;
	}
	case 6: { // wrap it up and let the caller know we're done with his variable. var should still be OK from the last time we touched it.
		state = 0;
		encoderOff();
		buttonOff();
		showCursor(false);
		return true;
	}

	}  // end of switch (state)

	var = makeTime(tmElem);

	displayTime(var);

	if (reblMenu.isCancelled()) {
		state = 0;
		encoderOff();
		buttonOff();
		showCursor(false);
		return true;
	}


	return false;
}

void displayTime(time_t aTime) {

	tmElements_t tmElem;
	breakTime(aTime, tmElem);

	char outBuf[2][NUM_LCD_COLS + 1];
	sprintf_P(outBuf[0], PSTR("%02d:%02d:%02d"), tmElem.Hour, tmElem.Minute,
			tmElem.Second);
	sprintf_P(outBuf[1], PSTR("%02d-%02d-%04d"), tmElem.Month, tmElem.Day,
			tmElem.Year + 1970);

	displayLineLeft(0, outBuf[0]);
	displayLineLeft(1, outBuf[1]);

}


//  To be used as a timer by other functions on the menu
//  Can only have one active at a time.
boolean displayDelay(unsigned int delSeconds) {
	static uint8_t state = 0;
	static unsigned long startMil = millis();
	unsigned long curMil = millis();

	switch (state) {

	case 0: {
		startMil = millis();
		state++;
		return false;
	}
	case 1: {
		if (curMil - startMil >= delSeconds * 1000ul) {
			state = 0;
			return true;
		} else {
			return false;
		}
	}

	}  // end swithc (state)
	return false;
}
