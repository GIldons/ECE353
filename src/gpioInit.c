#include "gpioInit.h"
#include "gpioPort.h"
#include "adc.h"
#include "boardUtil.h"

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

