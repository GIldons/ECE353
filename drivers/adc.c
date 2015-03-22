#include "adc.h"
#include "../include/sysctrl.h"

/******************************************************************************
 * Initializes ADC to use Sample Sequencer #3, triggered by software, no IRQs
 *****************************************************************************/
void configure_adc0()
{
  ADC0_Type  *myADC;
  uint32_t rcgc_adc_mask = SYSCTL_RCGCADC_R0;
  uint32_t pr_mask = SYSCTL_PRADC_R0;
	myADC = (ADC0_Type *) ADC0_BASE;
  
  // Turn on the ADC Clock
  SYSCTL->RCGCADC |= rcgc_adc_mask;
  
  // Wait for ADCx to become ready
  while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
    
  // disable sample sequencer #2 by writing a 0 to the 
  // corresponding ASENn bit in the ADCACTSS register 
	myADC->ACTSS &= ~ADC_ACTSS_ASEN2;
	
  // Set the event multiplexer to trigger conversion on a software trigger
  // for sample sequencer #2.
	myADC->EMUX &= ~ADC_EMUX_EM2_ALWAYS;
	
  // Set IE0, IE1, IE2 and END2 in SSCTL2
	myADC->SSCTL2 = (ADC_SSCTL2_IE2 | ADC_SSCTL2_END2);
	myADC->SSCTL2 &= ~(ADC_SSCTL0_IE0);
	myADC->SSCTL2 &= ~(ADC_SSCTL1_IE1);
	myADC->SSCTL2 &= ~(ADC_SSCTL2_END0);
	myADC->SSCTL2 &= ~(ADC_SSCTL2_END1);
}

/******************************************************************************
 * Reads SSMUX3 for the given ADC.  Busy waits until completion
 *****************************************************************************/
bool get_adc_values( uint32_t adc_base, uint16_t *ps2_x, uint16_t *ps2_y, uint16_t *pot)
{
  ADC0_Type  *myADC;
  
  if( adc_base == 0)
  {
    return false;
  }
  
  myADC = (ADC0_Type *)adc_base;
  myADC->SSMUX2 = 2;          // Set the Channel
  myADC->ACTSS |= ADC_ACTSS_ASEN2;  // Enable SS2
  myADC->PSSI = ADC_PSSI_SS2;     // Start SS2
  while( (myADC->RIS & ADC_RIS_INR2)  == 0){  }
	
  *pot = myADC->SSFIFO2 & 0xFFF;    // Read 12-bit data
  *ps2_y = myADC->SSFIFO2 & 0xFFF;    // Read 12-bit data
  *ps2_x = myADC->SSFIFO2 & 0xFFF;    // Read 12-bit data
  
  myADC->ISC  = ADC_ISC_IN2;          // Ack the conversion
	return true;
}

