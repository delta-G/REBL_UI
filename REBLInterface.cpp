#include "REBLInterface.h"
#define NOT_AN_INTERRUPT 50

uint8_t buttonPin;
uint8_t encoderInterrupt;

byte bMask;

volatile byte *bReg;

volatile char encoderCounter;
boolean encoderIsOn = false;

boolean buttonPressed = false;
boolean buttonLongPressed = false;
boolean killPress = false;
boolean buttonIsOn = false;

void initInterface(uint8_t aButPin, uint8_t aIntPin, uint8_t aBPin) {
	initEncoder(aIntPin, aBPin);
	initButton(aButPin);
}

void initEncoder(uint8_t aIntPin, uint8_t aBPin) {
	pinMode(aIntPin, INPUT_PULLUP);
	pinMode(aBPin, INPUT_PULLUP);
	bMask = digitalPinToBitMask(aBPin);
	bReg = portInputRegister(digitalPinToPort(aBPin));
	encoderInterrupt = digitalPinToInterrupt(aIntPin);
}

void ISR_encoder_handler() {
	//  My encoder has a full cycle of 4 transitions per click.
	//  I only want one increment or decrement per click
	//  So we are only looking at one transition
	//  We're only using the falling edge on the interrupt pin.
	//  So we know ENCODER_INTERRUPT_PIN is LOW.
	//  If ENCODER_B_PIN is HIGH then they're different so decrement
	//  if(digitalRead(ENCODER_B_PIN) == HIGH) {
	if (*bReg & bMask) {
		encoderCounter--;
	} else {
		encoderCounter++;
	}
}

void encoderOn() {
	attachInterrupt(encoderInterrupt, ISR_encoder_handler, FALLING);
	encoderCounter = 0;
	encoderIsOn = true;
}

void encoderOff() {
	detachInterrupt(encoderInterrupt);
	encoderCounter = 0;
	encoderIsOn = false;
}

char checkRotaryEncoder() {
	if (encoderIsOn) {
		char retval = encoderCounter;
		encoderCounter = 0;
		return retval;
	} else {
		return 0;
	}
}


const char asciiSet[] PROGMEM = {'a','b','c','d','e','f','g','h','i',
		'j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G',
		'H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X',
		'Y','Z','.','\?','/','\\','*','-','_','+'     // INSERT MORE HERE, KEEP SPACE LAST
		, ' '};

const int sizeOfAsciiSet = sizeof(asciiSet) / sizeof(asciiSet[0]);



int findCharIndexInAsciiSet(char aVal) {
	for (int i = 0; i < sizeOfAsciiSet; i++){
		char chr = pgm_read_byte_near(asciiSet + i);
		if(chr == aVal){
			return i;
		}
	}
	return 0;  // return 0 by default.
}

void useRotaryEncoderASCII(char& var) {
	int cre = checkRotaryEncoder();
	int chr = findCharIndexInAsciiSet(var);

	chr += cre;
	chr %= sizeOfAsciiSet;
	while(chr < 0){   // fix it if it went negative.  Use while just in case it went way negative.
		chr += sizeOfAsciiSet;
	}

	var = pgm_read_byte_near(asciiSet + chr);
}

//  This function could be better written with
//  an array of acceptable characters in progmem.
//void useRotaryEncoderASCII(char& var) {
//
//	int cre = checkRotaryEncoder();
//
//	var += cre;
//
//	if (cre >= 0) {  // Going up
//		if (var < ' ') {
//			var = ' ';
//		} else if (var < '0' && var > ' ') {
//			var = '0';
//		} else if (var < 'A' && var > '9') {
//			var = 'A';
//		} else if (var < 'a' && var > 'Z') {
//			var = 'a';
//		} else if (var > 'z') {
//			var = ' ';
//		}
//	} else {  // Going down
//		if (var < ' ') {
//			var = 'z';
//		} else if (var < '0' && var > ' ') {
//			var = ' ';
//		} else if (var < 'A' && var > '9') {
//			var = '9';
//		} else if (var < 'a' && var > 'Z') {
//			var = 'Z';
//		} else if (var > 'z') {
//			var = 'z';
//		}
//	}
//}

boolean peekRotaryEncoder() {
	if (encoderCounter) {
		return true;
	}
	return false;
}

boolean isEncoderOn() {
	return encoderIsOn;
}

void initButton(uint8_t aButPin) {
	buttonPin = aButPin;
	pinMode(buttonPin, INPUT_PULLUP);
}

void pollButton() {
	static boolean buttonState = HIGH;
	static boolean lastState = HIGH;
	static unsigned long pressTime = 0;
	if (buttonIsOn) {
		boolean curState = digitalRead(buttonPin);
		if (curState != lastState) {
			pressTime = millis();
		}
		if ((millis() - pressTime) > BUTTON_DEBOUNCE) {  //button has been in present state long enough
			if ((buttonState == LOW) && (curState == HIGH)) { // button was pressed and is being released
				if (!killPress) {   //Prevents registering a press on the release from a long press
					buttonPressed = true;    // register a button press
				}
				killPress = false;
			}
			// buttonState is the fully debounced state of the button for the rest of the program to use
			buttonState = curState;
		}

		if ((buttonState == LOW) && (millis() - pressTime > BUTTON_LONG_PRESS_TIME)) {
			buttonLongPressed = true;
			killPress = true;    //  Force to skip next press when button is released after long press
		}
		lastState = curState;
	}
}

void clearButton() {
	buttonPressed = false;
	buttonLongPressed = false;
}

boolean checkButtonLongPress() {
	boolean retval = buttonLongPressed;
	buttonLongPressed = false;
	return retval;
}

boolean checkButton() {
	boolean retval = buttonPressed;
	buttonPressed = false;
	return retval;
}

void buttonOn() {
	buttonIsOn = true;
	clearButton();
}

void buttonOff() {
	buttonIsOn = false;
	clearButton();
}

boolean isButtonOn() {
	return buttonIsOn;
}

