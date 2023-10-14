/******************I2C Program*******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:19/09/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "I2C_private.h"
#include "I2C_config.h"
#include "I2C_interface.h"

void I2C_voidMasterInit(u8 Copy_u8Address)
{
	if(Copy_u8Address==0)
	{
		asm("NOP");											/*Do Nothing*/
	}
	else
	{
		TWAR=Copy_u8Address<<1;								/*Copy Master Address in TWAR register*/
	}

	/*set speed for 400KHZ*/
	CLR_BIT(TWSR,TWSR_TWPS0);
	CLR_BIT(TWSR,TWSR_TWPS1);
	TWBR=2;
}

void I2C_voidSlaveInit(u8 Copy_u8Address)
{
	TWAR=Copy_u8Address<<1;									/*Copy Slave Address in TWAR register*/
}

I2C_state I2C_SendStartCondition(void)
{
	u8 LocaL_u8Error=no_err;

	TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWSTA)|(1<<TWCR_TWEN);	/*Send Start Condition*/
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==START_ACK)						/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=start_err;
	}

	return LocaL_u8Error;									/*Return Error Status*/
}

I2C_state I2C_SendRepeatedStart(void)
{
	u8 LocaL_u8Error=no_err;

	TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWSTA)|(1<<TWCR_TWEN);	/*Send Repeated Start Condition*/
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==REP_START_ACK)					/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=start_err;
	}

	return LocaL_u8Error;									/*Return Error Status*/
}

I2C_state I2C_SendSlaveAddressWithWrite(u8 Copy_u8Address)
{
	u8 LocaL_u8Error=no_err;
	/*for 24c02 Address will always be 0b10100000*/
	TWDR=Copy_u8Address;									/* Copy SlaveAddress in TWI data register */
	CLR_BIT(TWDR,0);										/* Clear bit 0 in TWDR to Write*/

	TWCR=(1<<TWCR_TWEN)|(1<<TWCR_TWINT)|(1<<TWCR_TWEA); 	/* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==SLAVE_ADD_AND_WR_ACK)				/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=slave_add_wr_err;
	}

	return LocaL_u8Error;									/*Return Error Status*/
}

I2C_state I2C_SendSlaveAddressWithRead(u8 Copy_u8Address)
{
	u8 LocaL_u8Error=no_err;
	/*for 24c02 Address will always be 0b10100001*/
	TWDR=Copy_u8Address;									/* Copy SlaveAddress in TWI data register */
	SET_BIT(TWDR,0);										/* Set bit 0 in TWDR to Read*/

	TWCR=(1<<TWCR_TWEN)|(1<<TWCR_TWINT)|(1<<TWCR_TWEA); 	/* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==SLAVE_ADD_AND_RD_ACK)				/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=slave_add_rd_err;
	}

	return LocaL_u8Error;									/*Return Error Status*/
}

I2C_state I2C_MasterWriteDataByteAck(u8 Copy_u8Data)		/*Master Write with ACK*/
{
	u8 LocaL_u8Error=no_err;

	TWDR=Copy_u8Data;										/* Copy data in TWI data register */

	TWCR=(1<<TWCR_TWEN)|(1<<TWCR_TWINT)|(1<<TWCR_TWEA); 	/* Enable TWI, generation of ACK */
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==MSTR_WR_BYTE_ACK)					/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=mstr_wr_data_err;
	}

	return LocaL_u8Error;									/*Return Error Status*/
}

I2C_state I2C_MasterWriteDataByteNack(u8 Copy_u8Data)		/*Master Write with NACK*/
{
	u8 LocaL_u8Error=no_err;

	TWDR=Copy_u8Data;										/* Copy data in TWI data register */

	TWCR=(1<<TWCR_TWEN)|(1<<TWCR_TWINT); 					/* Enable TWI, no ACK */
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==MSTR_WR_BYTE_NOT_ACK)				/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=mstr_wr_data_err;
	}

	return LocaL_u8Error;									/*Return Error Status*/
}

I2C_state I2C_MasterReadDataByteAck(u8 *Copy_u8pData)		/*Master Read with ACK*/
{
	u8 LocaL_u8Error=no_err;

	TWCR=(1<<TWCR_TWEN)|(1<<TWCR_TWINT)|(1<<TWCR_TWEA); 	/* Enable TWI, generation of ACK */
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==MSTR_RD_BYTE_WITH_ACK)			/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=mstr_rd_data_err;
	}

	*Copy_u8pData=TWDR;										/* Return received data */
	return LocaL_u8Error;									/*Return Error Status*/
}

I2C_state I2C_MasterReadDataByteNack(u8 *Copy_u8pData)		/*Master Read with NACK*/
{
	u8 LocaL_u8Error=no_err;
	TWCR=(1<<TWCR_TWEN)|(1<<TWCR_TWINT); 					/* Enable TWI, no ACK */
	while(!(TWCR&(1<<TWCR_TWINT)));							/* Wait until TWI finish its current job */

	if((TWSR&0b11111000)==MSTR_RD_BYTE_WITH_NOT_ACK)		/*Error Handling*/
	{

	}
	else
	{
		LocaL_u8Error=mstr_rd_data_err;
	}

	*Copy_u8pData=TWDR;					   				 	/* Return received data */
	return LocaL_u8Error;									/*Return Error Status*/
}

void I2C_voidSendStopCondition(void)
{
	TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN)|(1<<TWCR_TWSTO);  /*Send Stop Condition*/
}
