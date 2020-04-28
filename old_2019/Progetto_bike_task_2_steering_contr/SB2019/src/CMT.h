/********************************************************************************************************************************
 * Nome file: CMT.h
 * Versione: 1.0
 * Descrizione: Si occupa della gestione della temporizzazione tramite l’uso dei timer per scandire i tempi di delay del ciclo
 * di controllo del PWM e dell’IMU.
 * *******************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                06/03/2019    1.0        Prima Release
 *******************************************************************************************************************************/

#ifndef _CMT_H_             /* Multiple inclusion prevention. */
#define _CMT_H_

/*******************************************************************************
Prototypes for exported functions
*******************************************************************************/

/*******************************************************************************
* Function name: CMT_init
* Description  : Sets up CMT0 to generate interrupts at 100 ms
* Arguments    : none
* Return value : none
*******************************************************************************/
void CMT_init (void) ;
/*******************************************************************************
* Function name: get_ms
* Description  : Serve per ottenere il valore contenuto dalla variabile mills
* Arguments    : none
* Return value : int32_t
*******************************************************************************/
int32_t get_ms();
/*******************************************************************************
* Function name: delay_ms
* Description  : Imposta un attesa di n millisecondi
* Arguments    : int32_t
* Return value : none
*******************************************************************************/
void delay_ms(int32_t t);

#endif                       /* Multiple inclusion prevention. */
