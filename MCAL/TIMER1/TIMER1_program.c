/*****************TIMER1_PROGRAM******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:13/09/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "TIMER1_config.h"
#include "TIMER1_interface.h"
#include "TIMER1_private.h"

void TIMER1_voidInit(void)
{
	//FAST PWM
	CLR_BIT(TCCR1A,TCCR1A_WGM10);
	SET_BIT(TCCR1A,TCCR1A_WGM11);
	SET_BIT(TCCR1B,TCCR1B_WGM12);
	SET_BIT(TCCR1B,TCCR1B_WGM13);

	//NON INVERTED MODE ==> SET ON TOP & CLR AT COMP
	CLR_BIT(TCCR1A,TCCR1A_COM1A0);
	SET_BIT(TCCR1A,TCCR1A_COM1A1);

	//PRESCALER 8
	CLR_BIT(TCCR1B,TCCR1B_CS10);
	SET_BIT(TCCR1B,TCCR1B_CS11);
	CLR_BIT(TCCR1B,TCCR1B_CS12);

}

void TIMER1_voidSetOCRVal(u16 Copy_u16Val)
{
	OCR1A=Copy_u16Val;
}

void TIMER1_voidSetICRVal(u16 Copy_u16Val)
{
	ICR1A=Copy_u16Val;
}


