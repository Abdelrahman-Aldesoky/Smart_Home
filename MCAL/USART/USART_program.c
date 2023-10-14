/******************USART Program*******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:13/09/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "../DIO/DIO_interface.h"
#include "USART_private.h"
#include "USART_config.h"
#include "USART_interface.h"

void USART_voidInit(void)
{
	DIO_voidSetPinDirection(PORTD_REG, PIN0, PIN_DIRECTION_INPUT);
	DIO_voidSetPinDirection(PORTD_REG, PIN1, PIN_DIRECTION_OUTPUT);
	// Set BaudRate -> 9600/8MhZ
	UBRRL = 51;
	UBRRH = 0;
	// Set Frame Format -> 8 data, 1 stop, No Parity
	UCSRC = 0x86;
	// Enable RX and TX
	UCSRB = 0x18;
	//one stop bit
	//no parity check
	//char size 8 bits
	//no interrupt
	//mode async normal
	//baudrate 9600
	//rxen and txen
}

void USART_voidSend(u8 Copy_u8Data)
{
	// Wait until transmission Register Empty
	while((GET_BIT(UCSRA,UCSRA_UDRE))==0X00);

	//  while((UCSRA&0x20) == 0x00);
	UDR = Copy_u8Data;
}

u8 USART_voidReceive(void)
{
	u8 ReceivedData=0;
	// Wait until Reception Complete
	while((GET_BIT(UCSRA,UCSRA_RXC))==0);
	//while((UCSRA&0x80) == 0x00);
	ReceivedData = UDR;

	/* Clear Flag */
	SET_BIT(UCSRA,UCSRA_RXC);
	return ReceivedData;
}
