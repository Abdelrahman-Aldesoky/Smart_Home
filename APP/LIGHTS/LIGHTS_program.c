/**************LIGHTS_program.c******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#include "../../UTILS/STD_TYPES.h"
#include "../../UTILS/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "LIGHTS_config.h"
#include "LIGHTS_interface.h"

void Light_voidInit(void)
{
	DIO_voidSetPinDirection(LIGHT_PORT, LIGHT_PIN, PIN_DIRECTION_OUTPUT);
}

void LightToggle(void)
{
	if (DIO_u8GetPinValue(LIGHT_PORT, LIGHT_PIN))
	{
		DIO_voidSetPinValue(LIGHT_PORT, LIGHT_PIN, PIN_VALUE_LOW);
	}
	else
	{
		DIO_voidSetPinValue(LIGHT_PORT, LIGHT_PIN, PIN_VALUE_HIGH);
	}
}
