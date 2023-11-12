/*******************BIT_MATH*********************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:14/08/2023.****************/
/************************************************/


#ifndef _BIT_MATH_H_
#define _BIT_MATH_H_

#define SET_BIT(REG, BIT_NO) REG = REG | (1 << BIT_NO)
#define CLR_BIT(REG, BIT_NO) REG = REG & (~(1 << BIT_NO))
#define TOG_BIT(REG, BIT_NO) REG = REG ^ (1 << BIT_NO)
#define GET_BIT(REG, BIT_NO) (REG >> BIT_NO) & 1

#endif
