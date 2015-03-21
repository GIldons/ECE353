#include "gpio_init.h"
#include "gpioPort.h"
#include "adc.h"

void init_gpio()
{
	uint8_t DCs = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	//Turn Clock for PORTF (Push Up Button)  PORTE (Pot and PS2) and PORTB (DC0-DC5);
	gpio_enable_port(GPIOF_BASE);
	gpio_enable_port(GPIOE_BASE);
	gpio_enable_port(GPIOB_BASE);
	
	//Set pin for Push Up and DC0-DC5 to Digital
	gpio_config_digital_enable(GPIOF_BASE, (1 << 1));
	gpio_config_digital_enable(GPIOB_BASE, DCs);
	
	//Set DC0-DC5 as output and other to input;
	gpio_config_enable_input(GPIOF_BASE, (1 << 1));
	gpio_config_enable_input(GPIOE_BASE, (1 << 1) | (1 << 2) | (1 << 3));
	gpio_config_enable_output(GPIOB_BASE, DCs);
	
	//Set pin for Push Up to PullUp
	gpio_config_enable_pullup(GPIOF_BASE, (1 << 1));
	
	//Set pins for Pot and PS2 to analogic
	gpio_config_analog_enable(GPIOE_BASE, (1 << 1) | (1 << 2) | (1 << 3));
	
	//Enable pins for Pot and PS2 as alaternate function (ADC);
	gpio_config_alternate_function(GPIOE_BASE, (1 << 1) | (1 << 2) | (1 << 3));
}

