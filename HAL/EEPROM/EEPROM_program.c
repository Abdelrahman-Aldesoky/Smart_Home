/****************EEPROM Program******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:20/09/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "../../MCAL/I2C/I2C_interface.h"
#include "EEPROM_private.h"
#include "EEPROM_config.h"
#include "EEPROM_interface.h"
#include <util/delay.h>

void EEPROM_voidInit(void)
{
	I2C_voidMasterInit(0); 										 	 /*I2C Master Initialize*/
}

void EEPROM_voidClear(void)
{
	u8 Local_temp = EEPROM_BLOCK;
	u8 Local_DeviceAddress = EEPROM_DEVICE_ADDRESS;
	u8 Local_WordAddress = EEPROM_START_ADDRESS;

	for (u8 i = 0; i < EEPROM_NUMBER_OF_PAGES; i++)
	{
		if(i==Local_temp)
		{
			Local_temp+=EEPROM_BLOCK;
			Local_DeviceAddress+=2;
		}

		I2C_SendStartCondition();									  	/*Send Start Condition*/
		I2C_SendSlaveAddressWithWrite(Local_DeviceAddress);			  	/*0b10100XX0   EEPROM device address with write*/
		I2C_MasterWriteDataByteAck(Local_WordAddress); 				  	/*Send word address I want to access inside EEPROM with ACK*/

		for (u8 i = 0; i < EEPROM_MAX_BUFFER_SIZE; i++)
		{
			I2C_MasterWriteDataByteAck(EEPROM_DEFAULT_DATA); 		  	/*Write DEFAULT Data To Clear EEPROM Page*/
		}

		Local_WordAddress += EEPROM_PAGE;                             	/*Go to next to page to start Clearing again till all pages are erased*/
		I2C_voidSendStopCondition();		 						  	/*Send Stop Condition*/
		_delay_ms(10);						 						 	 /*Wait 10ms specified by the data sheet so the EEPROM could complete the write process*/
	}
}

void EEPROM_voidWriteData(u16 Copy_u16Address, u8 Copy_u8Data)
{
	u8 Local_DeviceAddress =  (Copy_u16Address >> 7) & 0XFF;
	Local_DeviceAddress |= EEPROM_DEVICE_ADDRESS;
	u8 Local_WordAddress = (u8) Copy_u16Address & 0XFF;

	I2C_SendStartCondition();												/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(Local_DeviceAddress);						/*0b10100XX0   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Local_WordAddress);							/*Send word address I want to access inside EEPROM with ACK*/
	I2C_MasterWriteDataByteAck(Copy_u8Data);								/*Write Data to the previous specified word address in the EEPROM to write into with ACK*/

	I2C_voidSendStopCondition();											/*Send Stop Condition*/
	_delay_ms(10);															/*Wait 10ms specified by the data sheet so the EEPROM could complete the write process*/
}

u8 EEPROM_voidReadData(u16 Copy_u16Address)
{
	u8 Local_DeviceAddress =  (Copy_u16Address >> 7) & 0XFF;
	Local_DeviceAddress |= EEPROM_DEVICE_ADDRESS;
	u8 Local_WordAddress = (u8) Copy_u16Address & 0XFF;

	u8 Local_u8DataRead=0;													/*Variable to store my the data that i read*/

	/*Dummy Write just to go to a specific address to read from*/
	I2C_SendStartCondition();												/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(Local_DeviceAddress);						/*0b10100XX0   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Local_WordAddress);							/*Send word address I want to access inside EEPROM to read from with ACK*/
	/*End of Dummy Write*/

	/*Start Actual Read Process from specified address*/
	I2C_SendRepeatedStart();												/*Send Repeated Start Condition*/
	I2C_SendSlaveAddressWithRead(Local_DeviceAddress);						/*0b10100XX1   EEPROM device address with Read " This was missing i think i didn't see it "*/
	I2C_MasterReadDataByteNack(&Local_u8DataRead);							/*Read Data Byte from the previous specified address with NACK as specified by the data sheet for 1 byte Reads*/
	I2C_voidSendStopCondition();											/*Send Stop Condition*/
	/*End Read Process*/

	return Local_u8DataRead;
}

void EEPROM_voidSeqWrite(u16 Copy_u16Address, u8 *Copy_u8DataArr, u8 Copy_u8DataArrLength)
{
	u8 Local_DeviceAddress =  (Copy_u16Address >> 7) & 0XFF;
	Local_DeviceAddress |= EEPROM_DEVICE_ADDRESS;
	u8 Local_WordAddress = (u8) Copy_u16Address & 0XFF;

	I2C_SendStartCondition();												/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(Local_DeviceAddress);						/*0b10100XX0   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Local_WordAddress);							/*Send word address I want to access inside EEPROM with ACK*/

	for (u8 i=0;i<Copy_u8DataArrLength;i++)
	{
		I2C_MasterWriteDataByteAck(Copy_u8DataArr[i]);						/*Write Data from user provided array to EEPROM memory starting from the address provided by the user*/
	}

	I2C_voidSendStopCondition();											/*Send Stop Condition*/
	_delay_ms(10);						 									/*Wait 10ms specified by the data sheet so the EEPROM could complete the write process*/
}

void EEPROM_voidSeqRead(u16 Copy_u16Address, u8 *Copy_u8DataArr, u8 Copy_u8DataArrLength)
{
	u8 Local_DeviceAddress =  (Copy_u16Address >> 7) & 0XFF;
	Local_DeviceAddress |= EEPROM_DEVICE_ADDRESS;
	u8 Local_WordAddress = (u8) Copy_u16Address & 0XFF;

	/*Dummy Write just to go to a specific address to read from*/
	I2C_SendStartCondition();												/*Send Start Condition*/
	I2C_SendSlaveAddressWithWrite(Local_DeviceAddress);						/*0b10100XX0   EEPROM device address with write*/
	I2C_MasterWriteDataByteAck(Local_WordAddress);							/*Send word address I want to access inside EEPROM to read from with ACK*/
	/*End of Dummy Write*/

	/*Start Actual Read Process from specified address*/
	I2C_SendRepeatedStart();												/*Send Repeated Start Condition*/
	I2C_SendSlaveAddressWithRead(Local_DeviceAddress);						/*0b10100XX1   EEPROM device address with Read " This was missing i think i didn't see it "*/

	for(u8 i=0;i<Copy_u8DataArrLength-1;i++)
	{
		I2C_MasterReadDataByteAck(&Copy_u8DataArr[i]);						/*Read data bytes with ACK in order from specified address and allocate them in provided array until the element before last*/
	}
	I2C_MasterReadDataByteNack(&Copy_u8DataArr[Copy_u8DataArrLength-1]);	/*Reading last byte with NACK to start the ending protocol for my eeprom as specified in data sheet*/

	I2C_voidSendStopCondition();
}
