/*******************LCD Config*******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:21/08/2023.****************/
/************************************************/

#ifndef _CLCD_CONFIG_H_
#define _CLCD_CONFIG_H_

/*CLCD CTRL and data
 * PORT OPTIONS:
 * 1-PORTA_REG
 * 2-PORTB_REG
 * 3-PORTC_REG
 * 4-PORTD_REG
 */
#define CLCD_DATA_PORT PORTA_REG
#define CLCD_CTRL_PORT PORTB_REG

/*CLCD CTRL
 * PIN OPTIONS:
 * 1-PIN0
 * 2-PIN1
 * 3-PIN2
 * 4-PIN3
 * 5-PIN4
 * 6-PIN5
 * 7-PIN6
 * 8-PIN7
 */
#define RS PIN5
#define RW PIN6
#define EN PIN7

// START OF 8 BIT MODE
/*
 * FUNCTION_SET OPTIONS:
 * *****8 bit mode******
 * 1-0b00110000 one line display with 5x7 dot character font
 * 2-0b00110100 one line display with 5x10 dot character font
 * 3-0b00111000 two line display with 5x7 dot character font
 * 4-0b00111100 two line display with 5x10 dot character font
 */
#define FUNCTION_SET 0b00111100

/*
 * DISPLAY_ON/OFF OPTIONS:
 * ********ON********
 * 1-0b00001111 cursor on with blink on
 * 2-0b00001110 cursor on with blink off
 * 3-0b00001101 cursor off with blink on
 * 4-0b00001100 cursor off with blink off
 * *******OFF*******
 * 1-0b00000000 everything off
 */
#define DISPLAY_ON 0b00001100
#define DISPLAY_OFF 0b00000000

/*
 * ENTRY_MODE_SET OPTIONS:
 * 1-0b00000100 fixed cursor fixed display
 * 2-0b00000101 fixed cursor moving display
 * 3-0b00000110 moving cursor fixed display
 * 4-0b00000111 moving cursor moving display
 */
#define ENTRY_MODE_SET 0b00000110
// END OF 8 BIT MODE

#endif
