/****************Smart Home main*****************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:08/10/2023.****************/
/************************************************/

#include "UTILS/STD_TYPES.h"
#include "UTILS/BIT_MATH.h"
#include "APP/SMART_HOME/SH_interface.h"

int main(void)
{
	SmartHome_voidInit();
	while (1)
	{
		SmartHome_voidStart();
	}
	return 0;
}
