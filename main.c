/*
 * main.c
 *
 *  Created on: Sep 13, 2023
 *      Author: Amira Magdy,Abdelrahman Mohamed
 */

#include "UTILS/STD_TYPES.h"
#include "UTILS/BIT_MATH.h"
#include "MCAL/DIO/DIO_interface.h"
#include "MCAL/USART/USART_interface.h"
#include "HAL/EEPROM/EEPROM_interface.h"
#include <util/delay.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 0X10

#define ENTER 1
#define NO_ENTER 0
#define DOT 1
#define NO_DOT 0

#define USERNAME 0
#define PASSWORD 1
#define CONFIRM_PASSWORD 2

void PrintTerminal(u8 *message,u8 enter,u8 dot);
void GettingUserInputAndCheck(u8 **data,u8 Selector,u8* arr_length);


int main()
{
	DIO_voidSetPortDirection(PORTB_REG, PORT_DIRECTION_OUTPUT);
	USART_voidInit();
	EEPROM_voidInit();

	u16 eeprom_cursor_position=0;

	if(EEPROM_voidReadData(0X03FF)==0XFF)
	{
		EEPROM_voidClear();
		eeprom_cursor_position=0;
	}

	u8 username_length=0;
	u8 *pusername_length = &username_length;

	u8  password1_length=0;
	u8 *ppassword1_length = &password1_length;

	u8 password2_length=0;
	u8 *ppassword2_length = &password2_length;

	u8 *user_name_arr=0;
	u8 **puser_name_arr = &user_name_arr;

	u8 *password_1_arr=0;
	u8 **ppassword_1_arr = &password_1_arr;

	u8 *password_2_arr=0;
	u8 **ppassword_2_arr = &password_2_arr;

	u8 message_1[] = { "For Light on and off Press 0\0" };
	PrintTerminal(message_1,ENTER,DOT);
	u8 message_2[] = { "For user name Press 1\0" };
	PrintTerminal(message_2,ENTER,DOT);


	u8 input = 0;

	u8 Led_on[] = {"Led on"};
	u8 Led_off[] = {"Led off"};

	while (1)
	{
		input = USART_voidReceive();
		switch (input)
		{
		case 'r':
			eeprom_cursor_position=0;
			EEPROM_voidClear();
			break;
		case 't':
			EEPROM_voidWriteData(0X03F0, 0XAA);
			if(EEPROM_voidReadData(0X03F0)==0XAA)
			{
				DIO_voidSetPortValue(PORTB_REG, PORT_VALUE_HIGH);
			}
			break;
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
			GettingUserInputAndCheck(puser_name_arr,USERNAME,pusername_length);
			u8 error=OK;
			u8 count=0;
			do
			{
				error=OK;
				GettingUserInputAndCheck(ppassword_1_arr,PASSWORD,ppassword1_length);
				GettingUserInputAndCheck(ppassword_2_arr,PASSWORD,ppassword2_length);
				if(password1_length!=password2_length)
				{
					error=NOK;
				}
				else
				{
					for(u8 i=0;i<password2_length;i++)
					{
						if(password_1_arr[i]!=password_2_arr[i])
						{
							error=NOK;
							break;
						}
					}
				}

				if (error)
				{
					u8 passwords_dont_match[]="Passwords don't match, Please try again\0";
					PrintTerminal(passwords_dont_match, ENTER, DOT);
					count++;
				}

			}while(error && (count<3));
			if(count > 2)
			{
				u8 enter[]="No more tries!!!\0";
				PrintTerminal(enter, ENTER, NO_DOT);
			}
			else
			{
				u8 passwords_match[]="Passwords match Thank you!\0";
				EEPROM_voidSeqWrite(eeprom_cursor_position, *puser_name_arr, username_length);
				eeprom_cursor_position+=PAGE_SIZE;
				EEPROM_voidSeqWrite(eeprom_cursor_position, *ppassword_2_arr, password2_length);
				eeprom_cursor_position+=PAGE_SIZE;
				EEPROM_voidWriteData(0X03FF, eeprom_cursor_position);
				PrintTerminal(passwords_match, ENTER, NO_DOT);
			}
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

void GettingUserInputAndCheck(u8 **data,u8 Selector,u8* arr_length)
{
	u8 index = 0;
	u8 input = 0;
	u8 user_input[] = { 0 };

	if(Selector==USERNAME)
	{
		u8 username_text[] = { "UserName: \0" };
		PrintTerminal(username_text, NO_ENTER,NO_DOT);
	}
	else if(Selector==PASSWORD)
	{
		u8 password_text[] = { "Password: \0" };
		PrintTerminal(password_text, NO_ENTER,NO_DOT);
	}
	else if(Selector==CONFIRM_PASSWORD)
	{
		u8 password_text[] = { "Confirm Password: \0" };
		PrintTerminal(password_text, NO_ENTER,NO_DOT);
	}
	else
	{
		/*do nothing*/
	}

	while (input != 0X0D)
	{
		input = USART_voidReceive();
		if (isprint(input)&&index<15)
		{
			USART_voidSend(input);
			user_input[index] = input;
			index++;
		}
		else if (input == 0X08 && index != 0)
		{
			USART_voidSend(input);
			index--;
		}
	}
	user_input[index] = '\0';
	*arr_length=index;
	USART_voidSend(0X0D);

	*data = (u8*) malloc(index*sizeof(u8));
	strcpy((char*)*data,(char*)user_input);
	u8 message[] = { "You Entered: \0" };
	PrintTerminal(message, NO_ENTER,NO_DOT);
	PrintTerminal(*data, ENTER, DOT);
}
