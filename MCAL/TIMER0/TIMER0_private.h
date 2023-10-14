/*****************TIMER0 Private*****************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:06/09/2023.****************/
/************************************************/

#ifndef _TIMER0_PRIVATE_H_
#define _TIMER0_PRIVATE_H_

#define TCNT0           	*((volatile u8*)0X52)   /*Number of ticks stored here*/
#define TCCR0           	*((volatile u8*)0X53)   /*Timer counter control register*/
#define TIFR            	*((volatile u8*)0X58)   /*Flag register*/
#define TIMSK           	*((volatile u8*)0X59)   /*Interrupt Enable*/
#define OCR0            	*((volatile u8*)0X5C)   /*compare value*/

#define TCCR0_CS00      	0
#define TCCR0_CS01      	1
#define TCCR0_CS02      	2
#define TCCR0_WGM01     	3
#define TCCR0_COM00     	4
#define TCCR0_COM01     	5
#define TCCR0_WGM00    	 	6

#define TIFR_TOV0       	0   /*Interrupt flag over flow mode*/
#define TIFR_OCF0       	1   /*Interrupt flag compare mode*/

#define TIMSK_TOIE0     	0   /*Interrupt enable over flow mode*/
#define TIMSK_OCIE0     	1   /*Interrupt enable compare mode*/

/*options:FAST_PWM
 * 1-S_TOP_C_CMP
 * 2-C_TOP_S_CMP*/
#define S_TOP_C_CMP 		1
#define C_TOP_S_CMP			2

/*TIMER0_MODE Options:
 * 1-CTC_MODE
 * 2-OVF_MODE
 * 3-PWM_PHASE_CORRECT
 * 4-FAST_PWM
 * */
#define CTC_MODE			1
#define OVF_MODE    		2
#define PWM_PHASE_CORRECT	3
#define FAST_PWM			4

/*PreScaler options:
 * 1-DIV_BY_1
 * 2-DIV_BY_8
 * 3-DIV_BY_64
 * 4-DIV_BY_256
 * 5-DIV_BY_1024*/
#define DIV_BY_1 			1
#define DIV_BY_8			2
#define DIV_BY_64			3
#define DIV_BY_256			4
#define DIV_BY_1024			5

#endif
