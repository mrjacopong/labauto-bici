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

//parametri_inr_config &par_encoder_1;
//identifica_enc enc_id_1;
//parametri_inr_config par_encoder_2;
//identifica_enc enc_id_2;

#define PERIOD 5
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif


	parametri_inr_config par_encoder_1;
	identifica_enc enc_id_1;
	parametri_inr_config par_encoder_2;
	identifica_enc enc_id_2;

void main(void)
{
/* MOTORI */
	Motor m1;	//motore sterzo
	Motor m2;	//motore trazione

	m1.volt_signal = 0.0;

	// Inizializzazione della MTU2 per generare la PWM sul primo canale (MTIOC3A)
	PWM_Init(3,12,&m2); // PWM motor 2 JN2-PIN 23
	PWM_Init(4,12,&m1); // PWM motor 1 JN2-PIN 22

	// vedere bene
	CMT_init();

	// Inizializzazione delle porte di Direzione
	Init_Port_Dir(); // solo per lo sterzo : pin 19 JN2
/* FINE MOTORI */

/* ENCODER */
	encoder_data enc_1; //encoder sterzo
	encoder_data enc_2;	//encoder trazione


	//inizializzazioni encoder e variabili utili
	// to do: cambiare encoder 2 con  3 (problema del pin)
	encoder_init_1();
	encoder_init_2();
	Init_Encoder_vars(&par_encoder_1 , &enc_id_1 , 1);
	Init_Encoder_vars(&par_encoder_2 , &enc_id_2 , 2);
/* FINE ENCODER */

/*LCD TEST*/
    lcd_initialize();
    lcd_clear();

    while (1)
    {
    	// conti encoder + visualizzazione
    	Query_Enc(&enc_1, &par_encoder_1 , &enc_id_1);
    	Media_Speed(&enc_1, &par_encoder_1);
    	Query_Enc(&enc_2, &par_encoder_2 , &enc_id_2);
    	Media_Speed(&enc_2, &par_encoder_2);
    	Read_Vel_1LCD(&enc_1);
    	Read_Pos_1LCD(&enc_1);
    	Read_Vel_2LCD(&enc_2);
    	Read_Pos_2LCD(&enc_2);
    	// fine encoder

    	//controllo direzione con switch
    	m1.direction_signal = SW1;
    	if ( SW2 ) {
    		m1.volt_signal = 6.0;
    	} else {
    		m1.volt_signal = 0.0;
    	}
		DutyCycle_to_Motor(&m1);
    	// premendo sw1 o meno setto la direzione e premendo sw2 aziono il motore: 6.0 v

    }
} /* End of function main() */
