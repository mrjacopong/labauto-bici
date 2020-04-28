/********************************************************************************************************************************
 * Nome file: CMT.c
 * Versione: 1.0
 * Descrizione: Si occupa della gestione della temporizzazione tramite l’uso dei timer per scandire i tempi di delay del ciclo
 * di controllo del PWM e dell’IMU.
 * *******************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                06/03/2019    1.0        Prima Release
 *******************************************************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "CMT.h"

/******************************************************************************
Macro Definitions
*******************************************************************************/
/* The CMT in this example is clocked at a rate of 
   (PCLK / 128) or (48 MHz / 128) = 375 kHz */
/* TICK_INTERVAL defines how many times CMT is clocked in 100 ms 
   (or 1/10th of second) */
#define TICK_INTERVAL (48000000 / 128 / 1000)  /* 375 counts of CMT = 100 ms */


volatile int32_t millis = 0;//Contatore timer in ms

/*******************************************************************************
* Function name: get_ms
* Description  : Serve per ottenere il valore contenuto dalla variabile mills
* Arguments    : none
* Return value : int32_t
*******************************************************************************/
int32_t get_ms(){
	return millis;
}
/*******************************************************************************
* Function name: delay_ms
* Description  : Imposta un attesa di n millisecondi
* Arguments    : int32_t
* Return value : none
*******************************************************************************/
void delay_ms(int32_t t){
	int32_t last = millis;
	while(millis-last<t);
}


/*******************************************************************************
* Function name: CMT_init
* Description  : Sets up CMT0 to generate interrupts at 100 ms
* Arguments    : none
* Return value : none
*******************************************************************************/
void CMT_init (void)
{
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
#endif

    /* Power up CMT0 */
    MSTP(CMT0) = 0;   
     
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
#endif  
    
    /* Stop the clock */
    CMT.CMSTR0.BIT.STR0 = 0;

    /* Trigger 100 ms from now */
    CMT0.CMCOR = CMT0.CMCNT + TICK_INTERVAL;
    
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
* Description  : Interrupt Service Routine for CMT match interrupt.
*                The CMT counter (CMCNT) is reset to zero when this interrupt is
*                taken so no further action is required to keep the CMT firing
*                at regular intervals.  
*                During this ISR the state of the green LED's on the YRDK RX63N
*                are toggled.
*                The processing in this ISR must take less time than the CMT 
*                compare interval. Use the Performance Counters in HEW to measure
*                execution of your own code.
* Arguments    : none
* Return value : none
*******************************************************************************/
#pragma interrupt (CMT_isr(vect = VECT(CMT0, CMI0)))
static void CMT_isr (void)
{
    millis++;
} /* End of CMT_isr() */

