/****************TIMER1_PRIVATE******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:13/09/2023.****************/
/************************************************/

#ifndef MCAL_TIMER1_TIMER1_PRIVATE_H_
#define MCAL_TIMER1_TIMER1_PRIVATE_H_

#define TCCR1A  *((volatile u8*) 0x4F) //Timer/Counter1 Control Register A
#define TCCR1A_COM1A1    7
#define TCCR1A_COM1A0    6
#define TCCR1A_WGM11     1
#define TCCR1A_WGM10     0


#define TCCR1B  *((volatile u8*) 0x4E) //Timer/Counter1 Control Register B
#define TCCR1B_WGM12     3
#define TCCR1B_WGM13     4
#define TCCR1B_CS10      0
#define TCCR1B_CS11      1
#define TCCR1B_CS12      2


#define OCR1A   *((volatile u16*)0x4A) //u16 -> OCR1A (H & L) => 16-bit
//pointer to address of LOW => OCR1AL (incrementing vonniumen)

#define ICR1A   *((volatile u16*)0x46)


#endif /* MCAL_TIMER1_TIMER1_PRIVATE_H_ */
