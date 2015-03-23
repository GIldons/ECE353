#include "adc.h"
#include "../include/sysctrl.h"

/******************************************************************************
 * Initializes ADC to use Sample Sequencer #3, triggered by software, no IRQs
 *****************************************************************************/
void configure_adc0()
{
	
  ADC0_Type  *myADC = (ADC0_Type *) ADC0_BASE;
  uint32_t rcgc_adc_mask = SYSCTL_RCGCADC_R0;
  uint32_t pr_mask = SYSCTL_PRADC_R0;
  
  /* Turn on the ADC Clock */
  SYSCTL->RCGCADC |= rcgc_adc_mask;
  
  /* Wait for ADCx to become ready */
  while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
    
  /* Disable sample sequencer #2 by writing a 0 to the 
   corresponding ASENn bit in the ADCACTSS register */
	myADC->ACTSS &= ~ADC_ACTSS_ASEN2;
	
  /* Set the event multiplexer to trigger conversion on a software trigger
   for sample sequencer #2. */
	myADC->EMUX &= ~ADC_EMUX_EM2_ALWAYS;
	
  /* Set IE2 and END2 in SSCTL2 */
		
	myADC->SSCTL2 = (ADC_SSCTL2_IE2 | ADC_SSCTL2_END2); /* This is not a mask */
	
	/* Set correct channels */
	myADC->SSMUX2 = ((0 << ADC_SSMUX2_MUX0_S) 
									|(1 << ADC_SSMUX2_MUX1_S) 
									| (2 << ADC_SSMUX2_MUX2_S));
		
}

/******************************************************************************
 * Reads SSMUX3 for the given ADC.  Busy waits until completion
 *****************************************************************************/
bool get_adc_values( uint32_t adc_base, uint16_t *ps2_x, uint16_t *ps2_y, uint16_t *pot)
{
  ADC0_Type  *myADC = (ADC0_Type *)adc_base;
  
  if( adc_base == 0)
  {
    return false;
  }
	
  /* Enable SS2 */
  myADC->ACTSS |= ADC_ACTSS_ASEN2;  
	
	/* Start the SS2 */
  myADC->PSSI = ADC_PSSI_SS2; 

	/* Wait until the conversion is complete */
  while( (myADC->RIS & ADC_RIS_INR2)  == 0){  }
	
  *ps2_y = myADC->SSFIFO2 & 0xFFF;    /* Read 12-bit data */
  *ps2_x = myADC->SSFIFO2 & 0xFFF;    /* Read 12-bit data */
  *pot = myADC->SSFIFO2 & 0xFFF;      /* Read 12-bit data */
	
	/* Ack the conversion */
  myADC->ISC  = ADC_ISC_IN2;   

	/* Disable SS2 */
  myADC->ACTSS &= ~ADC_ACTSS_ASEN2; 
	
	return true;
}

