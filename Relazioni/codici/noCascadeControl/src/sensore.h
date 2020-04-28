/*
 * sensore.h
 *
 *  Created on: 11 dic 2017
 *      Author: nicoe
 */

#ifndef SRC_SENSORE_H_
#define SRC_SENSORE_H_


typedef struct
{
//Dichiarazione delle variabili
float bias1;                 // Fattori di correzione per i sensori di corrente
float bias2;
float bias3;
float vout1;                 // Tensione in uscita dai sensori di corrente
float vout2;
float vout3;
int cont_calibration;		// Contatore che tiene il numero di elementi da inserire in vettore_vout
float sum_vout1;            // Somma delle tensioni lette dai sensori di corrente e inserite in vettore_vout
float sum_vout2;
float sum_vout3;
float sens_curr_1;
float sens_curr_2;
float sens_curr_3;
}SensParam;

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define calibration_length 1000
#define sensitivity 0.001			// Sensibilita'  del sensore di corrente (V/mA)
#define offset_vout 0			// Offset in volt corrispondente a 0A (V)

/*******************************************************************************
Prototypes for exported functions
*******************************************************************************/
void sens_in (SensParam* SENS);
void sens_calibration_init (SensParam* SENS_);
void sens_calibration_bias (SensParam* SENS);
void sens_read (SensParam*);

/* End of file sensore.h */

#endif /* SRC_SENSORE_H_ */
