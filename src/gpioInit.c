#include "gpioInit.h"
#include "gpioPort.h"
#include "adc.h"
#include "boardUtil.h"

#define MAX_READ_VALUE 0xfff

void init_gpio()
{
	
	/* Define the DCs for the DAC */
	uint8_t DCs = DAC_GPIO_0 | DAC_GPIO_1 | DAC_GPIO_2 | DAC_GPIO_3 | DAC_GPIO_4 | DAC_GPIO_5;
	
	/* Turn Clock ON for GPIOF (Push Up Button), GPIOE (Pot and PS2), and GPIOB (DC0-DC5) */	
	gpio_enable_port(PUSH_BUTTON_GPIO_BASE);
	gpio_enable_port(ADC_GPIO_BASE);
	gpio_enable_port(DAC_GPIO_BASE);
	
	/* Set pin for Push Up and DC0-DC5 to Digital */
	gpio_config_digital_enable(PUSH_BUTTON_GPIO_BASE, PUSH_BUTTON_UP_PIN);
	gpio_config_digital_enable(DAC_GPIO_BASE, DCs);
	
	/* Set pins for Pot and PS2 to analogic */
	gpio_config_analog_enable(ADC_GPIO_BASE, ADC_GPIO_X_PIN | ADC_GPIO_Y_PIN | ADC_GPIO_POT_PIN);
	
	/* Set PS2 and Push buttons as input and DC0-DC5 as output */
	gpio_config_enable_input(PUSH_BUTTON_GPIO_BASE, PUSH_BUTTON_UP_PIN);
	gpio_config_enable_input(ADC_GPIO_BASE, ADC_GPIO_X_PIN | ADC_GPIO_Y_PIN | ADC_GPIO_POT_PIN);
	
	gpio_config_enable_output(DAC_GPIO_BASE, DCs);
	
	/* Set pin for Push Up to PullUp */
	gpio_config_enable_pullup(PUSH_BUTTON_GPIO_BASE, PUSH_BUTTON_UP_PIN);
	
	/* Enable pins for Pot and PS2 as alaternate function (ADC) */
	gpio_config_alternate_function(ADC_GPIO_BASE, ADC_GPIO_X_PIN | ADC_GPIO_Y_PIN | ADC_GPIO_POT_PIN);
}

void read_anlogs(uint16_t * total_wave, uint16_t * duty, uint8_t * DAC)
{
	uint16_t ps2_x, ps2_y, pot;
	get_adc_values(ADC0_BASE, &ps2_x, &ps2_y, &pot);
	//Set the value for frequency
	if(ps2_y > (int)(0.8*MAX_READ_VALUE))				/* More than 80%, 4.5 KHz */
		*total_wave = 11111;
	else if(ps2_y > (int)(0.7*MAX_READ_VALUE))	/* Between 70% and 80%, 4.0 KHz */
		*total_wave = 12500;
	else if(ps2_y > (int)(0.6*MAX_READ_VALUE))	/* Between 60% and 70%, 3.5 KHz */
		*total_wave = 14285;
	else if(ps2_y > (int)(0.5*MAX_READ_VALUE))	/* Between 50% and 60%, 3.0 KHz */
		*total_wave = 16666;
	else if(ps2_y > (int)(0.4*MAX_READ_VALUE))	/* Between 40% and 50%, 2.5 KHz */
		*total_wave = 20000;
	else if(ps2_y > (int)(0.3*MAX_READ_VALUE))	/* Between 30% and 40%, 2.0 KHz */
		*total_wave = 25000;
	else if(ps2_y > (int)(0.2*MAX_READ_VALUE))	/* Between 20% and 30%, 1.5 KHz */
		*total_wave = 33333;
	else																				/* Less than 10%, 1.0 KHz */
		*total_wave = 50000;
	
	/* Set the value for  the Duty Cycle */
	if(ps2_x > (int)(.8*MAX_READ_VALUE))				/* More than 80%, duty cycle 50% */
		*duty = *total_wave/2;
	else if(ps2_x > (int)(.2*MAX_READ_VALUE))		/* Between 20% and 80%, duty cycle 33% */
		*duty = .33*(*total_wave);
	else																				/* Less than 20%, duty cycle 25% */
		*duty = *total_wave/4;
	
	/* Set DCA (Volume) */
	if(pot > (int)(0.75*MAX_READ_VALUE))
		*DAC = DAC_GPIO_5;
	else if(pot > (int)(0.6*MAX_READ_VALUE))
		*DAC = DAC_GPIO_4;
	else if(pot > (int)(0.45*MAX_READ_VALUE))
		*DAC = DAC_GPIO_3;
	else if(pot > (int)(0.30*MAX_READ_VALUE))
		*DAC = DAC_GPIO_2;
	else if(pot > (int)(0.15*MAX_READ_VALUE))
		*DAC = DAC_GPIO_1;
	else
		*DAC = DAC_GPIO_0;
		
}

bool Read_Up_Button(uint8_t * count)
{
	if((PUSH_BUTTON_GPIO_PERIPH->DATA & PUSH_BUTTON_UP_PIN) == 0)
	{
		if(*count < 16)
		{
			count++;
			return false;
		}
		else
			return true;
	}
	else
		count = 0;
	return false;
}


