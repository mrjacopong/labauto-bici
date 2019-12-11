
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description          Author
*         : 19.09.2014     1.01        First release        Andrea Bonci
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "cmt.h"
#include "s12adc.h"

/******************************************************************************
Variable Definitions
*******************************************************************************/
 unsigned int Tt_ms = 100;   /* desidered Tick Time expressed in ms */
//unsigned int Ft_Hz;         /* Tick Frequency expressed in Hz related to Tt_ms*/
 unsigned int PCLK_freq=48000000; /* RDK63N Peripheral Clock frequency in Hz (48MHz)*/
 unsigned int CMT_prescaler=128; /* Prescaler value of Compare Match Timer (CMT) peripheral */
                                /* Select from among four internal clock signals */
							    /* (PCLK/8, PCLK/32, PCLK/128, PCLK/512) */
                                /* CMT peripheral has 2 units CMT (CMT0 and CMT1) */
								/* each one having 2 channel with a 16bits counter (total 4 channels) */
 unsigned int CMCNT_count; /* istantaneous value of CMCNT (CMT Time Counter), */
                             /* CMCNT is the up-counter used to generate IR requests */
//unsigned int CMCOR_value; /* constant value of CMCOR, (CMT Constant Register), */


/******************************************************************************
Macro Definitions
*******************************************************************************/
 /* Tick Interval Generation */
 /* Compute number of CMT ticks in the selected Tick Time of Tt_ms=100ms*/
 unsigned int Ft_Hz = 10;  /* Tick Frequency expressed in Hz related to Tt_ms */
 /* Number of CMT ticks in Tt_ms (assuming 48 MHz PCLK and clock select = PCLK / 128) */
 unsigned int CMCOR_value = 375000; /* default 48000000/128 */
 unsigned int TICK_INTERVAL;
 /* default (48000000/128)/10=37500= number of CMT ticks in the Tt_ms time*/



/*******************************************************************************
* Function name: CMT_init
* Description  : Sets up CMT0 to generate interrupts at 100 ms
* Argument     : none
* Return value : none
*******************************************************************************/
void CMT_init (void)
{

#ifdef PLATFORM_BOARD_RDKRX63N
	/* Protect off */

	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */


#endif

    /* Power up CMT0 */

	MSTP(CMT0) = 0; /* Power up the CMT0 timer of the CMT */
     
#ifdef PLATFORM_BOARD_RDKRX63N
	/* Protect on  */
	SYSTEM.PRCR.WORD = 0xA500; /* Protect on */

#endif  

    /* Stop the clock */

	CMT.CMSTR0.BIT.STR0 = 0;

    /* Trigger 100 ms from now */

	CMT0.CMCNT=0;

	TICK_INTERVAL=37500;

	CMT0.CMCOR = TICK_INTERVAL;
    
    /* CMCR - Compare Match Timer Control Register
    b6      CMIE: 1 = Compare match interrupt (CMIn) enabled
    b1:b0   CKS:  2 = Clock selects is PCLK/128 (375 kHz @ PCLK = 48 MHz) 
    */

	CMT0.CMCR.WORD = 0x0042;
    
    /* Set interrupt priority in ICU */

	IPR(CMT0, CMI0) = 0x01;
    
    /* Enable the interrupt in the ICU */

	IEN(CMT0, CMI0) = 1;
    
    /* Start the clock running */ 

	CMT.CMSTR0.BIT.STR0 = 1;
} /* End of function CMT_init() */

/*******************************************************************************
* Function name: CMT_isr
* Description  : Interrupt Service Routine for CMT match interrupt.  Reads the 
*                results register from the ADC.  The ADC is running in continuous
*                scan mode so the results register will contain the latest reading.
*                The CMT counter (CMCNT) is reset to zero when this interrupt is
*                taken so no further action is required to keep the CMT firing
*                at regular intervals.  
*
*                The processing in this ISR must take less time than the CMT 
*                compare interval.  In this example, the compare interval is
*                set to 100 ms and the processing time for the ISR takes
*                roughly 12 ms.  Use the Performance Counters in HEW to measure
*                execution of your own code.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma interrupt (CMT_isr(vect=VECT(CMT0,CMI0)))
void CMT_isr (void)
{
    /* Latest ADC reading in counts*/
    unsigned short int adc_result;
    
    /* The ADC counts converted to a voltage */
    float adc_volts;
    
    /* String to display */
    char result_string[20];
    
    /* Toggle an LED with each interrupt */
    LED4 = ~LED4;
    
    /* Read the latest results from the ADC */
    adc_result = S12ADC_read();
    
    /* Convert to a string */
    sprintf(result_string, "Reading=%4d", adc_result);
       
    /* Update the display */
    lcd_display(LCD_LINE6, (const uint8_t *)result_string);

    /* Convert S12ADC counts to volts and display */    
    adc_volts = ((float) adc_result) / MAX_COUNTS * (VREFH - VREFL);
    
    /* Convert to a string */
    sprintf(result_string, "Volts  =%4.2f", adc_volts);
       
    /* Update the display */
    lcd_display(LCD_LINE8, (const uint8_t *)result_string);
} /* End of CMT_isr() */
