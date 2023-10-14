/***************EEPROM Interface*****************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:20/09/2023.****************/
/************************************************/

#ifndef _EEPROM_INTERFACE_H_
#define _EEPROM_INTERFACE_H_

void EEPROM_voidInit(void);
void EEPROM_voidClear(void);

void EEPROM_voidWriteData(u8 Copy_u8Address, u8 Copy_u8Data);
u8 EEPROM_voidReadData(u8 Copy_u8Address);

void EEPROM_voidSeQWrite(u8 Copy_u8Address, u8 *Copy_u8DataArr, u8 Copy_u8DataArrLength);
void EEPROM_voidSeqRead(u8 Copy_u8Address, u8 *Copy_u8DataArr, u8 Copy_u8DataArrLength);

#endif
