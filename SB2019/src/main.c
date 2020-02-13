/********************************************************************************************************************************
 /********************************************************************************************************************************
 * Nome file: main.c
 * Versione: 1.0
 * Descrizione:Classe principale con lo scopo di agglomerare e sfruttare le altre classi per la corretta esecuzione del programma.
 *******************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                06/03/2019    1.0        Prima Release
 *******************************************************************************************************************************/

#include <encoder.h>
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


parametri_inr_config par_encoder_1;		//enc_1 -> trazione
identifica_enc enc_id_1;
parametri_inr_config par_encoder_2;		//enc_2 -> sterzo
identifica_enc enc_id_2;

void main(void)
{
	/* Creazione variabile di tipo struttura Motor che verra'  passata alle funzioni di gestione del PWM */
	//vanno create entrambe altrimenti non funziona
	Motor m1;//motore controllo sterzo
	Motor m2;//motore controllo trazione


	//inizializzazione per l'encoder
	// Creazione variabile di tipo struttura encoder che verrà usata per le funzioni dell'encoder.
	encoder_data enc_1;
	encoder_data enc_2;

	encoder_init_1();
	encoder_init_2();

	Init_Encoder_vars( &par_encoder_1 , &enc_id_1 , 1);
	Init_Encoder_vars( &par_encoder_2 , &enc_id_2 , 2);

	CMT_init();

	// Inizializzazione PWM dei due motori
	PWM_Init(3,12,&m2); // PWM motor 2 JN2-PIN 23
	PWM_Init(4,12,&m1); // PWM motor 1 JN2-PIN 22
	Init_Port_Dir();


    /* Initialize LCD */
    lcd_initialize();

    /* Clear LCD */
    lcd_clear();

    //prova dei PWM e dei pin di direzione
    //possono essere variate anche nel while (1)
    m1.volt_signal=5;
    m2.volt_signal=11;
	m2.direction_signal = 1;
	m1.direction_signal = 1;


    while (1)
    {
    	/* Calcolo del Duty-Cycle da inviare ai motori */

    	//da rivedere bene quale dei due funziona, o se devono funzionare entrambi
    	DutyCycle_to_Motor(&m2);
    	DutyCycle(30,4);


    	//Letture della velocità dall'Encoder con media.

    	Query_Enc(&enc_2, &par_encoder_2 , &enc_id_2);
    	Media_Speed(&enc_2, &par_encoder_2);
    	Query_Enc(&enc_1, &par_encoder_1 , &enc_id_1);
    	Media_Speed(&enc_1, &par_encoder_1);

    	Read_Vel_1LCD(&enc_1);
    	Read_Pos_1LCD(&enc_1);
    	Read_Vel_2LCD(&enc_2);
    	Read_Pos_2LCD(&enc_2);

    }
} /* End of function main() */

