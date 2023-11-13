/**************SmartHome_config.h***************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#ifndef APP_SMART_HOME_SH_CONFIG_H_
#define APP_SMART_HOME_SH_CONFIG_H_

#define EEPROM_DEFAULT_VALUE 0XFF

#define EEPROM_START_ADDRESS 0X0000
#define ACCOUNTS_COUNT_ADDRESS 0X03F0
#define CURSOR_POS_ADDRESS 0X03FE
#define CURRENT_USER_NAME_ADDRESS 0X03E0
#define CURRENT_USER_PASS_ADDRESS 0X03EE
#define DEFAULT_DELETE_ADDRESS 0X03D0

#define MAX_USERS 20
#define NUMBER_OF_TRIES 3
#define NUMBER_OF_TRIES_DISPLAY 2

#endif /* APP_SMART_HOME_SH_CONFIG_H_ */
