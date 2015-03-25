//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "gpioPort.h"
#include "adc.h"
#include "timer0.h"
#include "boardUtil.h"

#define CLOCKS_1_MS 50000

extern void serialDebugInit(void);
char  teamNumber[] = "12";
char person1[] = "Raul Martins";
char person2[] = "Denis Medeiros" ;

/* Global variables for interrupts */

volatile bool Alert_1ms;
volatile bool Alert_Timer0A = false;
volatile bool Alert_Timer0B = true;

void configure_essential()
{	
	peripheral_init();						/* Initialize all GPIOs */
	configure_adc0();							/* Initialize the ADC */
	configure_timer0();						/* Configure Timer 0 */
	SysTick_Config(CLOCKS_1_MS);	/* Initialize the SysTick timer for waiting 1 millisecond */
}


//*****************************************************************************
//*****************************************************************************
int main(void)
{
	
	uint16_t total_wave, duty;
	uint8_t DAC, button_count = 0;
	bool button_pressed = false;
	bool soundGeneratorEnabled = true;			/* Sound generator should start enabled */
  
  serialDebugInit();
  
  printf("\n\r");
  printf("***************************************\n\r");
  printf("ECE353 Spring 2015  - HW3 \n\r");
  printf("\tTeam %s\n\r", teamNumber);
  printf("\t%s\n\r", person1);
  printf("\t%s\n\r", person2);
  printf("****************************************\n\r");

	
	configure_essential();
	

  while(1)
  {
		if(Alert_1ms)
		{
			/* Read data from PS2 and the potentiometer */
			read_anlogs(&total_wave, &duty, &DAC); 
			
			/* Read the button and deboucing */
			if((PUSH_BUTTON_GPIO_PERIPH->DATA & PUSH_BUTTON_UP_PIN) == 0)
				if (button_count >= 16)
				{
					if(!button_pressed)
					{ 
							button_pressed = true;															
							soundGeneratorEnabled = !soundGeneratorEnabled;	
					}
				}
				else
					button_count++;
			else
			{
				button_count = 0;
				button_pressed = false;
			}
			Alert_1ms = false;
		}
		
		/* Generate sound based on current PS2 and potentiometer values */
		if(soundGeneratorEnabled)
		{
			if(Alert_Timer0A)
			{
				Alert_Timer0A = false;
				DAC_GPIO_PERIPH->DATA &= ~DAC;
				start_timer0B(total_wave - duty);
			}
			if(Alert_Timer0B)
			{
				Alert_Timer0B = false;
				DAC_GPIO_PERIPH->DATA |= DAC;
				start_timer0A(duty);
			}
		}
  }
}
