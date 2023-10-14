/*
 * main.c
 *
 *  Created on: Sep 13, 2023
 *      Author: Amira Magdy,Abdelrahman Mohamed
 */

#include "UTILS/STD_TYPES.h"
#include "UTILS/BIT_MATH.h"
#include "MCAL/DIO/DIO_interface.h"
#include "../USART/USART_private.h"
#include "../USART/USART_interface.h"
#include <util/delay.h>


int main()
{
	DIO_voidSetPinDirection(PORTD_REG, PIN0, PIN_DIRECTION_INPUT);
	DIO_voidSetPinDirection(PORTD_REG, PIN1, PIN_DIRECTION_OUTPUT);
	DIO_voidSetPortDirection(PORTB_REG, PORT_DIRECTION_OUTPUT);

	//DIO_voidSetPortValue(PORTB_REG, PORT_VALUE_HIGH);

	USART_voidInit();
	u8 number=0;

	while(1)
	{
			_delay_ms(100);
		/***************************************************************************************/
		number=USART_voidReceive();
		switch(number)
		{
		case '0':
			DIO_voidSetPinValue(PORTB_REG, PIN0, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('0');
			break;
		case '1':
			DIO_voidSetPinValue(PORTB_REG, PIN1, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('1');
			break;
		case '2':
			DIO_voidSetPinValue(PORTB_REG, PIN2, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('2');
			break;
		case '3':
			DIO_voidSetPinValue(PORTB_REG, PIN3, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('3');
			break;
		case '4':
			DIO_voidSetPinValue(PORTB_REG, PIN4, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('4');
			break;
		case '5':
			DIO_voidSetPinValue(PORTB_REG, PIN5, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('5');
			break;
		case '6':
			DIO_voidSetPinValue(PORTB_REG, PIN6, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('6');
			break;
		case '7':
			DIO_voidSetPinValue(PORTB_REG, PIN7, PIN_VALUE_HIGH);
			_delay_ms(1000);
			USART_voidSend('7');
			break;
		default:
			DIO_voidSetPortValue(PORTB_REG, PORT_VALUE_HIGH);
			_delay_ms(1000);
			break;
		}
		DIO_voidSetPortValue(PORTB_REG, PORT_VALUE_LOW);
	}
	return 0;
}
