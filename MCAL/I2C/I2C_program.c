/******************I2C Program*******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:19/09/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "I2C_private.h"
#include "I2C_config.h"
#include "I2C_interface.h"

void I2C_voidMasterInit(u8 Copy_u8Address)
{
	if (Copy_u8Address == 0)
	{
		/*Do Nothing*/
		asm("NOP");
	}
	else
	{
		/*Copy Master Address in TWAR register*/
		TWAR = Copy_u8Address << 1;
	}

	/*set speed for 400KHZ*/
	CLR_BIT(TWSR, TWSR_TWPS0);
	CLR_BIT(TWSR, TWSR_TWPS1);
	TWBR = 2;
}

void I2C_voidSlaveInit(u8 Copy_u8Address)
{
	/*Copy Slave Address in TWAR register*/
	TWAR = Copy_u8Address << 1;
}

I2C_state I2C_SendStartCondition(void)
{
	u8 LocaL_u8Error = no_err;
	/*Send Start Condition*/
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWSTA) | (1 << TWCR_TWEN);

	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == START_ACK)
	{
	}
	else
	{
		LocaL_u8Error = start_err;
	}
	/*Return Error Status*/
	return LocaL_u8Error;
}

I2C_state I2C_SendRepeatedStart(void)
{
	u8 LocaL_u8Error = no_err;
	/*Send Repeated Start Condition*/
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWSTA) | (1 << TWCR_TWEN);
	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == REP_START_ACK)
	{
	}
	else
	{
		LocaL_u8Error = start_err;
	}
	/*Return Error Status*/
	return LocaL_u8Error;
}

I2C_state I2C_SendSlaveAddressWithWrite(u8 Copy_u8Address)
{
	u8 LocaL_u8Error = no_err;
	/*for 24c02 Address will always be 0b10100000*/
	u8 SlaveAddressWithWrite = CLR_BIT(Copy_u8Address, 0);
	TWDR = SlaveAddressWithWrite; /* Copy SlaveAddress in TWI data register*/
	/* Enable TWI, generation of ack */
	TWCR = (1 << TWCR_TWEN) | (1 << TWCR_TWINT) | (1 << TWCR_TWEA);
	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == SLAVE_ADD_AND_WR_ACK)
	{
	}
	else
	{
		LocaL_u8Error = slave_add_wr_err;
	}
	/*Return Error Status*/
	return LocaL_u8Error;
}

I2C_state I2C_SendSlaveAddressWithRead(u8 Copy_u8Address)
{
	u8 LocaL_u8Error = no_err;
	/*for 24c02 Address will always be 0b10100001*/
	u8 SlaveAddressWithRead = SET_BIT(Copy_u8Address, 0);
	TWDR = SlaveAddressWithRead; /* Copy SlaveAddress in TWI data register */
	/* Enable TWI, generation of ack */
	TWCR = (1 << TWCR_TWEN) | (1 << TWCR_TWINT) | (1 << TWCR_TWEA);
	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == SLAVE_ADD_AND_RD_ACK)
	{
	}
	else
	{
		LocaL_u8Error = slave_add_rd_err;
	}
	/*Return Error Status*/
	return LocaL_u8Error;
}

/*Master Write with ACK*/
I2C_state I2C_MasterWriteDataByteAck(u8 Copy_u8Data)
{
	u8 LocaL_u8Error = no_err;
	/* Copy data in TWI data register */
	TWDR = Copy_u8Data;
	/* Enable TWI, generation of ACK */
	TWCR = (1 << TWCR_TWEN) | (1 << TWCR_TWINT) | (1 << TWCR_TWEA);
	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == MSTR_WR_BYTE_ACK)
	{
	}
	else
	{
		LocaL_u8Error = mstr_wr_data_err;
	}
	/*Return Error Status*/
	return LocaL_u8Error;
}

/*Master Write with NACK*/
I2C_state I2C_MasterWriteDataByteNack(u8 Copy_u8Data)
{
	u8 LocaL_u8Error = no_err;
	/* Copy data in TWI data register */
	TWDR = Copy_u8Data;
	/* Enable TWI, no ACK */
	TWCR = (1 << TWCR_TWEN) | (1 << TWCR_TWINT);
	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == MSTR_WR_BYTE_NOT_ACK)
	{
	}
	else
	{
		LocaL_u8Error = mstr_wr_data_err;
	}
	/*Return Error Status*/
	return LocaL_u8Error;
}

/*Master Read with ACK*/
I2C_state I2C_MasterReadDataByteAck(u8 *Copy_u8pData)
{
	u8 LocaL_u8Error = no_err;

	/* Enable TWI, generation of ACK */
	TWCR = (1 << TWCR_TWEN) | (1 << TWCR_TWINT) | (1 << TWCR_TWEA);
	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == MSTR_RD_BYTE_WITH_ACK)
	{
	}
	else
	{
		LocaL_u8Error = mstr_rd_data_err;
	}
	/* Return received data */
	*Copy_u8pData = TWDR;
	/*Return Error Status*/
	return LocaL_u8Error;
}

/*Master Read with NACK*/
I2C_state I2C_MasterReadDataByteNack(u8 *Copy_u8pData)
{
	u8 LocaL_u8Error = no_err;
	/* Enable TWI, no ACK */
	TWCR = (1 << TWCR_TWEN) | (1 << TWCR_TWINT);
	/* Wait until TWI finish its current job */
	while (!(TWCR & (1 << TWCR_TWINT)))
	{
	}
	/*Error Handling*/
	if ((TWSR & 0b11111000) == MSTR_RD_BYTE_WITH_NOT_ACK)
	{
	}
	else
	{
		LocaL_u8Error = mstr_rd_data_err;
	}
	/* Return received data */
	*Copy_u8pData = TWDR;
	/*Return Error Status*/
	return LocaL_u8Error;
}

void I2C_voidSendStopCondition(void)
{
	/*Send Stop Condition*/
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN) | (1 << TWCR_TWSTO);
}
