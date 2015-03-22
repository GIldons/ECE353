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
#include "gpio_init.h"

extern void serialDebugInit(void);
char  teamNumber[] = "11";
char person1[] = "Raul Martins";
char person2[] = "Denis Medeiros" ;


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
  
	init_gpio();
	configure_adc0();
	
  while(1)
  {
		print_ps2();
  }
}
