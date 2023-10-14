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
#include <ctype.h>
#include <string.h>

#define ENTER 1
#define NO_ENTER 0
#define DOT 1
#define NO_DOT 0

#define USERNAME 0
#define PASSWORD 1
#define CONFIRM_PASSWORD 2

void PrintTerminal(u8 *message,u8 enter,u8 dot);
void GettingUserInputAndCheck(u8 data[],u8 Selector);


int main()
{
	DIO_voidSetPortDirection(PORTB_REG, PORT_DIRECTION_OUTPUT);
	USART_voidInit();

	u8 user_name[100] = { 0 };
	u8 password_1[100] = { 0 };
	u8 password_2[100] = { 0 };


	u8 message_1[] = { "For Light on and off Press 0" };
	PrintTerminal(message_1,ENTER,DOT);
	u8 message_2[] = { "For user name Press 1" };
	PrintTerminal(message_2,ENTER,DOT);

	u8 input = 0;

	u8 Led_on[] = {"Led on"};
	u8 Led_off[] = {"Led off"};

	while (1)
	{
		input = USART_voidReceive();
		switch (input)
		{
		case '0':
			if(DIO_u8GetPinValue(PORTB_REG, PIN0))
			{
				DIO_voidSetPinValue(PORTB_REG, PIN0, PIN_VALUE_LOW);
				/*print led off to the terminal*/
				PrintTerminal(Led_off,ENTER,DOT);
			}
			else
			{
				DIO_voidSetPinValue(PORTB_REG, PIN0, PIN_VALUE_HIGH);
				/*print led on to the terminal*/
				PrintTerminal(Led_on, ENTER,DOT);
			}
			break;
		case '1':
		{
			GettingUserInputAndCheck(user_name,USERNAME);
			u8 error=NOK;
			do
			{
				GettingUserInputAndCheck(password_1,PASSWORD);
				GettingUserInputAndCheck(password_2,CONFIRM_PASSWORD);
				error=strcmp(password_1,password_2);
				if (error)
				{
					u8 passwords_dont_match[]="Passwords don't match, Please try again";
					PrintTerminal(passwords_dont_match, ENTER, DOT);
				}

			}while(error);
			u8 passwords_match[]="Passwords match Thank you!";
			PrintTerminal(passwords_match, ENTER, NO_DOT);
			break;
		}
		default:
			break;
		}
	}
	return 0;
}

void PrintTerminal(u8 *message,u8 enter,u8 dot)
{
	for (int i = 0; message[i] != '\0'; i++)
	{
		USART_voidSend(message[i]); /*print*/
	}
	if (dot)
	{
		USART_voidSend('.');
	}
	if(enter)
	{
		USART_voidSend(0X0D);
	}
}

void GettingUserInputAndCheck(u8 data[],u8 Selector)
{
	u8 index = 0;
	u8 input = 0;
	u8 user_input[100] = { 0 };

	if(Selector==USERNAME)
	{
		u8 username_text[] = { "UserName: " };
		PrintTerminal(username_text, NO_ENTER,NO_DOT);
	}
	else if(Selector==PASSWORD)
	{
		u8 password_text[] = { "Password: " };
		PrintTerminal(password_text, NO_ENTER,NO_DOT);
	}
	else if(Selector==CONFIRM_PASSWORD)
	{
		u8 password_text[] = { "Confirm Password: " };
		PrintTerminal(password_text, NO_ENTER,NO_DOT);
	}
	else
	{
		/*do nothing*/
	}

	u8 message[] = { "You Entered: " };

	while (input != 0X0D)
	{
		input = USART_voidReceive();
		if (isprint(input))
		{
			USART_voidSend(input);
			user_input[index] = input;
			index++;
			continue;
		}
		else if (input == 0X08 && index != 0)
		{
			USART_voidSend(input);
			index--;
			continue;
		}
	}
	user_input[index] = '\0';
	USART_voidSend(0X0D);

	PrintTerminal(message, NO_ENTER,NO_DOT);

	strcpy(data,user_input);
	PrintTerminal(data, ENTER, DOT);
}
