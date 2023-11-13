/**************SmartHome_program.c***************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/USART/USART_interface.h"
#include "../../HAL/EEPROM/EEPROM_interface.h"
#include "../../HAL/CLCD/CLCD_interface.h"
#include "../../HAL/BLUETOOTH/BL_interface.h"
#include "../LIGHTS/LIGHTS_interface.h"
#include "../DOOR/DOOR_interface.h"
#include "SH_private.h"
#include "SH_config.h"
#include "SH_interface.h"
#include <util/delay.h>
#include <string.h>
#include <ctype.h>

void SmartHome_voidInit(void)
{
	u8 Local_u8SingleUserExist = FALSE;
	u8 Local_u8CursorPositionZeroingArr[] = { 0, 0 };
	u8 Local_u8CursorPositionZeroingArrLength = sizeof(Local_u8CursorPositionZeroingArr) / sizeof(Local_u8CursorPositionZeroingArr[0]);
	u8 Local_u8AccountsCountDefaultValue = 0;

	Light_voidInit();
	CLCD_voidInit();
	USART_voidInit();
	EEPROM_voidInit();
	Door_Init();

	if (EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS) == EEPROM_DEFAULT_VALUE)
	{
		EEPROM_voidClear();
		EEPROM_voidWriteData(ACCOUNTS_COUNT_ADDRESS,
				Local_u8AccountsCountDefaultValue);

		EEPROM_voidSeqWrite(CURSOR_POS_ADDRESS,
				Local_u8CursorPositionZeroingArr,
				Local_u8CursorPositionZeroingArrLength);

		Display(FIRST_BOOT, TWO_SEC, ZERO);
	}
	Local_u8SingleUserExist = EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS);
	while(!Local_u8SingleUserExist)
	{
		CreateNewAccount();
		Local_u8SingleUserExist = EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS);
	}

}

void SmartHome_voidStart(void)
{
	Door_Lock();
	Display(MAIN_MENU, NO_DELAY, ZERO);

	u8 Local_u8User_input = 0;
	Local_u8User_input = USART_voidReceive();
	Local_u8User_input=tolower(Local_u8User_input);

	if(Local_u8User_input=='l')
	{
		CLEAR_NEWLINE_CHAR;
		if(Login())
		{
			/*Unlock door Function*/
			Door_Unlock();
			Login_Menu();
		}
		else
		{
			Alarm();
		}
	}
	else
	{
		/*do nothing*/
	}
}

void Login_Menu(void)
{
	u8 Local_u8PageNo = 0;
	u8 Local_u8User_input = 0;
	while(Local_u8User_input != 'e')
	{
		Display_Login_Menu(Local_u8PageNo);
		Local_u8User_input = USART_voidReceive();
		Local_u8User_input = tolower(Local_u8User_input);

		switch (Local_u8User_input)
		{
		case 'l':
			CLEAR_NEWLINE_CHAR;
			Door_Lock();
			/*Lock door function*/
			Display(DOOR_LOCKED,ONE_SEC,ZERO);
			break;
		case 'u':
			CLEAR_NEWLINE_CHAR;
			Door_Unlock();
			/*unlock door function*/
			Display(DOOR_UNLOCKED,ONE_SEC,ZERO);
			break;
		case 'o':
			CLEAR_NEWLINE_CHAR;
			/*turn led on and off function*/
			LightToggle();
			break;
		case 'c':
			CLEAR_NEWLINE_CHAR;
			CreateNewAccount();
			break;
		case 'p':
			CLEAR_NEWLINE_CHAR;
			if(!ChangePassword()) return;
			break;
		case 'd':
		{
			CLEAR_NEWLINE_CHAR;
			DeleteAccount();
			u8 Local_u8SingleUserExist = EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS);
			if(!Local_u8SingleUserExist) Reset();
			return;
		}
		case 'r':
			CLEAR_NEWLINE_CHAR;
			Reset();
			return;
		case 'n':
			CLEAR_NEWLINE_CHAR;
			/*Next Page in login menu*/
			Local_u8PageNo++;
			if(Local_u8PageNo>END_OF_MENU) Local_u8PageNo = ZERO;
			break;
		case 'e':
			CLEAR_NEWLINE_CHAR;
			return;
		default:
			/*do nothing*/
			break;
		}
	}

}

void Display_Login_Menu(u8 Copy_u8PageNo)
{
	CLCD_voidSendCommand(DISPLAY_CLEAR);
	CLCD_voidSendString("\"N\" for NextPage");
	CLCD_voidSetPostion(SECOND_LINE, 0);
	switch (Copy_u8PageNo)
	{
	case LOGOUT:
		CLCD_voidSendString(" \"E\" To Logout. ");
		break;
	case LOCK_DOOR:
		CLCD_voidSendString(" \"L\" LOCK Door. ");
		break;
	case UNLOCK_DOOR:
		CLCD_voidSendString("\"U\" UNLOCK Door.");
		break;
	case LED_ON_OFF:
		CLCD_voidSendString(" \"O\" LED ON/OFF ");
		break;
	case CREATE_NEW_ACCOUNT:
		CLCD_voidSendString("\"C\" NEW Account.");
		break;
	case CHANGE_PASSWORD:
		CLCD_voidSendString("\"P\" Change Pass.");
		break;
	case DELETE_ACCOUNT:
		CLCD_voidSendString("\"D\" DEL Account.");
		break;
	case FACTORY_RESET:
		CLCD_voidSendString("\"R\" FactoryReset");
		break;
	default:
		/*do nothing*/
		break;
	}
}

u8 CheckMaxUsers(void)
{
	u8 Accounts_Count = EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS);
	if (Accounts_Count >= MAX_USERS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

u8 CheckExistingUserNames(u8 Copy_u8UserNameArr[], u8 Copy_u8UserNameLength)
{
	u16 Local_u16EEpromCurrentAdress = EEPROM_START_ADDRESS;
	u8 Local_u8UserNameExists = FALSE;
	u8 Local_u8AccountsCount = EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS);
	u8 Local_u8EEpromCurrentUserNameLength = 0;
	u8 Local_u8EEpromCurrentUserNAME[ARR_LENGTH] = { 0 };
	u8 Local_u8EEpromCursorPositionArr[] = { 0, 0 };
	u8 Local_u8EEpromCursorPositionLength = sizeof(Local_u8EEpromCursorPositionArr) / sizeof(Local_u8EEpromCursorPositionArr[0]);

	for (u8 Local_Iterator = 0; Local_Iterator < Local_u8AccountsCount; Local_Iterator++)
	{
		Local_u8UserNameExists = FALSE;
		Local_u8EEpromCurrentUserNameLength = EEPROM_voidReadData(USERNAME_ARR_LENGTH_LOCATION);
		EEPROM_voidSeqRead(Local_u16EEpromCurrentAdress, Local_u8EEpromCurrentUserNAME, Local_u8EEpromCurrentUserNameLength);

		if (Copy_u8UserNameLength == Local_u8EEpromCurrentUserNameLength)
		{
			Local_u8UserNameExists = !strcmp((char*) Copy_u8UserNameArr, (char*) Local_u8EEpromCurrentUserNAME);
			if (Local_u8UserNameExists)
			{
				Local_u8UserNameExists = TRUE;
				SAVE_CURRENT_USERNAME_ADDRESS;
				SAVE_CURRENT_PASSWORD_ADDRESS;
				return Local_u8UserNameExists;
			}
		}
		JUMP_TO_NEXT_USERNAME_ADDRESS;
	}
	return Local_u8UserNameExists;
}

void CreateNewAccount(void)
{
	u8 Local_u8UserNameArr[ARR_LENGTH] = { 0 };
	u8 Local_u8UserNameArrLength = 0;
	u8 Local_u8PasswordArr[ARR_LENGTH] = { 0 };
	u8 Local_u8PasswordLength = 0;
	u8 Local_u8ConfirmPasswordArr[ARR_LENGTH] = { 0 };
	u8 Local_u8ConfirmPasswordLength = 0;

	u8 Local_u8MaxUsersReached = FALSE;
	u8 Local_u8UserNameExists = FALSE;
	u8 Local_u8NotaMatch = TRUE;
	u8 Local_u8TriesCount = 0;

	Display(USERNAME, NO_DELAY, ZERO);
	BL_voidReceive(_USERNAME, Local_u8UserNameArr, &Local_u8UserNameArrLength);

	Local_u8MaxUsersReached = CheckMaxUsers();
	if (Local_u8MaxUsersReached)
	{
		Display(MAX_USERS_REACHED, ONE_SEC, ZERO);
		return;
	}

	Local_u8UserNameExists = CheckExistingUserNames(Local_u8UserNameArr,Local_u8UserNameArrLength);
	if (Local_u8UserNameExists)
	{
		Display(USER_EXISTS, TWO_SEC, ZERO);
		return;
	}

	do
	{
		Display(PASSWORD, NO_DELAY, ZERO);
		BL_voidReceive(PASSWORD, Local_u8PasswordArr, &Local_u8PasswordLength);

		Display(CONFIRM_PASSWORD, NO_DELAY, ZERO);
		BL_voidReceive(CONFIRM_PASSWORD, Local_u8ConfirmPasswordArr, &Local_u8ConfirmPasswordLength);

		if (Local_u8PasswordLength == Local_u8ConfirmPasswordLength)
		{
			Local_u8NotaMatch = strcmp((char*) Local_u8PasswordArr, (char*) Local_u8ConfirmPasswordArr);
			if (PASSWORDS_MATCH)
			{
				SaveNewAccount(Local_u8UserNameArr, Local_u8UserNameArrLength, Local_u8PasswordArr, Local_u8PasswordLength);
				return;
			}
		}

		Display(NOT_A_MATCH, TWO_SEC, Local_u8TriesCount);
		Local_u8TriesCount++;

	} while (Local_u8NotaMatch && (Local_u8TriesCount < NUMBER_OF_TRIES));
}

void SaveNewAccount(u8 Copy_u8UserNameArr[], u8 Copy_u8UserNameArrLength, u8 Copy_u8PasswordArr[], u8 Copy_u8PasswordLength)
{
	u16 Local_u16EEpromCurosrPosition = 0;
	u8 Local_u8EEpromCursorPositionArr[] = { 0, 0 };
	u8 Local_u8EEpromCursorPositionLength = sizeof(Local_u8EEpromCursorPositionArr) / sizeof(Local_u8EEpromCursorPositionArr[0]);

	u8 Local_u8AccountsNumber = EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS);
	Local_u8AccountsNumber++;
	EEPROM_voidWriteData(ACCOUNTS_COUNT_ADDRESS, Local_u8AccountsNumber);

	EEPROM_voidSeqRead(CURSOR_POS_ADDRESS, Local_u8EEpromCursorPositionArr, Local_u8EEpromCursorPositionLength);

	Local_u16EEpromCurosrPosition = Combine2u8(Local_u8EEpromCursorPositionArr);

	EEPROM_voidSeqWrite(Local_u16EEpromCurosrPosition, Copy_u8UserNameArr, Copy_u8UserNameArrLength);
	EEPROM_voidWriteData(DATA_LENGTH_ADDRESS_LOCATION, Copy_u8UserNameArrLength);
	Local_u16EEpromCurosrPosition += PAGE_SIZE;

	EEPROM_voidSeqWrite(Local_u16EEpromCurosrPosition, Copy_u8PasswordArr, Copy_u8PasswordLength);
	EEPROM_voidWriteData(DATA_LENGTH_ADDRESS_LOCATION, Copy_u8PasswordLength);
	Local_u16EEpromCurosrPosition += PAGE_SIZE;

	Splitu16(Local_u16EEpromCurosrPosition, Local_u8EEpromCursorPositionArr);

	EEPROM_voidSeqWrite(CURSOR_POS_ADDRESS, Local_u8EEpromCursorPositionArr, Local_u8EEpromCursorPositionLength);

	Display(ACCOUNT_CREATED, TWO_SEC, ZERO);
}

u8 Login(void)
{
	u8 Local_u8UserNameArr[ARR_LENGTH] = { 0 };
	u8 Local_u8UserNameArrLength = 0;
	u8 Local_u8PasswordArr[ARR_LENGTH] = { 0 };
	u8 Local_u8PasswordLength = 0;

	u8 Local_u8SuccessfulLogin = FALSE;
	u8 Local_u8UserNameExists = FALSE;

	for (u8 Local_Iterator = 0; Local_Iterator < NUMBER_OF_TRIES; Local_Iterator++)
	{
		Display(USERNAME, NO_DELAY, ZERO);
		BL_voidReceive(_USERNAME, Local_u8UserNameArr, &Local_u8UserNameArrLength);
		Local_u8UserNameExists = CheckExistingUserNames(Local_u8UserNameArr, Local_u8UserNameArrLength);
		Display(PASSWORD, NO_DELAY, ZERO);
		BL_voidReceive(PASSWORD, Local_u8PasswordArr, &Local_u8PasswordLength);
		Local_u8SuccessfulLogin = CheckPassword(Local_u8PasswordArr, Local_u8PasswordLength);

		if (Local_u8UserNameExists)
		{
			if(Local_u8SuccessfulLogin)
			{
				Display(SUCCESSFUL_LOGIN, TWO_SEC, ZERO);
				return Local_u8SuccessfulLogin;
			}
			else
			{
				Local_u8SuccessfulLogin = FALSE;
				Display(WRONG_PASSWORD, THREE_SEC, Local_Iterator);
			}
		}
		else
		{
			Local_u8SuccessfulLogin = FALSE;
			Display(USER_DOESNOT_EXIST, THREE_SEC, Local_Iterator);
		}
	}
	return Local_u8SuccessfulLogin;
}

void DeleteAccount(void)
{
	u8 Local_u8MoveArr[FULL_DATA_ARR_LENGTH] = { 0 };
	u8 Local_u8DeleteArr[FULL_DATA_ARR_LENGTH] = { 0 };

	u8 Local_u8MoveArrAddress[] = { 0 , 0 };
	u8 Local_u8MoveArrAddressLength = sizeof(Local_u8MoveArrAddress)/sizeof(Local_u8MoveArrAddress[0]);
	u16 Local_u16MoveAddress = 0;

	u8 Local_u8EEpromCursorPositionArr[] = { 0 , 0 };
	u8 Local_u8EEpromCursorPositionLength = sizeof(Local_u8EEpromCursorPositionArr) / sizeof(Local_u8EEpromCursorPositionArr[0]);
	u16 Local_u16EEpromCursorPosition = 0;


	EEPROM_voidSeqRead(DEFAULT_DELETE_ADDRESS, Local_u8DeleteArr, PAGE_SIZE);


	EEPROM_voidSeqRead(CURRENT_USER_PASS_ADDRESS, Local_u8MoveArrAddress, Local_u8MoveArrAddressLength);
	Local_u16MoveAddress = Combine2u8(Local_u8MoveArrAddress);

	EEPROM_voidSeqRead(CURSOR_POS_ADDRESS, Local_u8EEpromCursorPositionArr, Local_u8EEpromCursorPositionLength);
	Local_u16EEpromCursorPosition = Combine2u8(Local_u8EEpromCursorPositionArr);

	JUMP_TO_LAST_USER_PASS_ADDRESS;
	EEPROM_voidSeqRead(Local_u16EEpromCursorPosition, Local_u8MoveArr, PAGE_SIZE);
	EEPROM_voidSeqWrite(Local_u16MoveAddress, Local_u8MoveArr, PAGE_SIZE);
	EEPROM_voidSeqWrite(Local_u16EEpromCursorPosition, Local_u8DeleteArr, PAGE_SIZE);

	/**************************************************************************************************************************************/

	EEPROM_voidSeqRead(CURRENT_USER_NAME_ADDRESS, Local_u8MoveArrAddress, Local_u8MoveArrAddressLength);
	Local_u16MoveAddress = Combine2u8(Local_u8MoveArrAddress);

	JUMP_TO_LAST_USER_NAME_ADDRESS;
	EEPROM_voidSeqRead(Local_u16EEpromCursorPosition, Local_u8MoveArr, PAGE_SIZE);
	EEPROM_voidSeqWrite(Local_u16MoveAddress, Local_u8MoveArr, PAGE_SIZE);
	EEPROM_voidSeqWrite(Local_u16EEpromCursorPosition, Local_u8DeleteArr, PAGE_SIZE);


	Splitu16(Local_u16EEpromCursorPosition, Local_u8EEpromCursorPositionArr);
	EEPROM_voidSeqWrite(CURSOR_POS_ADDRESS, Local_u8EEpromCursorPositionArr, Local_u8EEpromCursorPositionLength);

	DECREASE_USER_COUNT_BY_ONE;

	Display(ACCOUNT_DELETED, TWO_SEC, ZERO);
}

s8 CheckPassword(u8 Copy_u8PasswordArr[], u8 Copy_u8PasswordLength)
{
	s8 Local_s8CorrectPassword = FALSE;
	u16 Local_u16PasswordAddress = 0;
	u8 Local_u8PasswordAddressArr[] = { 0, 0 };
	u8 Local_u8PasswordAddressLength = sizeof(Local_u8PasswordAddressArr) / sizeof(Local_u8PasswordAddressArr[0]);
	u8 Local_u8CurrentSavedUserPasswordArr[ARR_LENGTH] = { 0 };
	u8 Local_u8CurrentSavedUserPasswordLength = 0;

	EEPROM_voidSeqRead(CURRENT_USER_PASS_ADDRESS, Local_u8PasswordAddressArr, Local_u8PasswordAddressLength);
	Local_u16PasswordAddress = Combine2u8(Local_u8PasswordAddressArr);
	Local_u8CurrentSavedUserPasswordLength = EEPROM_voidReadData(PASSWORD_ARR_LENGTH_LOCATION);

	if (Local_u8CurrentSavedUserPasswordLength == Copy_u8PasswordLength)
	{
		EEPROM_voidSeqRead(Local_u16PasswordAddress, Local_u8CurrentSavedUserPasswordArr, Local_u8CurrentSavedUserPasswordLength);
		Local_s8CorrectPassword = !strcmp((char*) Local_u8CurrentSavedUserPasswordArr, (char*) Copy_u8PasswordArr);
		if (Local_s8CorrectPassword) return Local_s8CorrectPassword;
	}
	return Local_s8CorrectPassword;
}

u8 ChangePassword(void)
{
	u8 Local_PasswordChanged = FALSE;
	s8 Local_s8CorrectOldPassword = FALSE;
	u8 Local_u8NotaMatch = TRUE;

	u8 Local_u8PasswordArr[ARR_LENGTH] = { 0 };
	u8 Local_u8PasswordLength = 0;
	u8 Local_u8ConfirmPasswordArr[ARR_LENGTH] = { 0 };
	u8 Local_u8ConfirmPasswordLength = 0;
	u8 Local_u8TriesCount = 0;

	u8 Local_u8PasswordAddressArr[] = { 0, 0 };
	u8 Local_u8PasswordAddressLength = sizeof(Local_u8PasswordAddressArr) / sizeof(Local_u8PasswordAddressArr[0]);
	u16 Local_u16PasswordAddress=0;

	EEPROM_voidSeqRead(CURRENT_USER_PASS_ADDRESS, Local_u8PasswordAddressArr, Local_u8PasswordAddressLength);
	Local_u16PasswordAddress = Combine2u8(Local_u8PasswordAddressArr);

	u8 Local_u8DeleteArr[FULL_DATA_ARR_LENGTH] = { 0 };
	EEPROM_voidSeqRead(DEFAULT_DELETE_ADDRESS, Local_u8DeleteArr, PAGE_SIZE);

	for (u8 Local_Iterator = 0; Local_Iterator < NUMBER_OF_TRIES; Local_Iterator++)
	{
		Display(OLD_PASSWORD, NO_DELAY, ZERO);
		BL_voidReceive(PASSWORD, Local_u8PasswordArr, &Local_u8PasswordLength);
		Local_s8CorrectOldPassword = CheckPassword(Local_u8PasswordArr, Local_u8PasswordLength);
		if (Local_s8CorrectOldPassword)
		{
			do
			{
				Display(PASSWORD, NO_DELAY, ZERO);
				BL_voidReceive(PASSWORD, Local_u8PasswordArr, &Local_u8PasswordLength);

				Display(CONFIRM_PASSWORD, NO_DELAY, ZERO);
				BL_voidReceive(CONFIRM_PASSWORD, Local_u8ConfirmPasswordArr, &Local_u8ConfirmPasswordLength);

				if (Local_u8PasswordLength == Local_u8ConfirmPasswordLength)
				{
					Local_u8NotaMatch = strcmp((char*) Local_u8PasswordArr, (char*) Local_u8ConfirmPasswordArr);
					if (PASSWORDS_MATCH)
					{
						EEPROM_voidSeqWrite(Local_u16PasswordAddress, Local_u8DeleteArr, PAGE_SIZE);
						EEPROM_voidSeqWrite(Local_u16PasswordAddress, Local_u8ConfirmPasswordArr, Local_u8ConfirmPasswordLength);
						EEPROM_voidWriteData(CONFIRM_PASSWORD_LENGTH_ADDRESS, Local_u8ConfirmPasswordLength);
						Display(PASSWORD_CHANGED, TWO_SEC, ZERO);
						Local_PasswordChanged = TRUE;
						return Local_PasswordChanged;
					}
				}

				Display(NOT_A_MATCH, TWO_SEC, Local_u8TriesCount);
				Local_u8TriesCount++;
			} while (Local_u8NotaMatch && (Local_u8TriesCount < NUMBER_OF_TRIES));
			break;
		}
		else
		{
			Display(WRONG_PASSWORD, TWO_SEC, Local_Iterator);
		}
	}

	Display(ERR_PASSWORD, TWO_SEC, ZERO);
	Local_PasswordChanged = FALSE;
	return Local_PasswordChanged;
}

void Display(u8 Copy_u8Selector, u16 Copy_u16Time, u8 Copy_u8TriesCount)
{
	CLCD_voidSendCommand(DISPLAY_CLEAR);
	switch (Copy_u8Selector)
	{
	case FIRST_BOOT:
		CLCD_voidSendString("First Boot.");
		break;
	case MAIN_MENU:
		CLCD_voidSendString("   Smart Home   ");
		CLCD_voidSetPostion(SECOND_LINE, 0);
		CLCD_voidSendString("   LOGIN. \"L\"   ");
		break;
	case USERNAME:
		CLCD_voidSendString("Enter User name:");
		break;
	case MAX_USERS_REACHED:
		CLCD_voidSendNumber(MAX_USERS);
		CLCD_voidSendString(" users max!!");
		break;
	case USER_EXISTS:
		CLCD_voidSendString("User Exists!!");
		CLCD_voidSetPostion(SECOND_LINE, 0);
		CLCD_voidSendString("Try logging in.");
		break;
	case OLD_PASSWORD:
		CLCD_voidSendString("Old Password:");
		break;
	case PASSWORD:
		CLCD_voidSendString("Enter Password:");
		break;
	case ERR_PASSWORD:
		CLCD_voidSendString("Failed To Change");
		CLCD_voidSetPostion(SECOND_LINE, 0);
		CLCD_voidSendString(" Old Password!! ");
		break;
	case PASSWORD_CHANGED:
		CLCD_voidSendString("Password Changed");
		CLCD_voidSetPostion(SECOND_LINE, 0);
		CLCD_voidSendString(" SUCCESSFULLY!! ");
		break;
	case CONFIRM_PASSWORD:
		CLCD_voidSendString("Confirm Password");
		break;
	case NOT_A_MATCH:
		CLCD_voidSendString("  Not a Match.  ");
		CLCD_voidSetPostion(SECOND_LINE, 0);
		if(NO_MORE_TRIES)
		{
			CLCD_voidSendString(" NO MORE TRIES! ");
		}
		else
		{
			CLCD_voidSendString("Try Again TL= ");
			CLCD_voidSetPostion(SECOND_LINE, 14);
			CLCD_voidSendNumber(NUMBER_OF_TRIES_DISPLAY - Copy_u8TriesCount);
			CLCD_voidSendString(".");
		}
		break;
	case SUCCESSFUL_LOGIN:
		CLCD_voidSendString("SUCCESSFUL LOGIN");
		break;
	case WRONG_PASSWORD:
		CLCD_voidSendString("WRONG PASSWORD!!");
		CLCD_voidSetPostion(SECOND_LINE, 0);
		if(NO_MORE_TRIES)
		{
			CLCD_voidSendString(" NO MORE TRIES! ");
		}
		else
		{
			CLCD_voidSendString("Try Again TL= ");
			CLCD_voidSetPostion(SECOND_LINE, 14);
			CLCD_voidSendNumber(NUMBER_OF_TRIES_DISPLAY - Copy_u8TriesCount);
			CLCD_voidSendString(".");
		}
		break;
	case USER_DOESNOT_EXIST:
		CLCD_voidSendString(" USER DOES NOT! ");
		CLCD_voidSetPostion(SECOND_LINE, 0);
		if(NO_MORE_TRIES)
		{
			CLCD_voidSetPostion(FIRST_LINE, 0);
			CLCD_voidSendString(" NO MORE TRIES! ");
		}
		else
		{
			CLCD_voidSendString("EXIST!!   TL= ");
			CLCD_voidSetPostion(SECOND_LINE, 14);
			CLCD_voidSendNumber(NUMBER_OF_TRIES_DISPLAY - Copy_u8TriesCount);
			CLCD_voidSendString(".");
		}
		break;
	case ALARM_ON:
		CLCD_voidSendString("ALARM ON");
		break;
	case RESET:
		CLCD_voidSendString("Reset Complete.");
		break;
	case ACCOUNT_CREATED:
		CLCD_voidSendString("Account Created.");
		break;
	case ACCOUNT_DELETED:
		CLCD_voidSendString("Account Deleted!");
		break;
	case DOOR_LOCKED:
		CLCD_voidSendString("  DOOR LOCKED!  ");
		break;
	case DOOR_UNLOCKED:
		CLCD_voidSendString(" DOOR UNLOCKED! ");
		break;
	default:
		CLCD_voidSendString("ERROR.");
		break;
	}
	_delay_ms(Copy_u16Time);
}

void Reset(void)
{
	EEPROM_voidClear();
	Display(RESET, ONE_SEC, ZERO);
	SmartHome_voidInit();
}

void Alarm(void)
{
	/*Set PortD Pin7 to output for buzzer*/
	DIO_voidSetPinDirection(PORTD_REG, PIN7, PIN_DIRECTION_OUTPUT);

	DIO_voidSetPinValue(PORTD_REG, PIN7, PIN_VALUE_HIGH);
	Display(ALARM_ON, FIVE_SEC, ZERO);
	DIO_voidSetPinValue(PORTD_REG, PIN7, PIN_VALUE_LOW);
}

u16 Combine2u8(u8 arr[])
{
	u16 Local_temp_number = 0;
	Local_temp_number = arr[0] * 0X0100;
	Local_temp_number += arr[1];
	return Local_temp_number;
}

void Splitu16(u16 number, u8 arr[])
{
	arr[0] = (number >> 8) & 0XFF;
	arr[1] = (u8) number & 0XFF;
}
