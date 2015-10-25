/*
 * Defines.h
 *
 *  Created on: Oct 10, 2015
 *      Author: David
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#define BUTTON_DEBOUNCE 50
#define BUTTON_LONG_PRESS_TIME 2000

#define NUM_LCD_ROWS 2
#define NUM_LCD_COLS 16

#define DISPLAY_DELAY 500

#define COMMON_CATHODE_RGB_LCD  // Comment line if backlight LEDs are active HIGH.

// If you change this you have to change the interrupt used in REBLInterface for the encoder
#define ENCODER_INTERRUPT_PIN 2

// Assign pins as needed.
#define ENCODER_B_PIN 4
#define BUTTON_PIN 8
#define LCD_ENABLE_PIN A1
#define LCD_RS_PIN A3

#define HEART_BEAT_PIN A5
#define LCD_BLUE A0
#define LCD_GREEN A2
#define LCD_RED A4


#endif /* DEFINES_H_ */
