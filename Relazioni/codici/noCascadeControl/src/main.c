/********************************************************************************************************************************
 * Nome file: main.c
 * Versione: 1.0
 * Descrizione:Classe principale con lo scopo di agglomerare e sfruttare le altre classi per la corretta esecuzione del programma.
 *******************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                27/04/2020    1.0        Prima Release
 *******************************************************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "CMT.h"
#include "imu.h"
#include "mag.h"
#include "AHRS.h"
#include "switches.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <machine.h>
#include "ADb10.h"
#include "pwm.h"
#include "sensore.h"
#include "PID.h"
#include "encoder.h"

#define PERIOD 5
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

/********************************************************************************************************************************
 * Dichiarazione variabili
 *******************************************************************************************************************************/
/* Variabili di tipo strutture int32_t per il calcolo del tempo di esecuzione del ciclo while. */
int32_t start;
int32_t stop;

/* Variabili di controllo dell'imu */
int32_t t, last = 0;
char msg[12];
char errorestr[12];
float errore;

/* variabili dell'imu */
IMU_raw imu_raw;
IMU_sens imu_sens;
IMU_temp imu_temp;
MAG_data mag_data;
AHRS_data ahrs_data;

/* Creazione variabile di tipo struttura PID_Struct che verra'  passata alle funzioni di gestione del PID */
PIDSt_Type PID_P_curr_1;
PIDSt_Type PID_P_curr_2;

/* Creazione variabile di tipo struttura Motor che verra'  passata alle funzioni di gestione del PWM */
/* vanno create entrambe altrimenti non funziona */
Motor m1;//motore controllo trazione
Motor m2;//motore controllo sterzo

/* Inizializzazione per l'encoder */
/* Creazione variabile di tipo struttura encoder che verrà usata per le funzioni dell'encoder. */
encoder_data enc_1;	//trazione
encoder_data enc_2;	//sterzo

parametri_inr_config par_encoder_1;		//enc_1 -> trazione
identifica_enc enc_id_1;

parametri_inr_config par_encoder_2;		//enc_2 -> sterzo
identifica_enc enc_id_2;

/* Variabili del sensore di corrente */
SensParam SENS_S;


uint8_t  lcd_buffer[13];

/********************************************************************************************************************************
 * Nome funzione: main
 * descrizione:Classe principale con lo scopo di agglomerare e sfruttare le altre classi per la corretta esecuzione del programma.
 * Argomenti: none
 * Return value: none
 *******************************************************************************************************************************/
void main(void)
{
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*                                                    inizializzazioni                                                         */
/*-----------------------------------------------------------------------------------------------------------------------------*/
	switches_initialize();

	encoder_init_1();
	encoder_init_2();

	Init_Encoder_vars( &par_encoder_1 , &enc_id_1 , 1);
	Init_Encoder_vars( &par_encoder_2 , &enc_id_2 , 2);


	lcd_initialize();

	lcd_clear();

	CMT_init();

	/* Configurazione frequenza dell'imu */
	setAHRSFrequency(1000.0/(float)PERIOD);//Set frequenza di calcolo AHRS

	/* Visualizzazione dello stato dell'imu su display */
	switch (imu_init(&imu_sens)){ //Test per verificare il corretto funzionamento IMU
		case (0x0):
				lcd_display(LCD_LINE1,"-IMU OK-");
				break;
		case (0x1):
				lcd_display(LCD_LINE1,"i2c ERR");
				break;
		case (0x2):
				lcd_display(LCD_LINE1,"mpu ERR 1");
				break;
		case (0x3):
				lcd_display(LCD_LINE1,"mpu ERR 2");
				break;
		case (0x4):
				lcd_display(LCD_LINE1,"mpu  ERR 3");
				break;
	}
	switch (mag_init(&mag_data)){ //Test per verificare il corretto funzionamento MAG
			case (0x0):
				lcd_display(LCD_LINE2,"-MAG OK-");
				break;
			default:
				lcd_display(LCD_LINE2,"mag ERR");
		}


	/* Calibrazione dell'imu */
	calibrationYPR(&msg, &mag_data);				//Calibrazione MAG

	/* Inizializzazione della MTU2 per generare la PWM sul primo canale (MTIOC4A e MTIOC4C) */
	PWM_Init(3,12,&m1); // PWM motor 1 JN2-PIN 23  ->  PE2
	PWM_Init(4,12,&m2); // PWM motor 2 JN2-PIN 22  ->  PE1

	/* Inizializzazione dei parametri per il controllo PID in coppia- MANUBRIO*/
	init_pid(&PID_P_curr_1,SET_PARAM_PID_TORQUE_2);
	/* Inizializzazione dei parametri per il controllo PID in coppia- TRAZIONE*/
	init_pid(&PID_P_curr_2,SET_PARAM_PID_TORQUE_1);

	/* Inizializzazione del convertitore A/D 10-bit */
	ADb10_init();

	/* Inizializzazione delle porte di Direzione per i motori */
	Init_Port_Dir();

	/* Inizializzazione a 0 dei vettori per le tensioni lette dai sensori di corrente */
	vettore_vout_init();

	/* CALIBRAZIONE SENSORI*/
	/* Inizializzazione a 0 dei vettori di calibrazione del sensore di corrente */
	sens_calibration_init(&SENS_S);

	/* Ciclo di calibrazione del sensore di corrente */
			while(SENS_S.cont_calibration <= calibration_length){

				if(SENS_S.cont_calibration < calibration_length)
				{
					/* Funzione di calibrazione dei sensori di corrente */
					sens_calibration_init(&SENS_S);

					/* Stampa a video dell'avviso di calibrazione dei sensori di corrente */
					sprintf((char *)lcd_buffer, "Calibrating");
					lcd_display(LCD_LINE4, lcd_buffer);
					sprintf((char *)lcd_buffer, "Sensors:%d",SENS_S.cont_calibration);
					lcd_display(LCD_LINE5, lcd_buffer);
				}
				else if (SENS_S.cont_calibration == calibration_length){
					/* Calcolo del bias calcolato su 1000 elementi da sottrare al valore letto dai sensori */
					sens_calibration_bias(&SENS_S);
					lcd_clear();

				}

			/* Lo stesso ciclo di prima ma con SENS_T al posto di SENS_S */
			/*while(SENS_T.cont_calibration <= calibration_length){

							if(SENS_T.cont_calibration < calibration_length)
							{
								// Funzione di calibrazione dei sensori di corrente
								sens_calibration_init(&SENS_T);

								// Stampa a video dell'avviso di calibrazione dei sensori di corrente
								sprintf((char *)lcd_buffer, "Calibrating");
								lcd_display(LCD_LINE4, lcd_buffer);
								sprintf((char *)lcd_buffer, "Sensors:%d",SENS_T.cont_calibration);
								lcd_display(LCD_LINE5, lcd_buffer);
								}
								else if (SENS_T.cont_calibration == calibration_length){
									/*Calcolo del bias calcolato su 1000 elementi da sottrare al valore letto dai sensori
									sens_calibration_bias(&SENS_T);
									lcd_clear();

								}*/
			}

/*-----------------------------------------------------------------------------------------------------------------------------*/
/*                                                   Loop per il controllo                                                     */
/*-----------------------------------------------------------------------------------------------------------------------------*/

	/* Ciclo infinito. Lettura valori IMU-MAG e sensore di corrente con relativo calcolo e invio dei valori di coppia */
	while(1){

		/* Inizio del calcolo del tempo di ciclo. */
		start = get_ms();

		/* Lettura e controllo dello stato dell'imu con eventuale stampa su LCD */
		if(imu_read(&imu_raw, &imu_sens, &imu_temp)){
				lcd_display(LCD_LINE3,"imu_read ERR");
			}
		if(mag_read(&mag_data)){
				lcd_display(LCD_LINE4,"mag_read ERR");
			}

		getYPR(&mag_data, &imu_temp, &ahrs_data);
		errore=mag_data.x*mag_data.x+mag_data.y*mag_data.y+mag_data.z*mag_data.z;
		sprintf(errorestr, "%f", (errore-mag_data.ABS)/mag_data.ABS);

		lcd_display(LCD_LINE5,(uint8_t*)errorestr);

		/* passaggio del valore letto dall'imu al PID - STERZO */
		PID_P_curr_1.uc=0;

		/* Riferimento per PID in ampere. - TRAZIONE */
		PID_P_curr_2.uc=3;

		/* Lettura dei valori di tensione legati alla corrente inviata ai motori e relativa conversione in corrente-STERZO */
		sens_read(&SENS_S);

		/* Lettura dei valori di tensione legati alla corrente inviata ai motori e relativa conversione in corrente-TRAZIONE */
		//sens_read(&SENS_T);

		/* Assegnazione dei valori letti dal sensore di corrente alla struttura attuale del PID per il controllo in corrente */
		PID_P_curr_1.y=SENS_S.sens_curr_1;

		/* Assegnazione dei valori letti dal sensore di corrente all'ingresso in retroazione del PID per il controllo in corrente */
		PID_P_curr_2.y=SENS_S.sens_curr_2;

		/* Calcolo del segnale di controllo da inviare ai motori
		   Inviamo il segnale di riferimento curr_ref al PID in corrente che provvedera' a generare
		   il segnale di controllo che invieremo ai motori:STERZO  */
		calcPID(&PID_P_curr_1);
		/* Calcolo del segnale di controllo da inviare ai motori: TRAZIONE */
		calcPID(&PID_P_curr_2);
		/* Aggiornamento del valore di tensione del motore calcolato dal PID:STERZO */
		m1.volt_signal = PID_P_curr_1.output;
		/* Assegniamo a il segnale di controllo in uscita dal PID al segnale in volt che genera il pwm per controllare il motore:TRAZIONE */
		m2.volt_signal = PID_P_curr_2.output;


		/* Calcolo del Duty-Cycle da inviare ai motori */
		DutyCycle_to_Motor(&m1);
		DutyCycle_to_Motor(&m2);

		/* Letture della velocità dall'Encoder con media */
    	Query_Enc(&enc_2, &par_encoder_2 , &enc_id_2);
    	Media_Speed(&enc_2, &par_encoder_2);

    	Query_Enc(&enc_1, &par_encoder_1 , &enc_id_1);
    	Media_Speed(&enc_1, &par_encoder_1);

		/* Millisecondi di fine tempo di ciclo. */
		stop= get_ms();

/* LCD DEL MOTORE DI TRAZIONE
		//Stampa a video ogni 1s.
				if(!(get_ms()%1000)){
					//Stampa dei valori letti dall'Encoder in radianti per secondo.
					sprintf((char *)lcd_buffer, "enc: %.3f",enc_1.speed_in_rad_per_sec);
					lcd_display(LCD_LINE7, lcd_buffer);
					//Stampa su LCD la corrente assorbita.
					sprintf((char *)lcd_buffer, "C1: %.3f",SENS_P.sens_curr_1);
					lcd_display(LCD_LINE4, lcd_buffer);
					//Stampa il valore di riferimento al PID
					sprintf((char *)lcd_buffer, "RIF: %.3f",PID_P_curr_1.uc);
					lcd_display(LCD_LINE5, lcd_buffer);
					//Stampare su LCD il segnale mandato al motore
					sprintf((char *)lcd_buffer, "OUT: %.3f", m1.volt_signal);
					lcd_display(LCD_LINE6, lcd_buffer);
					//Stampare su LCD il tempo del ciclo di controllo.
					sprintf((char *)lcd_buffer, "T: %d",(stop-start));
					lcd_display(LCD_LINE8, lcd_buffer);
				}*/

		/* Visualizzazione su display LCD dei parametri essenziali del PID per eventuale verifica esterna */
		//sprintf((char *)lcd_buffer, "S.C.:=%.4f",SENS_S.sens_curr_1);
		//lcd_display(LCD_LINE4, lcd_buffer);

		sprintf((char *)lcd_buffer, "RIF=%.1f",PID_P_curr_1.uc);
		lcd_display(LCD_LINE5, lcd_buffer);
		sprintf((char *)lcd_buffer, "VoltPWM=%.1f",m1.volt_signal);
		lcd_display(LCD_LINE6, lcd_buffer);

					}


}
