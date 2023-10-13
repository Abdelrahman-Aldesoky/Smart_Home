/*****************LCD Interface.*****************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:21/08/2023.****************/
/************************************************/

#ifndef CLCD_INTERFACE_H_
#define CLCD_INTERFACE_H_

void CLCD_voidSendCommand(u8 Copy_u8Command);
void CLCD_voidSendData (u8 Copy_u8Data);
void CLCD_voidInit (void);
void CLCD_voidSetPostion(u8 Copy_u8Line,u8 Copy_u8Position);
void CLCD_voidSendString(char *Copy_pcString);
void CLCD_voidSendNumber(s64 Copy_s64Number);
void CLCD_voidDrawSpecialChar(u8 Copy_u8Location, u8 *Copy_pu8Pattern);

#define FIRST_LINE 0
#define SECOND_LINE 1

#define START_OF_SECOND_LINE_ADDRESS 0b01000000

#define SET_DDRAM_ADDRESS 0b10000000

#define SET_CGRAM_ADDRESS 0b01000000

//SHIFT CURSOR RIGHT
#define SHIFT_CURSOR_RIGHT 0b00010100

//SHIFT DISPLAY AND CURSOR RIGHT
#define SHIFT_RIGHT 0b00011100

//SHIFT CURSOR LEFT
#define SHIFT_CURSOR_LEFT 0b00010000

//SHIFT DISPLAY AND CURSOR LEFT
#define SHIFT_LEFT 0b00011000

//DISPLAY_CLEAR 0b00000001
#define DISPLAY_CLEAR 0b00000001

#endif
