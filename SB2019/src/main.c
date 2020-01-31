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

parametri_inr_config par_encoder_1;
identifica_enc enc_id_1;
parametri_inr_config par_encoder_2;
identifica_enc enc_id_2;

#define PERIOD 5
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif



void main(void)
{
	//par_encoder_1.MODULO_ENC=7;
	/* Creazione variabile di tipo struttura Motor che verra'  passata alle funzioni di gestione del PWM */
	Motor m1;//motore controllo sterzo
	Motor m2;//motore controllo trazione
	//Motor m3;
	//Motor m4;

	// Creazione variabile di tipo struttura encoder che verrà usata per le funzioni dell'encoder.
	encoder_data enc_1;
	encoder_data enc_2;


	encoder_data enc_steering;

	//inizializzazione per l'encoder
	encoder_init_1();
	encoder_init_2();
	Init_Encoder_vars( &par_encoder_1 , &enc_id_1 , 1);
	Init_Encoder_vars( &par_encoder_2 , &enc_id_2 , 2);

	// Inizializzazione della MTU2 per generare la PWM sul primo canale (MTIOC3A)
	//PWM_Init(1,12,&m4);
	//PWM_Init(2,12,&m3); // PWM motor 3 J8-PIN 15
	PWM_Init(3,12,&m2); // PWM motor 2 JN2-PIN 23
	PWM_Init(4,12,&m1); // PWM motor 1 JN2-PIN 22

	//encoder
	CMT_init();

	// Inizializzazione delle porte di Direzione
	//Init_Port_Dir();

    /* Initialize LCD */
    lcd_initialize();

    /* Clear LCD */
    lcd_clear();
    m1.volt_signal=5;
    m2.volt_signal=11;
float a=0;
    while (1)
    {
    	//Aggiornamento del valore di tensione del motore calcolato dal PID:STERZO
//for(int i=0;i<1000000;i++){}
//a=a+0.5;
//if (a>11.5)
//	a=0;
//m1.volt_signal=a;
    	/* Assegniamo a il segnale di controllo in uscita dal PID al segnale in volt che genera il pwm per controllare il motore:TRAZIONE*/
    	//m2.volt_signal = 11;
    	/* Calcolo del Duty-Cycle da inviare ai motori */
    	//DutyCycle_to_Motor(&m1);
    	//Calcolo del Duty-Cycle da inviare al motore.
    	//DutyCycle_to_Motor(&m2);
    	//DutyCycle(50,4);
    	//DutyCycle(80,3);
    	//DutyCycle(60,2);
    	//DutyCycle(60,1);
    	//Letture della velocità dall'Encoder con media.

    	//Query_Enc_1(&enc_1);
    	//Media_Speed_Enc_1(&enc_1);

 //    	printf("%f", enc_1.speed_in_rad_per_sec);
    	//Read_Vel_1LCD(&enc_1);
    	//Read_Pos_1LCD(&enc_1);
    	Query_Enc(&enc_1, &par_encoder_1 , &enc_id_1);
    	Media_Speed(&enc_1, &par_encoder_1);
    	Query_Enc(&enc_2, &par_encoder_2 , &enc_id_2);
    	Media_Speed(&enc_2, &par_encoder_2);
//    	Media_Speed_Enc_1(&enc_steering);
    	Read_Vel_1LCD(&enc_1);
    	Read_Pos_1LCD(&enc_1);
    	Read_Vel_2LCD(&enc_2);
    	Read_Pos_2LCD(&enc_2);

    }
} /* End of function main() */

/* interruptsss */


/* Implementazione degli interrupt per la gestione della lettura dell'encoder 1 */
#pragma interrupt MTU1_TGIA1_isr(vect = VECT_MTU1_TGIA1, enable)
static void MTU1_TGIA1_isr(void)
{
    volatile unsigned char dummy;

    if(ICU.IR[VECT_MTU1_TGIA1].BIT.IR)
    {
    	ICU.IR[VECT_MTU1_TGIA1].BIT.IR = 0;
    	dummy = ICU.IR[VECT_MTU1_TGIA1].BIT.IR;
    }

    par_encoder_1.TGRA1_data = MTU1.TGRA;    /* Read TGRA capture register */

    par_encoder_1.Contatore_di_overflow = par_encoder_1.Under_over_flow_cnt;
    par_encoder_1.posizione_acquisita = 1;

    /* Disable interrupt TGIA1 */
	/* ---- Timer interrupt enable register_1 (TIER_1) ---- */
	MTU1.TIER.BYTE &= 0xFE;
	/* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
	/* 6  :     =b'0 : reserve        */
	/* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
	/* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
	/* 3-2:     =b'00: reserve        */
	/* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
	/* 0  :TGIEA=b'0 : TGRA Interrupt Disable       */
}

//INTERRUPT USATO PER IL CALCOLO DELLA VELOCITA' (TGRB MTU1)
#pragma interrupt MTU1_TGIB1_isr(vect = VECT_MTU1_TGIB1, enable)
static void MTU1_TGIB1_isr(void)
{
	volatile unsigned char dummy;

	static unsigned char state = 0;

    if(ICU.IR[VECT_MTU1_TGIB1].BIT.IR)
    {
    	ICU.IR[VECT_MTU1_TGIB1].BIT.IR = 0;
    	dummy = ICU.IR[VECT_MTU1_TGIB1].BIT.IR;
    }
	switch(state)
	{
	 case 0:
		 par_encoder_1.TGRB1_data_old = MTU1.TGRB;		/* Read TGRB capture register */
		 state = 1;
		 par_encoder_1.Read_overflow_old = (int)par_encoder_1.Under_over_flow_cnt;
		 par_encoder_1.speed_sample = 0;

		 break;
	 case 1:
		 par_encoder_1.speed_sample = 1;
		 state = 0;
		 par_encoder_1.TGRB1_data_new = MTU1.TGRB;		/* Read TGRB capture register */

		 par_encoder_1.Read_overflow = (int)par_encoder_1.Under_over_flow_cnt;
		 /* Disable interrupt TGIB1 */
		 /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
		 MTU1.TIER.BYTE &= 0xFD;
		/* 7  :  */
		/* 6  :  */
		/* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
		/* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
		/* 3-2:     =b'00: reserve        */
		/* 1  :TGIEB=b'0 : TGRB Interrupt Disable        */
		/* 0  :TGIEA=b'       */
		 break;
	 default:
		state = 0;
		par_encoder_1.speed_sample = 0;
	}
}

//INTERRUPT USATO PER IL CONTEGGIO DEGLI OVERFLOW-UNDERFLOW DI TCNT1 CON SEGNO
#pragma interrupt MTU1_TCIV_TCUV_isr(vect = VECT_ICU_GROUPE1, enable)
void MTU1_TCIV_TCUV_isr(void)
{
    volatile unsigned char dummy;

    do {
    	if(ICU.GRP[GRP_MTU1_TCIV1].BIT.IS_MTU1_TCIV1)
		{
			/* ---- Clearing of TCIV Overflow Flag ---- */
			if(!ICU.GCR[GCR_MTU1_TCIV1].BIT.CLR_MTU1_TCIV1)
			{
				ICU.GCR[GCR_MTU1_TCIV1].BIT.CLR_MTU1_TCIV1 = 1;
				dummy = ICU.GCR[GCR_MTU1_TCIV1].BIT.CLR_MTU1_TCIV1;
			}
			par_encoder_1.Under_over_flow_cnt++;
		}
		if(ICU.GRP[GRP_MTU1_TCIU1].BIT.IS_MTU1_TCIU1)
		{
			/* ---- Clearing of TCIU Underflow Flag ---- */
			if(!ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1)
			{
				ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1 = 1;
				dummy = ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1;
			}
			par_encoder_1.Under_over_flow_cnt--;
		}
    }while(ICU.IR[VECT_ICU_GROUPE1].BIT.IR == 1);
}

/*
 * QUESTA FUNZIONE DI INTERRUPT PER ORA NON LA USO:
 * SERVE PER DARE LA VELOCITA' ISTANTANEA DELL'ALBERO;
 * PER LA VELOCITA' PRELEVO INVECE LA CATTURA SU TGRB della MTU1
 */
#pragma interrupt MTU0_TGIB0_isr(vect = VECT_MTU0_TGIB0, enable)
static void MTU0_TGIB0_isr(void)
{
    volatile unsigned char dummy;
    signed long data;

    /* ---- Clearing of TGIB interrupt flag ---- */
    if(ICU.IR[VECT_MTU0_TGIB0].BIT.IR)
    {
    	ICU.IR[VECT_MTU0_TGIB0].BIT.IR = 0;
    	dummy = ICU.IR[VECT_MTU0_TGIB0].BIT.IR;
    }

    data = MTU0.TGRB - MTU0.TGRD;
        /* Read TGRB capture register */
        /* Read TGRD buffer register  */

   if ( data < 0){
	   par_encoder_1.TGRD0_B0_data_diff = data + (CH0_TGRC_CYCLE+1);	/* set data */
   }else{
	   par_encoder_1.TGRD0_B0_data_diff = data;					/* set data */
   }
}


/* encoder 2 */

/* Implementazione degli interrupt per la gestione della lettura dell'encoder 2 */
#pragma interrupt TPU1_TGIA1_isr(vect = VECT_TPU1_TGI1A, enable)
static void TPU1_TGIA1_isr(void)
{
    volatile unsigned char dummy;

    if(TPU1.TSR.BIT.TGFA)
    {
    	TPU1.TSR.BIT.TGFA = 0;
    	dummy = TPU1.TSR.BIT.TGFA;
    }

    if(ICU.IR[VECT_TPU1_TGI1A].BIT.IR)
    {
    	ICU.IR[VECT_TPU1_TGI1A].BIT.IR = 0;
    	dummy = ICU.IR[VECT_TPU1_TGI1A].BIT.IR;
    }

    par_encoder_2.TGRA1_data = TPU1.TGRA;    /* Read TGRA capture register */

    par_encoder_2.Contatore_di_overflow = par_encoder_2.Under_over_flow_cnt;

    par_encoder_2.posizione_acquisita = 1;

    /* Disable interrupt TGIA1 */
    /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
    TPU1.TIER.BYTE &= 0xFE;
	/* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
	/* 6  :     =b'0 : reserve        */
	/* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
	/* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
	/* 3-2:     =b'00: reserve        */
	/* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
	/* 0  :TGIEA=b'0 : TGRA Interrupt Disable       */
}

#pragma interrupt TPU1_TGIB1_isr(vect = VECT_TPU1_TGI1B, enable)
static void TPU1_TGIB1_isr(void)
{
	volatile unsigned char dummy;

	static unsigned char state = 0;

    if(TPU1.TSR.BIT.TGFB)
    {
    	TPU1.TSR.BIT.TGFB = 0;
    	dummy = TPU1.TSR.BIT.TGFB;
    }

    if(ICU.IR[VECT_TPU1_TGI1B].BIT.IR)
    {
    	ICU.IR[VECT_TPU1_TGI1B].BIT.IR = 0;
    	dummy = ICU.IR[VECT_TPU1_TGI1B].BIT.IR;
    }

	switch(state)
	{
	 case 0:
		 par_encoder_2.TGRB1_data_old = TPU1.TGRB;		/* Read TGRB capture register */
		 state = 1;
		 par_encoder_2.Read_overflow_old = (int)par_encoder_2.Under_over_flow_cnt;
		 par_encoder_2.speed_sample = 0;
		 break;
	 case 1:
		 par_encoder_2.speed_sample = 1;
		 state = 0;
		 par_encoder_2.TGRB1_data_new = TPU1.TGRB;		/* Read TGRB capture register */

		 par_encoder_2.Read_overflow = (int)par_encoder_2.Under_over_flow_cnt;

		 /* Disable interrupt TGIB1 */
		 /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
		 TPU1.TIER.BYTE &= 0xFD;
				/* 7  :  */
				/* 6  :  */
				/* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
				/* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
				/* 3-2:     =b'00: reserve        */
				/* 1  :TGIEB=b'0 : TGRB Interrupt Disable        */
				/* 0  :TGIEA=b'       */
		 break;
	 default:
		state = 0;
		par_encoder_2.speed_sample = 0;
	}
}

#pragma interrupt TPU1_TCIV_TCUV_isr(vect = VECT_ICU_GROUPE3, enable)
void TPU1_TCIV_TCUV_isr(void)
{
    volatile unsigned char dummy;

    do {
		//ICU.GRP[ GRP ## x ].BIT.IS ## x
		//GRP_TPU0_TCI0V=1,GRP_TPU1_TCI1V=1,GRP_TPU1_TCI1U=1,
		if(ICU.GRP[GRP_TPU1_TCI1V].BIT.IS_TPU1_TCI1V)
		{
			/* ---- Clearing of TCIV Overflow Flag ---- */
			if(!ICU.GCR[GCR_TPU1_TCI1V].BIT.CLR_TPU1_TCI1V)
			{
				ICU.GCR[GCR_TPU1_TCI1V].BIT.CLR_TPU1_TCI1V = 1;
				dummy = ICU.GCR[GCR_TPU1_TCI1V].BIT.CLR_TPU1_TCI1V;
			}
	        if(TPU1.TSR.BIT.TCFV)
	        {
	        	TPU1.TSR.BIT.TCFV = 0;
	        	dummy = TPU1.TSR.BIT.TCFV;
	        }
	        par_encoder_2.Under_over_flow_cnt++;
		}

		if(ICU.GRP[GRP_TPU1_TCI1U].BIT.IS_TPU1_TCI1U)
		{
			/* ---- Clearing of TCIU Underflow Flag ---- */
			if(!ICU.GCR[GCR_TPU1_TCI1U].BIT.CLR_TPU1_TCI1U)
			{
				ICU.GCR[GCR_TPU1_TCI1U].BIT.CLR_TPU1_TCI1U = 1;
				dummy = ICU.GCR[GCR_TPU1_TCI1U].BIT.CLR_TPU1_TCI1U;
			}
	        if(TPU1.TSR.BIT.TCFU)
	        {
	        	TPU1.TSR.BIT.TCFU = 0;
	        	dummy = TPU1.TSR.BIT.TCFU;
	        }
	        par_encoder_2.Under_over_flow_cnt--;
		}

    }while(ICU.IR[VECT_ICU_GROUPE3].BIT.IR == 1);
}

/*
 * QUESTA FUNZIONE DI INTERRUPT PER ORA NON LA USO:
 * SERVE PER DARE LA VELOCITA' ISTANTANEA DELL'ALBERO;
 * PER LA VELOCITA' PRELEVO INVECE LA CATTURA SU TGRB della TPU1
 * */
#pragma interrupt TPU0_TGIB0_isr(vect = VECT_TPU0_TGI0B, enable)
static void TPU0_TGIB0_isr(void)
{
    volatile unsigned char dummy;
    signed long data;

    if(TPU0.TSR.BIT.TGFB)
    {
    	TPU0.TSR.BIT.TGFB = 0;
    	dummy = TPU0.TSR.BIT.TGFB;
    }
    /* ---- Clearing of TGIB interrupt flag ---- */
    if(ICU.IR[VECT_TPU0_TGI0B].BIT.IR)
    {
    	ICU.IR[VECT_TPU0_TGI0B].BIT.IR = 0;
    	dummy = ICU.IR[VECT_TPU0_TGI0B].BIT.IR;
    }

    data = TPU0.TGRB - TPU0.TGRD;
        /* Read TGRB capture register */
        /* Read TGRD buffer register  */

   if ( data < 0){
	   par_encoder_2.TGRD0_B0_data_diff = data + (CH0_TGRC_CYCLE+1);	/* set data */
   }else{
	   par_encoder_2.TGRD0_B0_data_diff = data;					/* set data */
   }
}
