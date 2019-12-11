#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "platform.h"

#include "encoder_MOT1.h"

static void MTU1_TGIA1_isr(void);	  /* ch1 TGRA Input capture Interrupt */
static void MTU1_TGIB1_isr(void);	  /* ch1 TGRB Input capture Interrupt */
static void MTU0_TGIB0_isr(void);	  /* ch0 TGRB Input capture Interrupt */


/* Variabili che mi segnalano se le funzioni di interrupt hanno acquisito
   i dati che mi servono per il calcolo di posizione e velocita' */
unsigned char posizione_acquisita = 0;
unsigned char speed_sample = 0;

/* numero di impulsi catturati ogni ms (velocita' in impulsi al ms) */
signed long speed_data;

/* Numero di impulsi per giro dell'albero (tiene conto anche del motoriduttore) */
unsigned int MODULO_ENC;

/* variabili usate per il calcolo della velocita' */
int Read_overflow_old;
int Read_overflow;

/* numero di gradi per ogni impulso moltiplicato per mille */
float mDEGREE_PER_PULSE;

/* variabile per il calcolo della posizione istantanea */
unsigned short TGRA1_data; 	        /* MTU ch1 TGRA capture data */

/* variabili per il calcolo della velocita' */
unsigned short TGRB1_data_old; 	    /* MTU ch1 TGRB capture data(old) */
unsigned short TGRB1_data_new;		/* MTU ch1 TGRB capture data(new) */

/* Numero complessivo (con segno!) di overflow e di underflow */
signed short int Under_over_flow_cnt; 	/* MTU ch1 underflow/overflow counter */

/* variabile di appoggio con il numero di overflow-underflow */
signed short int Contatore_di_overflow;

/* l'interrupt su TGRB0 e' definito ma non lo uso per ora */
signed long TGRD0_B0_data_diff; 	/* MTU ch0 TGRD,TGRB difference  */

void encoder_1_init(void)
{
	unsigned int dummy;

	Init_Encoder_1_vars();

#ifdef PLATFORM_BOARD_RDKRX63N
SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
#endif

	/* ==== Setting of MTU2  ==== */
	mtu2_init();

    /* ==== Setting of INTC(Interrupt Controller) ==== */

#if 0
#define	__IR( x )		ICU.IR[ IR ## x ].BIT.IR
#define	 _IR( x )		__IR( x )
#define	  IR( x , y )	_IR( _ ## x ## _ ## y )
#define	__DTCE( x )		ICU.DTCER[ DTCE ## x ].BIT.DTCE
#define	 _DTCE( x )		__DTCE( x )
#define	  DTCE( x , y )	_DTCE( _ ## x ## _ ## y )
#define	__IEN( x )		ICU.IER[ IER ## x ].BIT.IEN ## x
#define	 _IEN( x )		__IEN( x )
#define	  IEN( x , y )	_IEN( _ ## x ## _ ## y )
#define	__IPR( x )		ICU.IPR[ IPR ## x ].BIT.IPR
#define	 _IPR( x )		__IPR( x )
#define	  IPR( x , y )	_IPR( _ ## x ## _ ## y )
#define	__VECT( x )		VECT ## x
#define	 _VECT( x )		__VECT( x )
#define	  VECT( x , y )	_VECT( _ ## x ## _ ## y )
#define	__MSTP( x )		MSTP ## x
#define	 _MSTP( x )		__MSTP( x )
#define	  MSTP( x )		_MSTP( _ ## x )

#define	__IS( x )		ICU.GRP[ GRP ## x ].BIT.IS ## x
#define	 _IS( x )		__IS( x )
#define	  IS( x , y )	_IS( _ ## x ## _ ## y )
#define	__EN( x )		ICU.GEN[ GEN ## x ].BIT.EN ## x
#define	 _EN( x )		__EN( x )
#define	  EN( x , y )	_EN( _ ## x ## _ ## y )
#define	__CLR( x )		ICU.GCR[ GCR ## x ].BIT.CLR ## x
#define	 _CLR( x )		__CLR( x )
#define	  CLR( x , y )	_CLR( _ ## x ## _ ## y )
#define	__CN( x )		ICU.SEL.BIT.CN ## x
#define	 _CN( x )		__CN( x )
#define	  CN( x , y )	_CN( _ ## x ## _ ## y )
#endif

#if 1 /* Edge int */
    if(ICU.IR[VECT_MTU1_TGIA1].BIT.IR)
    {
    	ICU.IR[VECT_MTU1_TGIA1].BIT.IR = 0;
    	dummy = ICU.IR[VECT_MTU1_TGIA1].BIT.IR;
    }
    if(ICU.IR[VECT_MTU1_TGIB1].BIT.IR)
    {
    	ICU.IR[VECT_MTU1_TGIB1].BIT.IR = 0;
    	dummy = ICU.IR[VECT_MTU1_TGIB1].BIT.IR;
    }
    if(ICU.IR[VECT_MTU0_TGIB0].BIT.IR)
    {
    	ICU.IR[VECT_MTU0_TGIB0].BIT.IR = 0;
    	dummy = ICU.IR[VECT_MTU0_TGIB0].BIT.IR;
    }
#endif

    //ICU.GCR[ GCR ## x ].BIT.CLR ## x
    if(ICU.GRP[GRP_MTU1_TCIV1].BIT.IS_MTU1_TCIV1)
    {
    	ICU.GCR[GCR_MTU1_TCIV1].BIT.CLR_MTU1_TCIV1 = 1;
    	dummy = ICU.GCR[GCR_MTU1_TCIV1].BIT.CLR_MTU1_TCIV1;
    }
    if(ICU.GRP[GRP_MTU1_TCIU1].BIT.IS_MTU1_TCIU1)
    {
    	ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1 = 1;
    	dummy = ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1;
    }

#if 0 /* Level int */
    /* group 1 interrupt */
    if(ICU.IR[VECT_ICU_GROUPE1].BIT.IR)
    {
    	ICU.IR[VECT_ICU_GROUPE1].BIT.IR = 0;
    	dummy = ICU.IR[VECT_ICU_GROUPE1].BIT.IR;
    }
#endif

    /* Set interrupt priority.  */
    //IPR(MTU, ) = 0xA;...
    if(!ICU.IPR[IPR_MTU1_TGIA1].BIT.IPR)
    {
    	ICU.IPR[IPR_MTU1_TGIA1].BIT.IPR = 0xC;
    	dummy = ICU.IPR[IPR_MTU1_TGIA1].BIT.IPR;
    }
    if(!ICU.IPR[IPR_MTU1_TGIB1].BIT.IPR)
    {
    	ICU.IPR[IPR_MTU1_TGIB1].BIT.IPR = 0xC;
    	dummy = ICU.IPR[IPR_MTU1_TGIB1].BIT.IPR;
    }
    if(!ICU.IPR[IPR_MTU0_TGIB0].BIT.IPR)
    {
    	ICU.IPR[IPR_MTU0_TGIB0].BIT.IPR = 0x0;
    	dummy = ICU.IPR[IPR_MTU0_TGIB0].BIT.IPR;
    }
    if(!ICU.IPR[IPR_ICU_GROUPE1].BIT.IPR)
    {
    	ICU.IPR[IPR_ICU_GROUPE1].BIT.IPR = 0xD;
    	dummy = ICU.IPR[IPR_ICU_GROUPE1].BIT.IPR;
    }


    /* Enable interrupt sources */
    //IEN(MTU, ) = 0;
    ICU.IER[IER_MTU1_TGIA1].BIT.IEN_MTU1_TGIA1   = 1;
    ICU.IER[IER_MTU1_TGIB1].BIT.IEN_MTU1_TGIB1   = 1;
    ICU.IER[IER_MTU0_TGIB0].BIT.IEN_MTU0_TGIB0   = 1;
    /* Enable group 1 interrupt */
    ICU.IER[IER_ICU_GROUPE1].BIT.IEN_ICU_GROUPE1 = 1;
    /* Enable source interrupt in the group */
    //ICU.GEN[ GEN ## x ].BIT.EN ## x
    ICU.GEN[GEN_MTU1_TCIV1].BIT.EN_MTU1_TCIV1 = 1;
    ICU.GEN[GEN_MTU1_TCIU1].BIT.EN_MTU1_TCIU1 = 1;

    /* ==== Setting of PFC(Pin Function Controller) ==== */
    pfc_init();

    /* ==== MTU2 timer start register(TSTR) ==== */
    MTU.TSTR.BYTE |= 0x03;      /* TCNT_0,1 performs count operation */

}

#if 0
void MC_StartEnc(void)
/***************************************************************************
*	Description:	timer 1 (2) settings (phase counting mode 1) for encoder
*					input reading, timer start
***************************************************************************/
{

	// MTU settings (Timer 2: TCLKC, TCLKD)
	MTU.TSTR.BIT.CST1=0;		// timer 1 stop counting
	MTU1.TCNT=0;				// reset counter value
	//MTU1.TSR.BIT.TCFU=0;		// reset underflow flag (only 0 can be written)
	//MTU1.TSR.BIT.TCFV=0;		// reset overflow flag (only 0 can be written)
	MTU1.TCR.BIT.CCLR=0;		// select no clearing source
	MTU1.TMDR.BIT.MD=4;			// set phase counting mode 2

	// start counting
	MTU.TSTR.BIT.CST1=1;		// timer 2 start counting
}
#endif

void mtu2_init(void)
{

	/* ==== Setting of Power-Down Modes ==== */
	/* Startup channels 0 and 1 of mtu2 */
	MSTP(MTU0) = 0;
	MSTP(MTU1) = 0;

	/* ==== Setting of MTU2  ==== */

    /* ---- reset MTU2 Timer Start Register (TSTR) ---- */
    MTU.TSTR.BYTE &= 0xFC;      /* CH0-1 count operation is stopped */

    /* ==== Setting of MTU2( CH1 )  ==== */

    /* ---- Timer control register_1(TCR_1) ---- */
    MTU1.TCR.BYTE = 0x04; /* TCNT clearing disabled
    						-External clock MTCLKA*/

    /* ---- Timer counter_1(TCNT_1) ---- */
    MTU1.TCNT = 0x0000;        /* Clear the timer counter */

    /* ---- Timer general register_1 ---- */
    MTU1.TGRA = 0x0000;        /* Clear input capture register */
    MTU1.TGRB = 0x0000;        /* Clear input capture register */

    /* ---- Timer mode register_1 (TMDR_1) ---- */
	/* 7-4:       =b'0000: reserve        */
	/* 3-0:MD[3:0]=b'0100: Phase counting mode 1    */
    MTU1.TMDR.BYTE = 0x04;

    /* ---- Timer I/O control register_1 (TIOR_1) ---- */
    /* 7-4 IOB[3:0]=b'1100 :Input capture at generation of
    						TGRC_0 compare match */
    /* 3-0 IOA[3:0]=b'1100 :Input capture at generation of
    						TGRA_0 compare match */
    MTU1.TIOR.BYTE = 0xcc;

    /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
    /* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
    /* 6  :     =b'0 : reserve        */
    /* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
    /* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
    /* 3-2:     =b'00: reserve        */
    /* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
    /* 0  :TGIEA=b'1 : TGRA Interrupt Enable        */
    MTU1.TIER.BYTE = 0x33;

    /** ==== Setting of MTU2( CH0 )  ==== */

   /* ---- Timer control register_0(TCR_0) ---- */
    /* 7-5:CCLR[2:0]=b'101: TCNT cleared by TGRC compare match */
    /* 4-3:CKEG[1:0]=b'00 : Count at rising edge */
    /* 2-0:TPSC[2:0]=b'000: Internal clock: counts on Pclock/1 */
	MTU0.TCR.BYTE = 0xA0;

	/* ---- Timer counter_0(TCNT_0) ---- */
    MTU0.TCNT = 0x0000;        /* Clear the timer counter */

    /* ---- Timer general register_0 ---- */
    MTU0.TGRA = CH0_TGRA_CYCLE; /* Set compare match register. Set speed cycle */
    MTU0.TGRC = CH0_TGRC_CYCLE; /* Set compare match register. Set position cycle   */
    MTU0.TGRB = 0x0000;         /* Clear Input capture register */
    MTU0.TGRD = 0x0000;         /* Clear TGRB buffer register   */

    /* ---- Timer mode register_0 (TMDR_0) ---- */
    /* 7  :   =b'0   : reserve        */
    /* 6  :BFE=b'0   : TGRE_0 and TGRF_0 operate normally */
    /* 5  :BFB=b'1   : TGRB and TGRD buffer operation */
    /* 4  :BFA=b'0   : TGRA and TGRC  operate normally */
    /* 3-0:MD[3:0]=b'0000: Normal operation    */
    MTU0.TMDR.BYTE = 0x20;

    /* ---- Timer I/O control register_0 (TIORH_0,TIORL_0) ---- */
    /* 7-4 IOB[3:0]=b'1100 : Capture input source is channel 1 count clock.
     * 						 Input capture at TCNT_1 count-up/count-down.  */
    /* 3-0 IOA[3:0]=b'0000 :Output retained */
    MTU0.TIORH.BYTE = 0xC0;

    /* 7-4 IOD[3:0]=b'0000 :Output retained */
    /* 3-0 IOC[3:0]=b'0000 :Output retained */
    MTU0.TIORL.BYTE = 0x00;


    /* ---- Timer interrupt enable register_0 (TIER_0) ---- */
    /* 7  : TTGE=b'0 : A/D  Start Request Disabled */
    /* 6-5:     =b'0 : reserve */
    /* 4  :TCIEV=b'0 : Overflow Interrupt Disable   */
    /* 3  :TGIED=b'0 : TGID Interrupt Disable      */
    /* 2  :TGIEC=b'0 : TGIC Interrupt Disable      */
    /* 1  :TGIEB=b'1 : TGIB Interrupt Enable       */
    /* 0  :TGIEA=b'0 : TGIA Interrupt Disable      */
    MTU0.TIER.BYTE = 0x02;

}

void pfc_init(void)
{
#if 1
    /* porta P24, pin 25 conn. JN2 */
	MPC.P24PFS.BYTE  = 0x02; /* 1 defines P24 to be MTCLKA, with no IRQ */

    PORT2.PDR.BIT.B4 = 0;    /* Set P24 as input */

    PORT2.PMR.BIT.B4 = 1;    /* Set P24 as peripheral function bit */

    /* porta PA6, pin 6 conn. JN2 */
    MPC.PA6PFS.BYTE  = 0x02; /* 1 defines PA6 to be MTCLKB, with no IRQ */

    PORTA.PDR.BIT.B6 = 0;    /* Set PA6 as input */

    PORTA.PMR.BIT.B6 = 1;    /* Set PA6 as peripheral function bit */
#endif

}

void Query_Enc_1(encoder_data_t_mt_1* enc)
{
 static unsigned char state = INIT_STATE_ENC;

 switch(state)
 {
  case INIT_STATE_ENC:
	   posizione_acquisita = 0;
	   speed_sample = 0;

	   /* Enable interrupt TGIA1 and TGIB1 */
	   /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
	   MTU1.TIER.BYTE = 0x33;
	   /* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
	   /* 6  :     =b'0 : reserve        */
	   /* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
	   /* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
	   /* 3-2:     =b'00: reserve        */
	   /* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
	   /* 0  :TGIEA=b'1 : TGRA Interrupt Enable        */
	   state = READ_STATE_ENC;
	   break;

  case READ_STATE_ENC:
	   if(posizione_acquisita == 1 && speed_sample == 1)
		{
			posizione_acquisita = 0;
			speed_sample = 0;

			/* calcolo posizione e velocita' in base ai valori acquisiti */

			/* Calcolo la posizione */
		    if(Contatore_di_overflow < 0)
		     {
		      /* absolute position */
		      enc->position = -(( ((0xFFFF + 1) * (Contatore_di_overflow + 1)) + (0xFFFF - TGRA1_data + 1) ) % MODULO_ENC);
		     } else
		     	 {
		    	 /* absolute position */
		    	  enc->position = ((((0xFFFF + 1) * Contatore_di_overflow) + TGRA1_data) % MODULO_ENC);
		     	 }

		     /* Calcolo la velocita' */
			 /* non c'e' stato overflow ne' underflow (oppure c'e' stata un'oscillazione) */
			 if(Read_overflow == Read_overflow_old)
			 {
				 speed_data = TGRB1_data_new - TGRB1_data_old; 	/* new - old  */
				 if((speed_data <= 0 && (MTU1.TSR.BIT.TCFD == 1)) || (speed_data >= 0 && (MTU1.TSR.BIT.TCFD == 0)))
				 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
				 else enc->oscillation = 0;
			 }else if(Read_overflow > Read_overflow_old) /* overflow */
			 	 	 {
				 	 	 speed_data = TGRB1_data_new + (0xFFFF - TGRB1_data_old + 1); 	/* new - old  */
						 if(speed_data >= 0 && (MTU1.TSR.BIT.TCFD == 0))
						 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
						 else enc->oscillation = 0;
			 	 	 } else if(Read_overflow < Read_overflow_old) /* underflow */
			 	 	 	 	 {
			 	 		 	 	 speed_data = -((0xFFFF - TGRB1_data_new + 1) + TGRB1_data_old); 	/* new - old  */
								 if(speed_data <= 0 && (MTU1.TSR.BIT.TCFD == 1))
								 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
								 else enc->oscillation = 0;
			 	 	 	 	 }

			/* velocita' in impulsi al ms */
			enc->speed = (int)speed_data;

			/* posizione dell'albero in gradi */
			enc->position_in_degree = ((mDEGREE_PER_PULSE * enc->position) / 1000);

			/* Posizione in pi rad 								*/
			/* 180 : pi = deg : rad   =>   rad = pi * (deg/180) */
			enc->position_in_pigreek_radians_units = enc->position_in_degree/180;

			/* velocita' in gradi al secondo */
			enc->speed_in_degree_per_sec = (mDEGREE_PER_PULSE * enc->speed);

			/* velocita' in radianti al secondo */
			enc->speed_in_rad_per_sec = (PI_GREEK * enc->speed_in_degree_per_sec) / 180;

			/* posizione in radianti al secondo */
			enc->position_radiant = (enc->position_in_degree * PI_GREEK)/180;

			state = INIT_STATE_ENC;
		}
	  break;
 }
}

void Media_Speed_Enc_1(encoder_data_t_mt_1* enc)
{
	static int speed_vect[100] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static unsigned char queue_index = 0;
	unsigned int i;
	float velocita_media_1;

	velocita_media_1 = 0.0;

	speed_vect[queue_index] = enc->speed;
	queue_index++;
	if(queue_index >= 100) queue_index = 0;

	for(i = 0; i < 100; i++)
	{
		velocita_media_1 += (float)speed_vect[i];
	}

	velocita_media_1 = velocita_media_1 / 100.0;

	/* velocita' in gradi al secondo */
	enc->speed_in_degree_per_sec = (mDEGREE_PER_PULSE * velocita_media_1 );

	/* velocita' in radianti al secondo */
	enc->speed_in_rad_per_sec = (PI_GREEK * enc->speed_in_degree_per_sec) / 180;


}

void Read_Enc_1(encoder_data_t_mt_1* enc)
{
	/* stampo i valori acquisiti */

	/* Declare display buffer */
	uint8_t  lcd_buffer[20];

	/* Display message on LCD */
	lcd_display(LCD_LINE1, "Enc data:");

	sprintf((char *)lcd_buffer, "r/s= %.3f" , enc->speed_in_rad_per_sec);
	lcd_display(LCD_LINE2, lcd_buffer);

}

void Init_Encoder_1_vars(void)
{
	/* Numero di impulsi per giro dell'albero (con motoriduttore) */
	MODULO_ENC = (ENCODER_PPR*REDUCTION_GAIN*PHASE_COUNT_MODE);

	/* gradi per impulso per 1000 */
	if(MODULO_ENC)
		mDEGREE_PER_PULSE = (float)360000/(float)MODULO_ENC;

	/* resetto le variabili usate per i calcoli di posizione e velocita' */
	posizione_acquisita = 0;
	speed_sample = 0;
	Under_over_flow_cnt = 0;
	Read_overflow_old = 0;
	Read_overflow = 0;
}


//INTERRUPT USATO PER IL CALCOLO DELLA POSIZIONE (TGRA MTU1)

#pragma interrupt MTU1_TGIA1_isr(vect = VECT_MTU1_TGIA1, enable)
static void MTU1_TGIA1_isr(void)
{
    volatile unsigned char dummy;

    if(ICU.IR[VECT_MTU1_TGIA1].BIT.IR)
    {
    	ICU.IR[VECT_MTU1_TGIA1].BIT.IR = 0;
    	dummy = ICU.IR[VECT_MTU1_TGIA1].BIT.IR;
    }

    TGRA1_data = MTU1.TGRA;    /* Read TGRA capture register */

    Contatore_di_overflow = Under_over_flow_cnt;

#if 0
    if(Under_over_flow_cnt < 0)
     {
      /* absolute position */
      enc.position = -(( (0xFFFF * (Under_over_flow_cnt + 1)) + (0xFFFF - TGRA1_data + 1) ) % MODULO_ENC);
     } else
     	 {
    	 /* absolute position */
    	  enc.position = (((0xFFFF * Under_over_flow_cnt) + TGRA1_data) % MODULO_ENC);
     	 }
#endif

    posizione_acquisita = 1;

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

#if 0
	new_data = MTU1.TGRB;		/* Read TGRB capture register */

	data = new_data - TGRB1_data_old; 	/* new - old  */

	if( data<0 ){
		TGRB1_data_diff = data + ENC_1_MAX;	/* set data */
	}else{
		TGRB1_data_diff = data;				/* set data */
	}

	TGRB1_data_old = new_data;				/* set old data */
#endif

	switch(state)
	{
	 case 0:
		 TGRB1_data_old = MTU1.TGRB;		/* Read TGRB capture register */
		 state = 1;
		 Read_overflow_old = (int)Under_over_flow_cnt;
		 speed_sample = 0;

		 break;
	 case 1:
		 speed_sample = 1;
		 state = 0;
		 TGRB1_data_new = MTU1.TGRB;		/* Read TGRB capture register */

		 Read_overflow = (int)Under_over_flow_cnt;

#if 0
		 /* non c'e' stato overflow ne' underflow (oppure c'e' stata un'oscillazione) */
		 if(Read_overflow == Read_overflow_old)
		 {
			 data = new_data - TGRB1_data_old; 	/* new - old  */
			 if((data <= 0 && (MTU1.TSR.BIT.TCFD == 1)) || (data >= 0 && (MTU1.TSR.BIT.TCFD == 0)))
			 { enc.oscillazione = 1;} /* segnalo che e' una oscillazione */
			 else enc.oscillazione = 0;
		 }else if(Read_overflow > Read_overflow_old) /* overflow */
		 	 	 {
			 	 	 data = new_data + (0xFFFF - TGRB1_data_old + 1); 	/* new - old  */
					 if(data >= 0 && (MTU1.TSR.BIT.TCFD == 0))
					 { enc.oscillazione = 1;} /* segnalo che e' una oscillazione */
					 else enc.oscillazione = 0;
		 	 	 } else if(Read_overflow < Read_overflow_old) /* underflow */
		 	 	 	 	 {
							 data = -((0xFFFF - new_data + 1) + TGRB1_data_old); 	/* new - old  */
							 if(data <= 0 && (MTU1.TSR.BIT.TCFD == 1))
							 { enc.oscillazione = 1;} /* segnalo che e' una oscillazione */
							 else enc.oscillazione = 0;
		 	 	 	 	 }
#endif

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
		speed_sample = 0;
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
			Under_over_flow_cnt++;
		}
		if(ICU.GRP[GRP_MTU1_TCIU1].BIT.IS_MTU1_TCIU1)
		{
			/* ---- Clearing of TCIU Underflow Flag ---- */
			if(!ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1)
			{
				ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1 = 1;
				dummy = ICU.GCR[GCR_MTU1_TCIU1].BIT.CLR_MTU1_TCIU1;
			}
			Under_over_flow_cnt--;
		}
    }while(ICU.IR[VECT_ICU_GROUPE1].BIT.IR == 1);
}

/* QUESTA FUNZIONE DI INTERRUPT PER ORA NON LA USO:
   SERVE PER DARE LA VELOCITA' ISTANTANEA DELL'ALBERO;
   PER LA VELOCITA' PRELEVO INVECE LA CATTURA SU TGRB della MTU1 */

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
       TGRD0_B0_data_diff = data + (CH0_TGRC_CYCLE+1);	/* set data */
   }else{
       TGRD0_B0_data_diff = data;					/* set data */
   }

}
/* End of File */
