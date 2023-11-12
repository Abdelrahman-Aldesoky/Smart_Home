/******************I2C Private*******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:19/09/2023.****************/
/************************************************/

#ifndef _I2C_PRIVATE_H_
#define _I2C_PRIVATE_H_

#define TWCR *((volatile u8 *)0X56)
#define TWCR_TWINT 7
#define TWCR_TWEA 6
#define TWCR_TWSTA 5
#define TWCR_TWSTO 4
#define TWCR_TWWC 3
#define TWCR_TWEN 2
#define TWCR_TWIE 0
/*************************************************************************/
#define TWDR *((volatile u8 *)0X23)
/*************************************************************************/
#define TWAR *((volatile u8 *)0X22)
#define TWAR_TWGCE 0
/*************************************************************************/
#define TWSR *((volatile u8 *)0X21)
#define TWSR_TWPS0 0
#define TWSR_TWPS1 1
/*************************************************************************/
#define TWBR *((volatile u8 *)0X20)

/*********************TWSR_Status for Operations***************************/

#define START_ACK 0X08
#define REP_START_ACK 0X10
#define SLAVE_ADD_AND_WR_ACK 0X18
#define SLAVE_ADD_AND_WR_NOT_ACK 0X20
#define SLAVE_ADD_AND_RD_ACK 0X40
#define MSTR_WR_BYTE_ACK 0X28
#define MSTR_WR_BYTE_NOT_ACK 0X30
#define MSTR_RD_BYTE_WITH_ACK 0X50
#define MSTR_RD_BYTE_WITH_NOT_ACK 0X58
#define SLAVE_ADD_RCVD_RD_REQ 0XA8
#define SLAVE_ADD_RCVD_WR_REQ 0X60
#define SLAVE_DATA_REC 0X80
#define SLAVE_BYTE_TRANSMITTED 0XB8

#endif
