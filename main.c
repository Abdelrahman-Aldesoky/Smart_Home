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
#include "HAL/CLCD/CLCD_interface.h"
#include <util/delay.h>
#include <string.h>
#include <ctype.h>

#define PAGE_SIZE 0X10
#define EEPROM_DEFAULT_VALUE 0XFF

#define COMBINE_CURSOR_POS Combine2u8(eeprom_cursor_arr)
#define SPLIT_CURSOR_POS Splitu16(eeprom_cursor_position, eeprom_cursor_arr)
#define CURSON_POS_ADDRESS 0X03FE
#define EEPROM_CURSOR_ARR_LEN 2


#define ENTER 1
#define NO_ENTER 0
#define DOT 1
#define NO_DOT 0

#define ACCOUNTS_COUNT_ADDRESS 0X03F0
#define USERNAME 0
#define PASSWORD 1
#define CONFIRM_PASSWORD 2

#define ARR_LENGTH 16


void GettingUserInputAndCheck(u8 data[], u8 Selector, u8* arr_length);
s8 CheckRepeatedUsername( u8 data[],u8 arr_length);
u16 Combine2u8(u8 arr[]);
void Splitu16(u16 number,u8 arr[]);

int main()
{
	DIO_voidSetPortDirection(PORTB_REG, PORT_DIRECTION_OUTPUT);

	CLCD_voidInit();
	USART_voidInit();
	EEPROM_voidInit();

	u16 eeprom_cursor_position = 0;
	u8 eeprom_cursor_arr[3]={0};
	SPLIT_CURSOR_POS;

	if (EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS) == EEPROM_DEFAULT_VALUE)
	{
		EEPROM_voidClear();
		EEPROM_voidWriteData(ACCOUNTS_COUNT_ADDRESS, 0);
		EEPROM_voidSeqWrite(CURSON_POS_ADDRESS, eeprom_cursor_arr, EEPROM_CURSOR_ARR_LEN);
	}

	u8 input = 0;

	u8 username_length = 0;
	u8 user_name_arr[ARR_LENGTH] = { 0 };
	u8 password1_length = 0;
	u8 password_1_arr[ARR_LENGTH] = { 0 };
	u8 password2_length = 0;
	u8 password_2_arr[ARR_LENGTH] = { 0 };


	while (1)
	{
		CLCD_voidSendCommand(DISPLAY_CLEAR);
		CLCD_voidSendString("Light on/off \"o\"");
		for (u8 i = 0; i < 12; i++)
		{
			if (i > 5)
			{
				CLCD_voidSendCommand(SHIFT_RIGHT);
			}
			else
			{
				CLCD_voidSendCommand(SHIFT_LEFT);
			}
		}
		CLCD_voidSetPostion(SECOND_LINE, 0);
		CLCD_voidSendString("New Account \"n\"");

		input = USART_voidReceive();
		input = tolower(input);
		switch (input)
		{
		case 'r':
			eeprom_cursor_position = 0;
			EEPROM_voidClear();
			break;
		case 'o':
			if (DIO_u8GetPinValue(PORTB_REG, PIN0))
			{
				DIO_voidSetPinValue(PORTB_REG, PIN0, PIN_VALUE_LOW);
			}
			else
			{
				DIO_voidSetPinValue(PORTB_REG, PIN0, PIN_VALUE_HIGH);
			}
			break;
		case 'n':
		{
			s8 Repeated = FALSE;
			GettingUserInputAndCheck(user_name_arr, USERNAME, &username_length);
			Repeated = CheckRepeatedUsername(user_name_arr,username_length);

			if (Repeated)
			{
				CLCD_voidSendCommand(DISPLAY_CLEAR);
				CLCD_voidSendString("User Exists");
				CLCD_voidSetPostion(SECOND_LINE, 0);
				CLCD_voidSendString("Try again.");
				_delay_ms(1000);
				break;
			}

			s8 error = OK;
			u8 count = 0;
			do
			{
				error = OK;
				GettingUserInputAndCheck(password_1_arr, PASSWORD,&password1_length);
				GettingUserInputAndCheck(password_2_arr, CONFIRM_PASSWORD,&password2_length);

				if (password1_length != password2_length)
				{
					error = NOK;
				}
				else
				{
					error = strcmp((char*) password_1_arr,(char*) password_2_arr);
				}

				if (error)
				{
					CLCD_voidSendCommand(DISPLAY_CLEAR);
					CLCD_voidSendString("Not a Match.");
					CLCD_voidSetPostion(SECOND_LINE, 0);
					CLCD_voidSendString("Try Again TL= ");
					CLCD_voidSetPostion(SECOND_LINE, 14);
					CLCD_voidSendNumber(2 - count);
					CLCD_voidSendString(".");
					_delay_ms(1000);
					count++;
				}

			} while (error && (count < 3));
			if (count > 2)
			{
				CLCD_voidSendCommand(DISPLAY_CLEAR);
				CLCD_voidSendString("No more tries");
				CLCD_voidSetPostion(SECOND_LINE, 0);
				CLCD_voidSendString("Bye");
				_delay_ms(1000);
			}
			else
			{
				EEPROM_voidWriteData(ACCOUNTS_COUNT_ADDRESS, (EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS)+1));

				EEPROM_voidSeqRead(CURSON_POS_ADDRESS, eeprom_cursor_arr, EEPROM_CURSOR_ARR_LEN);
				eeprom_cursor_position = COMBINE_CURSOR_POS;

				EEPROM_voidSeqWrite(eeprom_cursor_position, user_name_arr,username_length);
				EEPROM_voidWriteData(eeprom_cursor_position + (PAGE_SIZE - 1),username_length);
				eeprom_cursor_position += PAGE_SIZE;
				EEPROM_voidSeqWrite(eeprom_cursor_position, password_2_arr,password2_length);
				EEPROM_voidWriteData((eeprom_cursor_position + (PAGE_SIZE - 1)),password2_length);
				eeprom_cursor_position += PAGE_SIZE;

				SPLIT_CURSOR_POS;
				EEPROM_voidSeqWrite(CURSON_POS_ADDRESS, eeprom_cursor_arr, EEPROM_CURSOR_ARR_LEN);

				CLCD_voidSendCommand(DISPLAY_CLEAR);
				CLCD_voidSendString("Account Created");
				_delay_ms(1000);
			}
			break;
		}
		default:
			break;
		}
	}
	return 0;
}

void GettingUserInputAndCheck(u8 data[], u8 Selector, u8* arr_length)
{
	u8 index = 0;
	u8 input = 0;
	u8 user_input[PAGE_SIZE];

	if (Selector == USERNAME)
	{
		CLCD_voidSendCommand(DISPLAY_CLEAR);
		CLCD_voidSendString("Enter User name");
	}
	else if (Selector == PASSWORD)
	{
		CLCD_voidSendCommand(DISPLAY_CLEAR);
		CLCD_voidSendString("Enter Password");
	}
	else if (Selector == CONFIRM_PASSWORD)
	{
		CLCD_voidSendCommand(DISPLAY_CLEAR);
		CLCD_voidSendString("Confirm Password");
	}

	while (input != 0X0D)
	{
		input = USART_voidReceive();
		if (isprint(input) && index < 14)
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
	*arr_length = index;
	USART_voidSend(0X0D);

	strcpy((char*) data, (char*) user_input);
}

s8 CheckRepeatedUsername(u8 data[],u8 arr_length)
{
	s8 Repeated = FALSE;
	u8 Accounts_Count=0;
	u16 Address=0X0000;
	u8 username_data[ARR_LENGTH]={0XFF};
	username_data[ARR_LENGTH-2]='\0';

	for(u8 i=arr_length;i<(ARR_LENGTH-2);i++)
	{
		data[i]=0XFF;
	}
	data[ARR_LENGTH-2]='\0';

	if(EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS))
	{
		Accounts_Count=EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS);
		for(u8 i=0;i<Accounts_Count;i++)
		{
			EEPROM_voidSeqRead(Address, username_data, ARR_LENGTH-2);
			username_data[ARR_LENGTH-2]='\0';
			CLCD_voidSendCommand(DISPLAY_CLEAR);
			CLCD_voidSendString(username_data);
			_delay_ms(200);
			Repeated=!(strcmp((char*)data,(char*)username_data));
			if(Repeated)
			{
				Repeated=TRUE;
				break;
			}
			else
			{
				Address+=0X20;
				Repeated = FALSE;
			}
		}
	}
	else
	{
		Repeated = FALSE;
	}
	return Repeated;
}

u16 Combine2u8(u8 arr[])
{
	u16 Local_temp_number=0;
	Local_temp_number=arr[0]*0X0100;
	Local_temp_number+=arr[1];
	return Local_temp_number;
}

void Splitu16(u16 number,u8 arr[])
{
	arr[0]= (number>>8) & 0XFF;
	arr[1]= (u8) number & 0XFF;
}
