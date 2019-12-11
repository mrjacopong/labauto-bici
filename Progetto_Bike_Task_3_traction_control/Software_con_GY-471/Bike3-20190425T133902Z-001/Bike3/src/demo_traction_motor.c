
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 25.01.2019      1.00        First release
*         Author: Daniele Polucci, Emanuele Fares, Michele Brocchini
*******************************************************************************/
/*                                                                     */
/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <machine.h>
#include "platform.h"
#include "ADb10.h"
#include "cmt.h"
#include "pwm.h"
#include "sensore.h"
#include "PID.h"
#include "encoder_MOT1.h"

/*VARIABILI DI TIPO STRUTTURA*/

//Variabili di tipo strutture int32_t per il calcolo del tempo di esecuzione del ciclo while.
int32_t start;
int32_t stop;

// Creazione variabile di tipo struttura PID_Struct che verra'  passata alle funzioni di gestione del PID.
PIDSt_Type PID_P_curr_1;

// Creazione variabile di tipo struttura Motor utili alla gestione di più motori.
Motor m1;
Motor m2;
Motor m3;
Motor m4;

// Creazione variabile di tipo struttura encoder che verrà usata per le funzioni dell'encoder.
encoder_data_t_mt_1 enc_1;

// Creazione variabile di tipo struttura SensParam che verra'  passata alle funzioni di gestione del Sensore.
SensParam SENS_P;

// Declare display buffer.
uint8_t  lcd_buffer[13];

void main(void)
{
    /* Initialize LCD */
    lcd_initialize();

    /* Clear LCD */
    lcd_clear();

    /* Inizializzazione encoder */
    encoder_1_init();

    /* Initialize CMT */
	CMT_init();

	/* Inizializzazione della MTU2 per generare la PWM*/
	PWM_Init(1,V_MAX,&m4);
	PWM_Init(2,V_MAX,&m3); // PWM motor 3 J8-PIN 15
	PWM_Init(3,V_MAX,&m2); // PWM motor 2 JN2-PIN 23
	PWM_Init(4,V_MAX,&m1); // PWM motor 1 JN2-PIN 22

	//Inizializzazione delle porte di Direzione
	//Init_Port_Dir();

	/*Inizializzazione dei parametri per il controllo PID in coppia*/
	init_pid(&PID_P_curr_1,SET_PARAM_PID_TORQUE);

	// Inizializzazione del convertitore A/D a 10-bit
	ADb10_init();

	// Inizializzazione a 0 dei vettori per le tensioni lette dai sensori di corrente
	vettore_vout_init();

	// Inizializzazione del sensore di corrente
	sens_in(&SENS_P);

	/* Ciclo di calibrazione del sensore di corrente*/
	while( SENS_P.cont_calibration <= calibration_length){

		if(SENS_P.cont_calibration < calibration_length)
		{
			// Funzione di calibrazione dei sensori di corrente
			sens_calibration_init(&SENS_P);

			// Stampa a video dell'avviso di calibrazione dei sensori di corrente
			sprintf((char *)lcd_buffer, "Calibrating");
			lcd_display(LCD_LINE4, lcd_buffer);
			sprintf((char *)lcd_buffer, "Sensors:%d",SENS_P.cont_calibration);
			lcd_display(LCD_LINE5, lcd_buffer);
		}
		else if (SENS_P.cont_calibration == calibration_length){
			/*Calcolo del bias calcolato su 1000 elementi da sottrare al valore letto dai sensori*/
			sens_calibration_bias(&SENS_P);
			lcd_clear();

		}

	}

    //This is the main loop
	while (1)
    {
		//Inizio del calcolo del tempo di ciclo.
		start = get_ms();

		// Riferimento per PID in ampere.
		PID_P_curr_1.uc= 3;

		//Lettura dei valori di tensione legati alla corrente inviata ai motori e relativa conversione in corrente.
		sens_read(&SENS_P);

		/*Assegnazione dei valori letti dal sensore di corrente all'ingresso in retroazione del PID
		  per il controllo in corrente.*/
		PID_P_curr_1.y=SENS_P.sens_curr_2;

		// Calcolo del segnale di controllo da inviare ai motori.
		calcPID(&PID_P_curr_1);

		/* Assegniamo a il segnale di controllo in uscita dal PID al segnale in volt che genera il pwm
		per controllare il motore.*/
		m2.volt_signal = PID_P_curr_1.output;

		//Calcolo del Duty-Cycle da inviare al motore.
		DutyCycle_to_Motor(&m2);

		//Letture della velocità dall'Encoder con media.
		Query_Enc_1(&enc_1);
		Media_Speed_Enc_1(&enc_1);

		//Millisecondi di fine tempo di ciclo.
		stop= get_ms();

		//Stampa a video ogni 1s.
		if(!(get_ms()%1000)){
			//Stampa dei valori letti dall'Encoder in radianti per secondo.
			sprintf((char *)lcd_buffer, "enc: %.3f",enc_1.speed_in_rad_per_sec);
			lcd_display(LCD_LINE7, lcd_buffer);
			//Stampa su LCD la corrente assorbita.
			sprintf((char *)lcd_buffer, "C1: %.3f",SENS_P.sens_curr_2);
			lcd_display(LCD_LINE4, lcd_buffer);
			//Stampa il valore di riferimento al PID
			sprintf((char *)lcd_buffer, "RIF: %.1f",PID_P_curr_1.uc);
			lcd_display(LCD_LINE5, lcd_buffer);
			//Stampare su LCD il segnale mandato al motore
			sprintf((char *)lcd_buffer, "OUT: %.1f", m2.volt_signal);
			lcd_display(LCD_LINE6, lcd_buffer);
			//Stampare su LCD il tempo del ciclo di controllo.
			sprintf((char *)lcd_buffer, "T: %d",(stop-start));
			lcd_display(LCD_LINE8, lcd_buffer);
		}
   }
} /* End function main() */

