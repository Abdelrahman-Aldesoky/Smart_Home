/****************TIMER0 Interface****************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:06/09/2023.****************/
/************************************************/

#ifndef _TIMER0_INTERFACE_H_
#define _TIMER0_INTERFACE_H_

void TIMER0_voidInit(void);
u8 TIMER0_voidSetCallBack(void (*Copy_ptrtofunc)(void));
void TIMER0_voidSetOcrValue(u8 Copy_u8OcrValue);
void TIMER0_voidSetPWMMode(void);

#endif
