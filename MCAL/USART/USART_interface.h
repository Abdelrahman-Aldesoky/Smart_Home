/****************USART Interface****************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:13/09/2023.****************/
/************************************************/

#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_

void USART_voidInit(void);
void USART_voidSend(u8 Copy_u8Data);
u8 USART_voidReceive(void);

#endif
