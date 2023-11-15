/*****************BL_program.c*******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "../../MCAL/USART/USART_interface.h"
#include "../CLCD/CLCD_interface.h"
#include "BL_config.h"
#include "BL_private.h"
#include "BL_interface.h"
#include <util/delay.h>
#include <string.h>
#include <ctype.h>

void BL_voidReceive(u8 Selector, u8 arr[], u8 *arr_length)
{
	u8 index = 0;
	u8 input = 0;
	u8 user_input[ARR_LENGTH];

	while (input != '\r' && input != '\n')
	{
		input = USART_voidReceive();
		if (isprint(input) && index < MAX_LENGTH)
		{
			if (Selector == _USERNAME)
			{
				CLCD_voidSetPostion(SECOND_LINE, index);
				CLCD_voidSendData(input);
			}
			else
			{
				CLCD_voidSetPostion(SECOND_LINE, index);
				CLCD_voidSendData(input);
				_delay_ms(5);
				CLCD_voidSetPostion(SECOND_LINE, index);
				CLCD_voidSendString("*");
			}
			USART_voidSend(input);
			user_input[index] = input;
			index++;
		}
		else if (input == '\b' && index != 0)
		{
			USART_voidSend(input);
			index--;
			CLCD_voidSetPostion(SECOND_LINE, index);
			CLCD_voidSendData(' ');
		}
	}
	user_input[index] = '\0';
	index++;
	*arr_length = index;
	USART_voidSend('\r');
	USART_voidSend('\n');

	strcpy((char *)arr, (char *)user_input);
}
