/*****************TIMER0 Config******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:06/09/2023.****************/
/************************************************/

#ifndef _TIMER0_CONFIG_H_
#define _TIMER0_CONFIG_H_

/**************************************************************************************************************/
/*******************************Start of Timer mode and PreScaler Configuration********************************/

/*TIMER0_MODE Options:
 * 1-CTC_MODE
 * 2-OVF_MODE
 * 3-PWM_PHASE_CORRECT
 * 4-FAST_PWM
 * */
#define TIMER0_MODE CTC_MODE

/*PreScaler options:
 * 1-DIV_BY_1
 * 2-DIV_BY_8
 * 3-DIV_BY_64
 * 4-DIV_BY_256
 * 5-DIV_BY_1024*/
#define PRE_SCALER DIV_BY_256

/********************************End of Timer mode and PreScaler Configuration*********************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
/*------------------------------------------------------------------------------------------------------------*/

/********Start of CTC Mode configuration based on compare value and CTC count Intervals can be adjusted********/

/*Settings here for 1 milli second Interval*/
/*OCR0 u can choose any value from 0 to 255*/
#define OCR0_COMPARE_VALUE 250
/*Number of times the CTC does interrupts until desired time is reached*/
#define CTC_COUNT 125

/****************************************End of CTC Mode configuration*****************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
/*------------------------------------------------------------------------------------------------------------*/

/*options:FAST_PWM
 * 1-S_TOP_C_CMP
 * 2-C_TOP_S_CMP*/
#define FAST_PWM_MODE S_TOP_C_CMP

#endif
