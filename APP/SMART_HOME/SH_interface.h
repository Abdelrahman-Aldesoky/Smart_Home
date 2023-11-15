/**************SmartHome_interface.h***************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#ifndef SMART_HOME_SH_INTERFACE_H_
#define SMART_HOME_SH_INTERFACE_H_

#define ZERO 0

void SmartHome_voidInit(void);
void SmartHome_voidStart(void);
void Login_Menu(void);
void Display_Login_Menu(u8 Copy_u8Page);
u8 CheckMaxUsers(void);
u8 CheckExistingUserNames(u8 Copy_u8UserNameArr[], u8 Copy_u8UserNameLength);
void CreateNewAccount(void);
void DeleteAccount(void);
void SaveNewAccount(u8 Copy_u8UserNameArr[], u8 Copy_u8UserNameArrLength, u8 Copy_u8PasswordArr[], u8 Copy_u8PasswordLength);
u8 Login(void);
s8 CheckPassword(u8 Copy_u8PasswordArr[], u8 Copy_u8PasswordLength);
u8 ChangePassword(void);
void Reset(void);
void Display(u8 Copy_u8Selector, u16 Copy_u16Time, u8 Copy_u8TriesCount);
void Alarm(void);
u16 Combine2u8(u8 arr[]);
void Splitu16(u16 number, u8 arr[]);

#endif
