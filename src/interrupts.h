#ifndef __ECE353_INTERRUPTS_H__
#define __ECE353_INTERRUPTS_H__

#include <stdint.h>
#include <stdbool.h>
#include "TM4C123.h"

#endif

//*****************************************************************************
// Used to debounce the pushbutton and check the potentiometer.
//*****************************************************************************
void SysTick_Handler(void);

//*****************************************************************************
// Timer0A Interrupt Handler
// Used to time the "ON" portion of the waveform
//*****************************************************************************
void TIMER0A_Handler(void) ;

//*****************************************************************************
// Turns off Timer0B.  This function does not alter the load value.
//*****************************************************************************
void TIMER0B_Handler(void);





