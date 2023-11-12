/******************LCD Program*******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:21/08/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "CLCD_private.h"
#include "CLCD_config.h"
#include "CLCD_interface.h"
#include <util/delay.h>


void CLCD_voidInit(void) // LCD Initialize function
{
	/*******************Start of port and pins Initialization*******************/

	/*Initialize CLCD control pins as given in the CLCD_config file*/
	DIO_voidSetPinDirection(CLCD_CTRL_PORT, RS, PIN_DIRECTION_OUTPUT); // SET CLCD CTRL port > pin RS direction as output
	DIO_voidSetPinDirection(CLCD_CTRL_PORT, RW, PIN_DIRECTION_OUTPUT); // SET CLCD CTRL port > pin RW direction as output
	DIO_voidSetPinDirection(CLCD_CTRL_PORT, EN, PIN_DIRECTION_OUTPUT); // SET CLCD CTRL port > pin EN direction as output

	/*Initialize CLCD data port given in the CLCD_config file*/
	DIO_voidSetPortDirection(CLCD_DATA_PORT, PORT_DIRECTION_OUTPUT); // Set CLCD DATA PORT direction as output
	_delay_ms(20);

	/*******************end of CLCD port and pins Initialization*******************/

	CLCD_voidSendCommand(FUNCTION_SET); // Initialization of 16X2 LCD in 8bit mode
	_delay_us(50);
	CLCD_voidSendCommand(DISPLAY_ON); // Display ON Cursor OFF Blink OFF
	_delay_us(50);
	CLCD_voidSendCommand(DISPLAY_CLEAR); // clear display
	_delay_ms(2);
	CLCD_voidSendCommand(ENTRY_MODE_SET); // Auto Increment cursor
	_delay_ms(2);
}

void CLCD_voidSendCommand(u8 Copy_u8Command)
{
	DIO_voidSetPinValue(CLCD_CTRL_PORT, RS, PIN_VALUE_LOW);	 // RS=0
	DIO_voidSetPinValue(CLCD_CTRL_PORT, RW, PIN_VALUE_LOW);	 // RW=0
	DIO_voidSetPortValue(CLCD_DATA_PORT, Copy_u8Command);	 // send the data for the command on data port
	DIO_voidSetPinValue(CLCD_CTRL_PORT, EN, PIN_VALUE_HIGH); // EN=1
	_delay_us(1);
	DIO_voidSetPinValue(CLCD_CTRL_PORT, EN, PIN_VALUE_LOW); // EN=0
	_delay_ms(2);
}

void CLCD_voidSendData(u8 Copy_u8Data)
{
	DIO_voidSetPinValue(CLCD_CTRL_PORT, RS, PIN_VALUE_HIGH); // RS=1
	DIO_voidSetPinValue(CLCD_CTRL_PORT, RW, PIN_VALUE_LOW);	 // RW=0
	DIO_voidSetPortValue(CLCD_DATA_PORT, Copy_u8Data);		 // send the actual data on data port
	DIO_voidSetPinValue(CLCD_CTRL_PORT, EN, PIN_VALUE_HIGH); // EN=1
	_delay_us(1);
	DIO_voidSetPinValue(CLCD_CTRL_PORT, EN, PIN_VALUE_LOW); // EN=0
	_delay_ms(2);
}

void CLCD_voidSetPostion(u8 Copy_u8Line, u8 Copy_u8Position)
{
	char Local_u8Position = 0;
	if (Copy_u8Line == FIRST_LINE)
	{
		Local_u8Position = Copy_u8Position;
	}
	else if (Copy_u8Line == SECOND_LINE)
	{
		Local_u8Position = START_OF_SECOND_LINE_ADDRESS + Copy_u8Position;
	}
	CLCD_voidSendCommand(SET_DDRAM_ADDRESS + Local_u8Position);
}

void CLCD_voidSendString(char *Copy_pcString)
{
	u8 Local_u8Iterator = 0;
	for (Local_u8Iterator = 0; Copy_pcString[Local_u8Iterator] != 0; Local_u8Iterator++)
	{
		CLCD_voidSendData(Copy_pcString[Local_u8Iterator]);
	}
}

void CLCD_voidSendNumber(s64 Copy_s64Number)
{
	u8 Local_u8arr[18];
	s8 Local_u8Iterator = 0, Local_u8Iterator2 = 0;
	if (Copy_s64Number == 0)
	{
		CLCD_voidSendData('0');
	}
	else if (Copy_s64Number < 0) // if a negative number is entered
	{
		Copy_s64Number = -Copy_s64Number; // turn it into positive to process it
		while (Copy_s64Number)
		{
			Local_u8arr[Local_u8Iterator] = (Copy_s64Number % 10) + '0';
			Copy_s64Number = Copy_s64Number / 10;
			Local_u8Iterator++;
		}
		Local_u8arr[Local_u8Iterator] = '-'; // put - sign at the end of it since the next loop to display the number in reverse
		for (Local_u8Iterator2 = Local_u8Iterator; Local_u8Iterator2 >= 0; Local_u8Iterator2--)
		{
			CLCD_voidSendData(Local_u8arr[Local_u8Iterator2]);
		}
	}
	else
	{
		while (Copy_s64Number)
		{
			Local_u8arr[Local_u8Iterator] = (Copy_s64Number % 10) + '0';
			Copy_s64Number = Copy_s64Number / 10;
			Local_u8Iterator++;
		}
		for (Local_u8Iterator2 = Local_u8Iterator; Local_u8Iterator2 > 0; Local_u8Iterator2--)
		{
			CLCD_voidSendData(Local_u8arr[Local_u8Iterator2 - 1]);
		}
	}
}

void CLCD_voidDrawSpecialChar(u8 Copy_u8Location, u8 *Copy_pu8Pattern)
{
	u8 Local_u8Iterator = 0;
	if (Copy_u8Location < 8) // only 8 custom chars can be stored
	{
		CLCD_voidSendCommand(SET_CGRAM_ADDRESS + (Copy_u8Location * 8)); // go to CGRAM ADDRESS To Store custom chars

		for (Local_u8Iterator = 0; Local_u8Iterator < 8; Local_u8Iterator++) // Write on CGRAM Custom chars
		{
			CLCD_voidSendData(Copy_pu8Pattern[Local_u8Iterator]);
		}
	}
}
