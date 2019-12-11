/********************************************************************************************************************************
 * Nome file: main.c
 * Versione: 1.0
 * Descrizione:Classe principale con lo scopo di agglomerare e sfruttare le altre classi per la corretta esecuzione del programma.
 *******************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                06/03/2019    1.0        Prima Release
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

#define PERIOD 5
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

/********************************************************************************************************************************
 * Dichiarazione variabili
 *******************************************************************************************************************************/

//variabili di controllo dell'imu
int32_t t, last = 0;
char msg[12];
char errorestr[12];
float errore;

//variabili dell'imu
IMU_raw imu_raw;
IMU_sens imu_sens;
IMU_temp imu_temp;
MAG_data mag_data;
AHRS_data ahrs_data;

/* Creazione variabile di tipo struttura PID_Struct che verra'  passata alle funzioni di gestione del PID */
PIDSt_Type PID_P_curr_1;

/* Creazione variabile di tipo struttura Motor che verra'  passata alle funzioni di gestione del PWM */
Motor m1;
Motor m2;
Motor m3;
Motor m4;

//variabili del sensore di corrente
SensParam SENS_P;

uint8_t  lcd_buffer[13];

/********************************************************************************************************************************
 * Nome funzione: main
 * descrizione:Classe principale con lo scopo di agglomerare e sfruttare le altre classi per la corretta esecuzione del programma.
 * Argomenti: none
 * Return value: none
 *******************************************************************************************************************************/
void main(void)
{

	switches_initialize();
	lcd_initialize();
	lcd_clear();
	CMT_init();

	//configurazione frequenza dell'imu
	setAHRSFrequency(1000.0/(float)PERIOD);			//Set frequenza di calcolo AHRS

	//Visualizzazione dello stato dell'imu su display
	switch (imu_init(&imu_sens)){					//Test per verificare il corretto funzionamento IMU
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
	switch (mag_init(&mag_data)){					//Test per verificare il corretto funzionamento MAG
			case (0x0):
				lcd_display(LCD_LINE2,"-MAG OK-");
				break;
			default:
				lcd_display(LCD_LINE2,"mag ERR");
		}

	//calibrazione dell'imu
	calibrationYPR(&msg, &mag_data);				//Calibrazione MAG


	// Inizializzazione della MTU2 per generare la PWM sul primo canale (MTIOC3A)
	PWM_Init(1,12,&m4);
	PWM_Init(2,12,&m3);
	PWM_Init(3,12,&m2);
	PWM_Init(4,12,&m1); // PWM motor 1 JN2-PIN 22

	/*Inizializzazione dei parametri per il controllo PID in coppia*/
	init_pid(&PID_P_curr_1,SET_PARAM_PID_ROLL);

	//Inizializzazione del convertitore A/D 10-bit
	ADb10_init();
	Init_Port_Dir(); // Inizializzazione delle porte di Direzione
	vettore_vout_init(); // Inizializzazione a 0 dei vettori per le tensioni lette dai sensori di corrente
	sens_in(&SENS_P); // Inizializzazione a 0 dei vettori di calibrazione del sensore di corrente

	while(1){//Ciclo infinito. Lettura valori IMU-MAG e sensore di corrente con relativo calcolo e invio dei valori di coppia

		//Lettura e controllo dello stato dell'imu con eventuale stampa su LCD
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

		//passaggio del valore letto dall'imu al PID
		PID_P_curr_1.uc=ahrs_data.RollDeg;
		// Ciclo che scandisce l'acquisizione dei dati dai sensori di corrente(ogni 2ms)
		if(get_ms()%2==0)
		    		{
			            //controlla se è stato letto un numero sufficente di valori dal sensore di corrente per la sua calibrazione
		    			if(SENS_P.cont_calibration < calibration_length)
		    			{
		    					// Stampa a video dell'avviso di calibrazione dei sensori di corrente
		    					sprintf((char *)lcd_buffer, "Calibrating");
		    					lcd_display(LCD_LINE4, lcd_buffer);
		    					sprintf((char *)lcd_buffer, "Sensors: %d",SENS_P.cont_calibration);
		    					lcd_display(LCD_LINE5, lcd_buffer);
		    				sens_calibration_init(&SENS_P);	// Funzione di calibrazione dei sensori di corrente
		    			}
		    			else if (SENS_P.cont_calibration == calibration_length)
		    				sens_calibration_bias(&SENS_P);   	/*Calcolo del bias calcolato su 1000 elementi da sottrare
		    											  al valore letto dai sensori*/
		    			else
		    			{
		    				//Lettura dei valori di tensione legati alla corrente inviata ai motori e relativa conversione in corrente
		    				sens_read(&SENS_P);
		    				/*Assegnazione dei valori letti dal sensore di corrente alla struttura attuale del PID
		    			  	  per il controllo in corrente*/
		    				PID_P_curr_1.y=SENS_P.sens_curr_1;
		    				// Calcolo del segnale di controllo da inviare ai motori
		    				// Inviamo il segnale di riferimento curr_ref al PID in corrente che provvedera' a generare
		    				// il segnale di controllo che invieremo ai motori.
		    				calcPID(&PID_P_curr_1);
		    				//Aggiornamento del valore di tensione del motore calcolato dal PID
		    				m1.volt_signal = PID_P_curr_1.output;

			                /* Calcolo del Duty-Cycle da inviare ai motori */
		    				DutyCycle_to_Motor(&m1);

		    				//Visualizzazione su display LCD dei parametri essenziali del PID per eventuale verifica esterna
		    				sprintf((char *)lcd_buffer, "S.C.:=%.4f",SENS_P.sens_curr_1);
		    		     	lcd_display(LCD_LINE4, lcd_buffer);
		    				sprintf((char *)lcd_buffer, "Roll=%.4f",ahrs_data.RollDeg);
		    				lcd_display(LCD_LINE5, lcd_buffer);
		    				sprintf((char *)lcd_buffer, "VoltPWM=%.4f",m1.volt_signal);
		    				lcd_display(LCD_LINE6, lcd_buffer);
		    			}
		    		}
	}
}
