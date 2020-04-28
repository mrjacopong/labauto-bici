/********************************************************************************************************************************
 * Nome file: sensore.h
 * Versione: 1.0
 * Descrizione:Si occupa della calibrazione e della lettura dei dati provenienti dal sensore di corrente.
 *******************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                06/03/2019    1.0        Prima Release
 *******************************************************************************************************************************/

#ifndef SRC_SENSORE_H_
#define SRC_SENSORE_H_


typedef struct
{
//Dichiarazione delle variabili
float bias1;                 // Fattori di correzione per i sensori di corrente
float Vout;                 // Tensione in uscita dai sensori di corrente
int cont_calibration;		// Contatore che tiene il numero di elementi da inserire in vettore_vout
float sum_Vout;            // Somma delle tensioni lette dai sensori di corrente e inserite in vettore_vout
float sens_curr_1;
}SensParam;



/*******************************************************************************
Macro definitions
*******************************************************************************/
#define calibration_length 1000         // Numero di valori richiesti per completare la calibrazione
#define sensitivity 0.000185			// Sensibilita'  del sensore di corrente (V/mA)
#define offset_vout 2.5					// Offset in volt corrispondente a 0A (V)

/*******************************************************************************
Prototypes for exported functions
*******************************************************************************/


/*******************************************************************************
* Function name: sens_calibration_init
* Description  : Current sensors calibration
* Arguments    : SensParam
* Return value : none
*******************************************************************************/
void sens_calibration_init (SensParam* SENS_);
/*******************************************************************************
* Function name: sens_calibration_bias
* Description  : Calculation function for current sensors' bias
* Arguments    : SensParam
* Return value : none
*******************************************************************************/
void sens_calibration_bias (SensParam* SENS);

/*******************************************************************************
* Function name: sens_read
* Description  : Acquisition function for voltage values proportional to the current
*				 from sensors
* Arguments    : SensParam
* Return value : none
*******************************************************************************/
void sens_read (SensParam* SENS);

/*******************************************************************************
* Function name: sens_in
* Description  : Inizializzazione a 0 del parametro di calibrazione
* Arguments    : SensParam
* Return value : none
*******************************************************************************/
void sens_in(SensParam* SENS);
/* End of file sensore.h */

#endif /* SRC_SENSORE_H_ */
