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
#include "gpioInit.h"

#define CLOCKS_1_MS 50000

extern void serialDebugInit(void);
char  teamNumber[] = "11";
char person1[] = "Raul Martins";
char person2[] = "Denis Medeiros" ;

/* Global variables for interrupts */

volatile bool Alert_1ms;
volatile bool Alert_Timer0A;
volatile bool Alert_Timer0B;

bool soundGeneratorEnabled;

void print_ps2(void)
{
  uint32_t i;
	uint16_t psx, ps_y, pot;
  while(1)
  {

    if(get_adc_values(ADC0_BASE,&psx, &ps_y, &pot))
			printf("X Dir value : 0x%03x        Y Dir value : 0x%03x		 Pot value : 0x%03x\r",psx, ps_y, pot);
		else
			printf("Deu ruim\n\n");
    for(i=0;i<1000000; i++){}
    
  }
}

/* Generate square wave using timers and interrupts */

void generate_sound() {
	
	bool buttonPressed = false;
	uint8_t DAC, countButtonPressed = 0;
	uint16_t psx, psy, pot;
	uint16_t numClocksTotal, numClocks1, numClocks0, dataUpButton;	
	
	while(1) {
		
		print_ps2();
		/* Get UP button input */
		
		if(Alert_1ms) { /* Every 5 milliseconds */
			
			dataUpButton = PUSH_BUTTON_GPIO_PERIPH->DATA; /* Read data from the pin */
			
			if((dataUpButton & PUSH_BUTTON_UP_PIN) == 0) { /* Button pressed */
								
				if (countButtonPressed >= 16) {	/* If 16 ms has passed */
					
					if(!buttonPressed) { /* The button is not pressed */

						buttonPressed = true;
						
						/* Toggle the sound generator */
						
						if (!soundGeneratorEnabled) { 	
							soundGeneratorEnabled = true;
						} else {
							soundGeneratorEnabled = false;
						}
						
					}
					
				} else { /* Reset flags when the button is released */
					countButtonPressed++;
				}
				
			} else { /* Button released */
				countButtonPressed = 0;
				buttonPressed = false;
			}
			
			/* Reset the interrupt flag */
			Alert_1ms = false;
		}
		
		
		/* Verify if the sound generator is enabled */
		
		if(soundGeneratorEnabled) {
		
			/* Get frequency, duty cycle, and volume from PS2 and Pot */
			
			get_adc_values(ADC0_BASE, &psx, &psy, &pot);
			//read_anlogs(&numClocksTotal, &numClocks1, &DAC);
			//numClocks0 = 1 - numClocks1;
			/* Range for X and Y is from 0 to 4095 */
			/* The frequency of the processor de 50MHz */
			
			/* Get the frequency */
			
			if(psy < 819) { /* Less than 20%, 1 KHz */
				
				numClocksTotal = 50000;
				
			} else if(psy >= 819 && psy < 1229) { /* Between 20% and 30%, 1.5 KHz */
				
				numClocksTotal = 33333;
				
			} else if(psy >= 1229 && psy < 1638) { /* Between 30% and 40%, 2 KHz */
				
				numClocksTotal = 25000;
				
			} else if(psy >= 1638 && psy < 2048) { /* Between 40% and 50%, 2.5 KHz */
				
				numClocksTotal = 20000;
					
			} else if(psy >= 2048 && psy < 2457) { /* Between 50% and 60%, 3 KHz */
				
				numClocksTotal = 16666;
						
			} else if(psy >= 2457 && psy < 2867) { /* Between 60% and 70%, 3.5 KHz */
				
				numClocksTotal = 14285;
				
			} else { /* More than 70%, 4 KHz */
				
				numClocksTotal = 12500;
				
			}
			
				/* Get the duty cycle */
			
			if(psx < 819) { /* Less than 20%, duty cycle 25% */
				
				numClocks1 = numClocksTotal * 0.25;
				numClocks0 = numClocksTotal * 0.75;
				
			} else if(psx >= 819 && psx < 3276) { /* Between 20% and 80%, duty cycle 33% */
				
				numClocks1 = numClocksTotal * 0.33;
				numClocks0 = numClocksTotal * 0.67;
				
			} else { /* More than 80%, duty cycle 50% */
				
				numClocks1 = numClocksTotal * 0.5;
				numClocks0 = numClocks1;
				
			}
				
			/* Get the volume */
			
			if(pot < 615) { /* Less than 15%, DAC_0 */
				
				DAC = DAC_GPIO_0;
				
			} else if(pot >= 615 && pot < 1229) { /* Between 15% and 30%, DAC_1 */
				
				DAC = DAC_GPIO_1;
				
			} else if(pot >= 1229 && pot < 1843) { /* Between 30% and 45%, DAC_2 */
				
				DAC = DAC_GPIO_2;
			} else if(pot >= 1843 && pot < 2457) { /* Between 45% and 60%, DAC_3 */
				
				DAC = DAC_GPIO_3;
				
			} else if(pot >= 2457 && pot < 3072) { /* Between 60% and 75%, DAC_4 */
				
				DAC = DAC_GPIO_4;
				
			}else { /* More than 75%, DAC_5 */
				
				DAC = DAC_GPIO_5;
				
			}

			if(Alert_Timer0A) { /* Send signal 1 */
				
				DAC_GPIO_PERIPH->DATA |= DAC;
				Alert_Timer0A = false;
				start_timer0B(numClocks1);
				
			} else { /* Send signal 0 */

				DAC_GPIO_PERIPH->DATA &= ~DAC;
				Alert_Timer0B = false;
				start_timer0A(numClocks0);
				
			}
		
		}
		
	}
	
	
}



void configure_everything() {
	
	/* Initialize all GPIOs */
	
	init_gpio();
	
	/* Initialize the ADC */
	
	configure_adc0();
	
	/* Configure Timer 0 */
	
	configure_timer0();	
	
	/* Initialize the SysTick timer for waiting 1 millisecond */
	
	SysTick_Config(CLOCKS_1_MS);
	
	
}





//*****************************************************************************
//*****************************************************************************
int main(void)
{

  
  serialDebugInit();
  
  printf("\n\r");
  printf("***************************************\n\r");
  printf("ECE353 Spring 2015  - HW3 \n\r");
  printf("\tTeam %s\n\r", teamNumber);
  printf("\t%s\n\r", person1);
  printf("\t%s\n\r", person2);
  printf("****************************************\n\r");

	/* Sound generator should start enabled */
	
	soundGeneratorEnabled = true;
	
	configure_everything();
	generate_sound();
	
  while(1)
  {
  }
}
