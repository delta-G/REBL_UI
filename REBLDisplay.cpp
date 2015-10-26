#include "REBLDisplay.h"

#ifdef USING_SERIAL
extern HardwareSerial Serial;
#endif

LiquidCrystal_SPI_8Bit *LCD;
uint8_t redPin;
uint8_t greenPin;
uint8_t bluePin;

char lineBuffer[NUM_LCD_ROWS][NUM_LCD_COLS + 1]; // Leave an extra space for terminating null

color_vars current_color;

boolean cursorActive = false;
uint8_t cursorRow;
uint8_t cursorCol;

void initLCD(uint8_t aRsPin, uint8_t aEnablePin, uint8_t aRed, uint8_t aGreen, uint8_t aBlue) {
	redPin = aRed;
	greenPin = aGreen;
	bluePin = aBlue;
	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);

#ifdef USING_SERIAL
	Serial.begin(19200);
#else
	LCD = new LiquidCrystal_SPI_8Bit(aRsPin, aEnablePin);
	LCD->begin(NUM_LCD_COLS, NUM_LCD_ROWS);
	LCD->noCursor();
	LCD->clear();
	setColor(BLUE);
#endif
}

void setColor(color_vars color) {
	current_color = color;
}



void doBacklightColor() {
#ifdef BACKLIGHT_ACTIVE_LOW  // Uses LOW to turn on an LED
	digitalWrite(redPin, !(current_color & 1));
	digitalWrite(greenPin, !(current_color & 2));
	digitalWrite(bluePin, !(current_color & 4));

#else   // Uses HIGH to turn on an LED
	digitalWrite(redPin, (current_color & 1));
	digitalWrite(greenPin, (current_color & 2));
	digitalWrite(bluePin, (current_color & 4));
#endif
}



void doDisplay() {
//	static unsigned long preMil = millis();
//	unsigned long curMil = millis();
//	if(curMil - preMil >= DISPLAY_DELAY){
#ifdef USING_SERIAL
		Serial.print(F("0: "));
		Serial.print(lineBuffer[0]);
		Serial.println(F("\\"));
		Serial.print(F("1: "));
		Serial.print(lineBuffer[1]);
		Serial.print(F("\\ RAM = "));
		Serial.println(freeRam());
		Serial.println(F("**"));
		Serial.print(F("Backlight = "));
		Serial.println(current_color);
		Serial.println(F("**"));
		delay(100);
#else
		LCD->clear();
		doBacklightColor();
		for (int i = 0; i < NUM_LCD_ROWS; i++) {
			lineBuffer[i][NUM_LCD_COLS] = 0;   // Make sure the null is there.
			LCD->setCursor(0, i);
			LCD->print(lineBuffer[i]);
		}
#endif
		doCursor();
//	}
}

void displayTextLeft(int row, int startcol, int colwidth, const char* message) {
	boolean over = false;
	for (int i = startcol; i < startcol + colwidth; i++) {
		if (!over) {
			char c = message[i];
			if (c == 0) {
				over = true;
				lineBuffer[row][i] = ' ';
			} else {
				lineBuffer[row][i] = c;
			}
		} else {
			lineBuffer[row][i] = ' '; // fill with spaces to clear anything else on the line.
		}
	}
}

void displayTextLeft(int row, int startcol, int colwidth, const __FlashStringHelper* message) {
	boolean over = false;
	PGM_P p = reinterpret_cast<PGM_P>(message);
	for(int i=startcol; i < startcol + colwidth; i++) {
		if (!over) {
			char c = pgm_read_byte(&p[i]);
			if(c == 0) {
				over = true;
				lineBuffer[row][i] = ' ';
			}
			else {
				lineBuffer[row][i] = c;
			}
		}
		else {
			lineBuffer[row][i] = ' '; // fill with spaces to clear anything else on the line.
	}
}
}

void displayTextRight(int row, int startcol, int colwidth, const char* message) {
	int numChars = strlen(message);
	int numSpaces = 0;

	if (colwidth > 16 - startcol) {
		colwidth = 16 - startcol;
	}
	numSpaces = colwidth - numChars;
	if (numSpaces < 0) {
		numSpaces = 0;
	}
	for (int i = startcol; i < startcol + colwidth; i++) {
		if (i < numSpaces) {
			lineBuffer[row][i] = ' ';
		} else {
			lineBuffer[row][i] = message[i - numSpaces];
		}
	}
}

void displayTextRight(int row, int startcol, int colwidth, const __FlashStringHelper* message) {
	int numChars = strlen_P((const char*)message);
	int numSpaces = 0;
	PGM_P p = reinterpret_cast<PGM_P>(message);

	if (colwidth > 16 - startcol) {
		colwidth = 16 - startcol;
	}
	numSpaces = colwidth - numChars;
	if (numSpaces < 0) {
		numSpaces = 0;
	}
	for(int i=startcol; i< startcol + colwidth; i++) {
		if(i < numSpaces) {
			lineBuffer[row][i] = ' ';
		}
		else {
			lineBuffer[row][i] = pgm_read_byte(&p[i - numSpaces]);
		}
	}
}

void displayLineLeft(int row, const char* message) {
	displayTextLeft(row, 0, NUM_LCD_COLS, message);
}

void displayLineLeft(int row, const __FlashStringHelper* message) {
	displayTextLeft(row, 0, NUM_LCD_COLS, message);
}

void displayLineRight(int row, const char* message) {
	displayTextRight(row, 0, NUM_LCD_COLS, message);
}

void displayLineRight(int row, const __FlashStringHelper* message) {
	displayTextRight(row, 0, NUM_LCD_COLS, message);
}

void showCursor(boolean aBool){
	cursorActive = aBool;
}

void setCursor(uint8_t arow, uint8_t acol){
	cursorRow = arow;
	cursorCol = acol;
	cursorActive = true;
}


void doCursor(){
	if(cursorActive){
#ifdef USING_SERIAL
	Serial.print("Cursor at ");
	Serial.print(cursorRow);
	Serial.println(cursorCol);
#else
	LCD->cursor();
	LCD->setCursor(cursorCol, cursorRow);
#endif
	}
	else {
#ifdef USING_SERIAL

#else
		LCD->noCursor();
#endif
	}
}

