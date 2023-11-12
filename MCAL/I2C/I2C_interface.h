/******************I2C Interface*****************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:19/09/2023.****************/
/************************************************/

#ifndef _I2C_INTERFACE_H_
#define _I2C_INTERFACE_H_

typedef enum
{
	no_err = 0,
	start_err,
	slave_add_wr_err,
	slave_add_rd_err,
	mstr_wr_data_err,
	mstr_rd_data_err
} I2C_state;

void I2C_voidMasterInit(u8 Copy_u8Address);
void I2C_voidSlaveInit(u8 Copy_u8Address);

I2C_state I2C_SendStartCondition(void);
I2C_state I2C_SendRepeatedStart(void);

I2C_state I2C_SendSlaveAddressWithWrite(u8 Copy_u8Address);
I2C_state I2C_SendSlaveAddressWithRead(u8 Copy_u8Address);

I2C_state I2C_MasterWriteDataByteAck(u8 Copy_u8Data);
I2C_state I2C_MasterWriteDataByteNack(u8 Copy_u8Data);

I2C_state I2C_MasterReadDataByteAck(u8 *Copy_u8pData);
I2C_state I2C_MasterReadDataByteNack(u8 *Copy_u8pData);

void I2C_voidSendStopCondition(void);

#endif
