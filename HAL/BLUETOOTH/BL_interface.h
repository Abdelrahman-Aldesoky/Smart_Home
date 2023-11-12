/****************BL_interface.h******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#ifndef HAL_BLUETOOTH_BL_INTERFACE_H_
#define HAL_BLUETOOTH_BL_INTERFACE_H_

#define PAGE_SIZE 0X10
#define FULL_DATA_ARR_LENGTH 17
#define ARR_LENGTH 15
#define MAX_LENGTH 14

#define BACKSPACE 0X08

#define _USERNAME 1

void BL_voidReceive(u8 Selector, u8 arr[], u8* arr_length);

#endif /* HAL_BLUETOOTH_BL_INTERFACE_H_ */
