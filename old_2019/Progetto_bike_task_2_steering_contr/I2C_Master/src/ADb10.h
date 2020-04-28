/**********************************************************************************************************************************************************
 * Nome file: ADB10.H
 * Versione: 1.0
 * Descrizione:Driver per convertitore A/D 10-bit utilizzato per interfacciarsi ai valori letti dal sensore di corrente
 * con aggiunta del filtro FIR; Nella parte di codice relativo,
 * è presente un filtraggio FIR (finite impulse response ‐ risposta infinita all’impulso) sui valori ricevuti dal sensore di corrente.
 *  Questa tecnica di filtraggio a “media mobile ponderata”, sostituisce ad ogni nuovo valore letto, la media di tutti i valori nella finestra
 *   di campionamento, pesati in ordine di lettura; quindi il più recente avrà un peso maggiore rispetto ai valori meno recenti.
 *********************************************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                06/03/2019    1.0        Prima Release
 *******************************************************************************************************************************/

#ifndef _ADb10_H_
#define _ADb10_H_


/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Values for conversion of AD10 counts to voltage */
#define MAX_COUNT 1023.0 // Massimo valore in bit del valore restituito dal convertitore
#define VMAX 3.3 // Voltaggio massimo in ingresso alla scheda
#define VMIN 0.0 // Voltaggio minimo in ingresso alla scheda
#define lengthADb 1000 // Dimensione del campione per la media dei valori letti dai sensori
#define denom_tot 500500 // Denominatore per il calcolo della media
#include <stdbool.h>

static int stato = 0;//Variabile per il controllo dello stato del convertitore
                     //0)dispositivo in attesa
                     //1)dispositovo in lettura

/*******************************************************************************
Prototypes for exported functions
*******************************************************************************/
/*******************************************************************************
* Function name: ADb10_init
* Description  : Sets up ADb10 in single scan mode.
*	      	Used to read AN4,AN5.
* Arguments	: none
* Return value : none
*******************************************************************************/
void ADb10_init (void);
/*******************************************************************************
* Function name: ADb10_start
* Description  : Starts the ADC converter.  It will run continuously.
* Arguments	: none
* Return value : none
*******************************************************************************/
void ADb10_start (void);
/*****************************************************************************
	    * Function name : filtraggio
	    * Description   : Funzione per l' implementazione del filtro FIR sui valori letti
	    * 				  dal sensore di corrente, cioè fa la media pesata mobile continuamente, da 2 valori in su.
	    * 				  Arrivati a 1000 fa lo shift eliminando il primo valore preso,
	    * 				  aggiungendo l’ultimo valore letto, e fa la media.
	    * Arguments     : Valori letti dal registro convertiti in volt
	    * Return_value  : Media mobile dei valori del registro in volt
	    *******************************************************************************/
float filtraggio (float read,int contatore,float vettore_vout[]);
/***********************************************************************************************
	* Function name : ADb10_read
	* Description : Legge i valori dai 3 registri dell' ADb10 per filtrarli
	* Arguments	: canale selezionato
	* Return value : il valore letto e filtrato in volt
	***********************************************************************************************/
float ADb10_read(int sens_channel);

/*******************************************************************************
* Function name: ADb10_conversion_complete
* Description  : Checks to see if the conversion is complete.
* Arguments	: none
* Return value : True -> Conversion is complete
*            	False -> Otherwise
*******************************************************************************/
bool ADb10_conversion_complete (void);
/*******************************************************************************
	* Function name: Gestisci_ADb10
	* Description  : Manage conversion without waiting cycle for it to end
	* Arguments	: none
	* Return value : state
	*******************************************************************************/
int Gestisci_ADb10 (void);
/*******************************************************************************
	* Function name: vettore_Vout_init
	* Description  : Average voltage values' vector initialization to 0
	* Arguments	: none
	* Return value : none
	*******************************************************************************/
void vettore_vout_init(void);

/* End of file ADb10.h */
#endif
