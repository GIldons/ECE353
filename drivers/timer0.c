#include "timer0.h"

//*****************************************************************************
// Configure Timer 0 to be two 16-bit, periodic, count down timers.
// Do not set the values for TAILR or TBILR and DO NOT enable the timer.
//*****************************************************************************
TIMER0_Type *gp_timer = (TIMER0_Type*) TIMER0_BASE;
void configure_timer0()
{
  /* Turn on the clock for the timer */
  SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R0;

  /* Wait for the timer to turn on */
  while( (SYSCTL->PRTIMER & SYSCTL_PRTIMER_R0) == 0) {};	
		
	/* Disable timers A and B */
	gp_timer->CTL &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);
		
  /* Set the timer to be in 16-bit mode */
	gp_timer->CFG = TIMER_CFG_16_BIT;
		
	/* Set timers to be in periodic mode and count-down */
	gp_timer->TAMR = (TIMER_TAMR_TAMR_PERIOD | TIMER_TAMR_TACDIR);
	gp_timer->TBMR = (TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBCDIR);
		
	/* Activating Nested Vector Interrupt Controller for timers A and B */		
	NVIC_EnableIRQ(TIMER0A_IRQn); 
	NVIC_EnableIRQ(TIMER0B_IRQn); 
		
	/* Activate the interrupts for timers A and B*/
	gp_timer->IMR = (TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM);

}


//*****************************************************************************
// Turns on Timer0A and Turns Off Timer0B.  The TAILR is set to load_value
//*****************************************************************************
void start_timer0A(uint16_t load_value)
{
	stop_timer0B();
	gp_timer->TAILR = load_value;
	gp_timer->ICR |= TIMER_ICR_TATOCINT;		//Clear Interrupet;
	gp_timer->CTL |= TIMER_CTL_TAEN;				//Enable Interrupt;
}

//*****************************************************************************
// Turns off Timer0A.  This function does not alter the load value.
//*****************************************************************************
void stop_timer0A(void)
{
	gp_timer->CTL &= ~TIMER_CTL_TAEN;				//Disable Interrupt;
}

//*****************************************************************************
// Turns on Timer0B and Turns Off Timer0A.  The TBILR is set to load_value
//*****************************************************************************
void start_timer0B(uint16_t load_value)
{
	stop_timer0A();
	gp_timer->TBILR = load_value;
	gp_timer->ICR |= TIMER_ICR_TBTOCINT;		//Clear Interrupet;
	gp_timer->CTL |= TIMER_CTL_TBEN;				//Enable Interrupt;
}

//*****************************************************************************
// Turns off Timer0B.  This function does not alter the load value.
//*****************************************************************************
void stop_timer0B(void)
{
	/* Disable timer B*/
	gp_timer->CTL &= ~TIMER_CTL_TBEN;
}
