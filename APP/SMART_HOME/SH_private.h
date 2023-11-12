/**************SmartHome_private.h***************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#ifndef APP_SMART_HOME_SH_PRIVATE_H_
#define APP_SMART_HOME_SH_PRIVATE_H_

#define CLEAR_NEWLINE_CHAR Local_u8User_input = USART_voidReceive();\
		if (Local_u8User_input == '\n' || Local_u8User_input == '\r') Local_u8User_input = 0

#define PASSWORDS_MATCH !Local_u8NotaMatch

#define JUMP_TO_NEXT_USERNAME_ADDRESS Local_u16EEpromCurrentAdress+=0X20
#define JUMP_TO_CURR_USER_PASS_ADDRESS Local_u16EEpromCurrentAdress+=0X10

#define JUMP_TO_LAST_USER_PASS_ADDRESS Local_u16EEpromCursorPosition-=0X10
#define JUMP_TO_LAST_USER_NAME_ADDRESS Local_u16EEpromCursorPosition-=0X10

#define DECREASE_USER_COUNT_BY_ONE EEPROM_voidWriteData(ACCOUNTS_COUNT_ADDRESS, (EEPROM_voidReadData(ACCOUNTS_COUNT_ADDRESS)-1))

#define SAVE_CURRENT_USERNAME_ADDRESS Splitu16(Local_u16EEpromCurrentAdress, Local_u8EEpromCursorPositionArr); \
		EEPROM_voidSeqWrite(CURRENT_USER_NAME_ADDRESS, Local_u8EEpromCursorPositionArr, Local_u8EEpromCursorPositionLength)

#define SAVE_CURRENT_PASSWORD_ADDRESS JUMP_TO_CURR_USER_PASS_ADDRESS; \
		Splitu16(Local_u16EEpromCurrentAdress, Local_u8EEpromCursorPositionArr); \
		EEPROM_voidSeqWrite(CURRENT_USER_PASS_ADDRESS, Local_u8EEpromCursorPositionArr, Local_u8EEpromCursorPositionLength)

#define USERNAME_ARR_LENGTH_LOCATION Local_u16EEpromCurrentAdress + 15
#define DATA_LENGTH_ADDRESS_LOCATION Local_u16EEpromCurosrPosition + 15
#define PASSWORD_ARR_LENGTH_LOCATION Local_u16PasswordAddress+15

#define OLD_PASSWORD_LENGTH_ADDRESS CURRENT_USER_PASS_ADDRESS+15
#define CONFIRM_PASSWORD_LENGTH_ADDRESS Local_u16PasswordAddress+15

#define NO_MORE_TRIES Copy_u8TriesCount>=2

#define END_OF_MENU 7
#define LOGOUT 0
#define LOCK_DOOR 1
#define UNLOCK_DOOR 2
#define LED_ON_OFF 3
#define	CREATE_NEW_ACCOUNT 4
#define CHANGE_PASSWORD 5
#define DELETE_ACCOUNT 6
#define FACTORY_RESET 7




#define NO_DELAY 0
#define ONE_SEC 1000
#define TWO_SEC 2000
#define THREE_SEC 3000
#define FOUR_SEC 4000
#define FIVE_SEC 5000

#define FIRST_BOOT 1
#define MAIN_MENU 2
#define MAX_USERS_REACHED 3
#define USER_EXISTS 4
#define NOT_A_MATCH 5
#define SUCCESSFUL_LOGIN 6
#define WRONG_PASSWORD 7
#define USER_DOESNOT_EXIST 8
#define ALARM_ON 9
#define RESET 10
#define ACCOUNT_CREATED 11
#define USERNAME 12
#define PASSWORD 13
#define CONFIRM_PASSWORD 14
#define ACCOUNT_DELETED 15
#define OLD_PASSWORD 16
#define ERR_PASSWORD 17
#define PASSWORD_CHANGED 18
#define DOOR_LOCKED 19
#define DOOR_UNLOCKED 20

#endif /* APP_SMART_HOME_SH_PRIVATE_H_ */
