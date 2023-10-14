/****************EEPROM Program******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:20/09/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include <util/delay.h>
#include "../../MCAL/I2C/I2C_interface.h"
#include "EEPROM_private.h"
#include "EEPROM_config.h"
#include "EEPROM_interface.h"

void EEPROM_voidInit(void)
{
	I2C_voidMasterInit(0); /*I2C Master Initialize*/
}

void EEPROM_voidClear(void)
{
	u8 Local_CurrentAddress = EEPROM_START_ADDRESS;

	for (u8 i = 0; i < EEPROM_NUMBER_OF_PAGES; i++)
	{
		I2C_SendStartCondition();						  /*Send Start Condition*/
		I2C_SendSlaveAddressWithWrite(0XA0);			  /*0b10100000   EEPROM device address with write*/
		I2C_MasterWriteDataByteAck(Local_CurrentAddress); /*Send word address I want to access inside EEPROM with ACK*/

		for (u8 i = 0; i < EEPROM_MAX_BUFFER_SIZE; i++)
		{
			I2C_MasterWriteDataByteAck(EEPROM_DEFAULT_DATA); /*Write DEFAULT Data To Clear EEPROM Page*/
		}

		I2C_voidSendStopCondition();		 /*Send Stop Condition*/
		_delay_ms(10);						 /*Wait 10ms specified by the data sheet so the EEPROM could complete the write process*/
		Local_CurrentAddress += EEPROM_PAGE; /*Go to next to page to start Clearing again till all pages are erased*/
	}
}

void EEPROM_voidWriteData(u8 Copy_u8Address, u8 Copy_u8Data)
{
	I2C_SendStartCondition();					/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(0XA0);		/*0b10100000   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Copy_u8Address); /*Send word address I want to access inside EEPROM with ACK*/
	I2C_MasterWriteDataByteAck(Copy_u8Data);	/*Write Data to the previous specified word address in the EEPROM to write into with ACK*/
	I2C_voidSendStopCondition();				/*Send Stop Condition*/
	_delay_ms(10);								/*Wait 10ms specified by the data sheet so the EEPROM could complete the write process*/
}

u8 EEPROM_voidReadData(u8 Copy_u8Address)
{
	u8 Local_u8DataRead = 0; /*Variable to store my the data that i read*/

	/*Dummy Write just to go to a specific address to read from*/
	I2C_SendStartCondition();					/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(0XA0);		/*0b10100000   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Copy_u8Address); /*Send word address I want to access inside EEPROM to read from with ACK*/
	/*End of Dummy Write*/

	/*Start Actual Read Process from specified address*/
	I2C_SendRepeatedStart();					   /*Send Repeated Start Condition*/
	I2C_SendSlaveAddressWithRead(0XA0);			   /*0b10100000   EEPROM device address with Read " This was missing i think i didn't see it "*/
	I2C_MasterReadDataByteNack(&Local_u8DataRead); /*Read Data Byte from the previous specified address with NACK as specified by the data sheet for 1 byte Reads*/
	I2C_voidSendStopCondition();				   /*Send Stop Condition*/
	/*End Read Process*/

	return Local_u8DataRead; /*Return Data that i already Read*/
}

void EEPROM_voidSeQWrite(u8 Copy_u8Address, u8 *Copy_u8DataArr, u8 Copy_u8DataArrLength)
{
	I2C_SendStartCondition();					/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(0XA0);		/*0b10100000   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Copy_u8Address); /*Send word address I want to access inside EEPROM with ACK*/

	for (u8 i = 0; i < Copy_u8DataArrLength; i++)
	{
		I2C_MasterWriteDataByteAck(Copy_u8DataArr[i]); /*Write Data from user provided array to EEPROM memory starting from the address provided by the user*/
	}
	I2C_MasterWriteDataByteAck(Copy_u8DataArr[Copy_u8DataArrLength]); /*Writing the Last byte in my array which is usually '/0' to end my data in EEPROM memory*/

	I2C_voidSendStopCondition(); /*Send Stop Condition*/
	_delay_ms(10);				 /*Wait 10ms specified by the data sheet so the EEPROM could complete the write process*/
}

void EEPROM_voidSeqRead(u8 Copy_u8Address, u8 *Copy_u8DataArr, u8 Copy_u8DataArrLength)
{
	/*Dummy Write just to go to a specific address to read from*/
	I2C_SendStartCondition();					/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(0XA0);		/*0b10100000   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Copy_u8Address); /*Send word address I want to access inside EEPROM to read from with ACK*/
	/*End of Dummy Write*/

	/*Start Actual Read Process from specified address*/
	I2C_SendRepeatedStart();			/*Send Repeated Start Condition*/
	I2C_SendSlaveAddressWithRead(0XA0); /*0b10100000   EEPROM device address with Read " This was missing i think i didn't see it "*/

	for (u8 i = 0; i < Copy_u8DataArrLength; i++)
	{
		I2C_MasterReadDataByteAck(&Copy_u8DataArr[i]); /*Read data bytes in order from specified address and allocate them in provided array*/
	}
	I2C_MasterReadDataByteNack(&Copy_u8DataArr[Copy_u8DataArrLength]); /*just Reading the last byte which is usually '/0' to assign it at the end of my array ,Reading with  NACK to start the ending protocol for my 24c02 as specified in data sheet*/

	I2C_voidSendStopCondition(); /*Send Stop Condition*/
								 /*End Read Process*/
}