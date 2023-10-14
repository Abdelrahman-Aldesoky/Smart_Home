/******************ADC Program*******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:06/09/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "TIMER0_private.h"
#include "TIMER0_config.h"
#include "TIMER0_interface.h"

static void (*Global_ptrtofunc)(void)=NULL;

void TIMER0_voidInit(void)
{
#if TIMER0_MODE == CTC_MODE
	/*Compare Match Mode*/
	CLR_BIT(TCCR0,TCCR0_WGM00);
	SET_BIT(TCCR0,TCCR0_WGM01);
	/*Compare match event interrupt enable*/
	SET_BIT(TIMSK,TIMSK_OCIE0);
	/*SET OCR0 to the compare value defined in config file*/
	OCR0=OCR0_COMPARE_VALUE;
#elif TIMER0_MODE == OVF_MODE
	/*Over flow Mode*/
	CLR_BIT(TCCR0,TCCR0_WGM00);
	CLR_BIT(TCCR0,TCCR0_WGM01);
	/*Over flow event interrupt enable*/
	SET_BIT(TIMSK,TIMSK_TOIE0);
#elif TIMER0_MODE == PWM_PHASE_CORRECT
	SET_BIT(TCCR0,TCCR0_WGM00);
	CLR_BIT(TCCR0,TCCR0_WGM01);
#elif TIMER0_MODE == FAST_PWM
	/*Fast pwm mode*/
	SET_BIT(TCCR0,TCCR0_WGM00);
	SET_BIT(TCCR0,TCCR0_WGM01);
#else
#error("WRONG CHOICE");
#endif


#if PRE_SCALER == DIV_BY_1
	/*No PreScaler*/
	SET_BIT(TCCR0,TCCR0_CS00);
	CLR_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);
#elif PRE_SCALER == DIV_BY_8
	/*PreScaler 8*/
	CLR_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);
#elif PRE_SCALER == DIV_BY_64
	/*PreScaler 64*/
	SET_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);
#elif PRE_SCALER == DIV_BY_256
	/*PreScaler 256*/
	CLR_BIT(TCCR0,TCCR0_CS00);
	CLR_BIT(TCCR0,TCCR0_CS01);
	SET_BIT(TCCR0,TCCR0_CS02);
#elif PRE_SCALER == DIV_BY_1024
	/*PreScaler 1024*/
	SET_BIT(TCCR0,TCCR0_CS00);
	CLR_BIT(TCCR0,TCCR0_CS01);
	SET_BIT(TCCR0,TCCR0_CS02);
#else
#error("WRONG PRE SCALER");
#endif
}

void TIMER0_voidSetPWMMode(void)
{
#if FAST_PWM_MODE == S_TOP_C_CMP
	/*Non inverted mode*/
	CLR_BIT(TCCR0,TCCR0_COM00);
	SET_BIT(TCCR0,TCCR0_COM01);
#elif FAST_PWM_MODE == C_TOP_S_CMP
	/*inverted mode*/
	SET_BIT(TCCR0,TCCR0_COM00);
	SET_BIT(TCCR0,TCCR0_COM01);
#else
#error("WRONG FAST_PWM_MODE");
#endif
}

void TIMER0_voidSetOcrValue(u8 Copy_u8OcrValue)
{
	OCR0=Copy_u8OcrValue;
}

u8 TIMER0_voidSetCallBack(void(*Copy_ptrtofunc)(void))
{
	u8 Local_u8Errorstate=OK;
	if(Copy_ptrtofunc!=NULL)
	{
		Global_ptrtofunc=Copy_ptrtofunc;
	}
	else
	{
		Local_u8Errorstate=NOK;
	}
	return Local_u8Errorstate;
}

void __vector_10(void)		__attribute__((signal));

void __vector_10(void)
{
	static u16 Local_u16Counter=0;
	Local_u16Counter++;

	if(CTC_COUNT == Local_u16Counter)
	{
		if(Global_ptrtofunc!=NULL)
		{
			Global_ptrtofunc();
			Local_u16Counter=0;
		}
	}
}
