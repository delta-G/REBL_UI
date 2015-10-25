/*
 * REBLMenu.cpp
 *
 *  Created on: Oct 10, 2015
 *      Author: David
 */

#include "REBLMenu.h"

REBLMenu reblMenu;

boolean REBLMenu::checkForCancel() {
	return checkButtonLongPress();
}

int REBLMenu::updateSelection() {
	int retval = 0;

	if (!isEncoderOn()) {
		encoderOn();
		return 0;
	}
	useRotaryEncoder(retval, 0);
	return retval;
}

boolean REBLMenu::selectionMade() {
	if (!isButtonOn()) {
		buttonOn();
		return false;
	}
	if (checkButton()) { // checkButton returned true, a selection is made.
		buttonOff();
		encoderOff();
		return true;
	}
	return false;
}

//  Currently only implemented to use 2 lines.  Could be expanded.
void REBLMenu::displayMenu() {

	char outBuf[NUM_LCD_COLS + 1];

	int currentLine = 0;

	outBuf[0] = '-';
	outBuf[1] = '>';
	getText(outBuf + 2, currentItemIndex);
	displayLineLeft(currentLine++, outBuf);
	outBuf[0] = ' ';
	outBuf[1] = ' ';
	getText(outBuf + 2, currentItemIndex + 1);

	displayLineLeft(currentLine++, outBuf);

}

