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

#define SEC_ONE 60000

volatile bool Alert_1ms;
volatile bool Alert_Timer0A;
volatile bool Alert_Timer0B;


extern void serialDebugInit(void);
char  teamNumber[] = "11";
char person1[] = "Raul Martins";
char person2[] = "Denis Medeiros" ;



//*****************************************************************************
//*****************************************************************************
int 
main(void)
{

  
  serialDebugInit();
  
  printf("\n\r");
  printf("***************************************\n\r");
  printf("ECE353 Spring 2015  - HW3 \n\r");
  printf("\tTeam %s\n\r", teamNumber);
  printf("\t%s\n\r", person1);
  printf("\t%s\n\r", person2);
  printf("****************************************\n\r");
	
	configure_timer0();
	start_timer0A(SEC_ONE);
	
  while(1)
  {
		if(Alert_Timer0A) {
			printf("Timer A\n");
			Alert_Timer0A = false;
		} else {
			printf("None\n");
		}
		
  }
}
