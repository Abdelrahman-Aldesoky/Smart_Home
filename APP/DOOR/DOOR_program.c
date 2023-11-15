/****************DOOR_program.c******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/TIMER1/TIMER1_interface.h"
#include "DOOR_config.h"
#include "DOOR_interface.h"
#include <util/delay.h>

void Door_Init(void)
{
	DIO_voidSetPinDirection(DOOR_PORT, DOOR_PIN, PIN_DIRECTION_OUTPUT); // OCR1A
	TIMER1_voidSetICRVal(ICR_VALUE);
	TIMER1_voidInit();
}

void Door_Lock(void)
{
	TIMER1_voidSetOCRVal(START_POINT);
}

void Door_Unlock(void)
{
	TIMER1_voidSetOCRVal(END_POINT);
}
