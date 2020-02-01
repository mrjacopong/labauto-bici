/*""FILE COMMENT""********** Technical reference data *************************
*
*       System Name : MTU2 Sample Program for Encoder
*       File Name   : encoder_1.c
*       Abstract    : Sample program of MTU2 phase count mode 1
*       Version     :
*       Device      : RX63N
*       Tool-Chain  : C/C++ compiler package for the RX Family
*       H/W Platform: YRDKRX63N
*
*""FILE COMMENT END""*********************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "platform.h"
#include "encoder.h"

/* Inizializzazione dell'encoder attraverso il settaggio di registri,
 *  ognuna delle init contiene due chiamate di funzione,una all'inizio
 *  per il settaggio dei registri di acquisizione delle fasi e settaggio
 *  dei timer(mtu_init,tpu_init,tpu_init_), la seconda chiamata � per il settaggio
 *  delle porte per ogni encoder per la lettura di FASE A e FASE B
 *  (pin_enc1_init,pin_enc2_init,pin_enc3_init, le parti centrali della
 *  inizializzazione riguardano la gestione degli interrupt */

extern parametri_inr_config par_encoder_1;
extern parametri_inr_config par_encoder_2;

void encoder_init_1(void)
{
	unsigned int dummy;


#ifdef PLATFORM_BOARD_RDKRX63N
SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
#endif

	/* ==== Setting of MTU2  ==== */
	mtu2_init();

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
    pin_enc1_init();

    /* ==== MTU2 timer start register(TSTR) ==== */
    MTU.TSTR.BYTE |= 0x03;      /* TCNT_0,1 performs count operation */

}

void encoder_init_2(){
	unsigned int dummy;

	#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	#endif

		/* ==== Setting of TPU  ==== */
		tpu_init();

	#if 1 /* Edge int */
	    if(ICU.IR[VECT_TPU1_TGI1A].BIT.IR)
	    {
	    	ICU.IR[VECT_TPU1_TGI1A].BIT.IR = 0;
	    	dummy = ICU.IR[VECT_TPU1_TGI1A].BIT.IR;
	    }
	    if(ICU.IR[VECT_TPU1_TGI1B].BIT.IR)
	    {
	    	ICU.IR[VECT_TPU1_TGI1B].BIT.IR = 0;
	    	dummy = ICU.IR[VECT_TPU1_TGI1B].BIT.IR;
	    }
	    if(ICU.IR[VECT_TPU0_TGI0B].BIT.IR)
	    {
	    	ICU.IR[VECT_TPU0_TGI0B].BIT.IR = 0;
	    	dummy = ICU.IR[VECT_TPU0_TGI0B].BIT.IR;
	    }
	#endif

	    //ICU.GCR[ GCR ## x ].BIT.CLR ## x
	    if(ICU.GRP[GRP_TPU1_TCI1V].BIT.IS_TPU1_TCI1V)
	    {
	    	ICU.GCR[GCR_TPU1_TCI1V].BIT.CLR_TPU1_TCI1V = 1;
	    	dummy = ICU.GCR[GCR_TPU1_TCI1V].BIT.CLR_TPU1_TCI1V;
	    }
	    if(ICU.GRP[GRP_TPU1_TCI1U].BIT.IS_TPU1_TCI1U)
	    {
	    	ICU.GCR[GCR_TPU1_TCI1U].BIT.CLR_TPU1_TCI1U = 1;
	    	dummy = ICU.GCR[GCR_TPU1_TCI1U].BIT.CLR_TPU1_TCI1U;
	    }

	    //IPR(TPU, ) = 0xA;...
	    if(!ICU.IPR[IPR_TPU1_TGI1A].BIT.IPR)
	    {
	    	ICU.IPR[IPR_TPU1_TGI1A].BIT.IPR = 0xC;
	    	dummy = ICU.IPR[IPR_TPU1_TGI1A].BIT.IPR;
	    }
	    if(!ICU.IPR[IPR_TPU1_TGI1B].BIT.IPR)
	    {
	    	ICU.IPR[IPR_TPU1_TGI1B].BIT.IPR = 0xC;
	    	dummy = ICU.IPR[IPR_TPU1_TGI1B].BIT.IPR;
	    }
	    if(!ICU.IPR[IPR_TPU0_TGI0B].BIT.IPR)
	    {
	    	ICU.IPR[IPR_TPU0_TGI0B].BIT.IPR = 0x0;
	    	dummy = ICU.IPR[IPR_TPU0_TGI0B].BIT.IPR;
	    }
	    if(!ICU.IPR[IPR_ICU_GROUPE3].BIT.IPR)
	    {
	    	ICU.IPR[IPR_ICU_GROUPE3].BIT.IPR = 0xD;
	    	dummy = ICU.IPR[IPR_ICU_GROUPE3].BIT.IPR;
	    }

	    /* Enable interrupt sources */
	    //IEN(TPU, ) = 0;
	    ICU.IER[IER_TPU1_TGI1A].BIT.IEN_TPU1_TGI1A   = 1;
	    ICU.IER[IER_TPU1_TGI1B].BIT.IEN_TPU1_TGI1B   = 1;
	    ICU.IER[IER_TPU0_TGI0B].BIT.IEN_TPU0_TGI0B   = 1;
	    /* Enable group 3 interrupt */
	    ICU.IER[IER_ICU_GROUPE3].BIT.IEN_ICU_GROUPE3 = 1;
	    /* Enable source interrupt in the group */
	    //ICU.GEN[ GEN ## x ].BIT.EN ## x
	    ICU.GEN[GEN_TPU1_TCI1V].BIT.EN_TPU1_TCI1V = 1;
	    ICU.GEN[GEN_TPU1_TCI1U].BIT.EN_TPU1_TCI1U = 1;

	    /* ==== Setting of PFC(Pin Function Controller) ==== */
	    pin_enc2_init();

	    /* ==== TPUA timer start register(TSTR) ==== */
	    TPUA.TSTR.BYTE |= 0x03;      /* TCNT_0,1 performs count operation */
}

void encoder_init_3(){
	unsigned int dummy;

	#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	#endif

		/* ==== Setting of TPU  ==== */
		tpu_init_();

	    /* ==== Setting of INTC(Interrupt Controller) ==== */

	#if 1 /* Edge int */
	    if(ICU.IR[VECT_TPU4_TGI4A].BIT.IR)
	    {
	    	ICU.IR[VECT_TPU4_TGI4A].BIT.IR = 0;
	    	dummy = ICU.IR[VECT_TPU4_TGI4A].BIT.IR;
	    }
	    if(ICU.IR[VECT_TPU4_TGI4B].BIT.IR)
	    {
	    	ICU.IR[VECT_TPU4_TGI4B].BIT.IR = 0;
	    	dummy = ICU.IR[VECT_TPU4_TGI4B].BIT.IR;
	    }
	    if(ICU.IR[VECT_TPU3_TGI3B].BIT.IR)
	    {
	    	ICU.IR[VECT_TPU3_TGI3B].BIT.IR = 0;
	    	dummy = ICU.IR[VECT_TPU3_TGI3B].BIT.IR;
	    }
	#endif

	    //ICU.GCR[ GCR ## x ].BIT.CLR ## x
	    if(ICU.GRP[GRP_TPU4_TCI4V].BIT.IS_TPU4_TCI4V)
	    {
	    	ICU.GCR[GCR_TPU4_TCI4V].BIT.CLR_TPU4_TCI4V = 1;
	    	dummy = ICU.GCR[GCR_TPU4_TCI4V].BIT.CLR_TPU4_TCI4V;
	    }
	    if(ICU.GRP[GRP_TPU4_TCI4U].BIT.IS_TPU4_TCI4U)
	    {
	    	ICU.GCR[GCR_TPU4_TCI4U].BIT.CLR_TPU4_TCI4U = 1;
	    	dummy = ICU.GCR[GCR_TPU4_TCI4U].BIT.CLR_TPU4_TCI4U;
	    }


	    /* Set interrupt priority.  */
	    //IPR(TPU, ) = 0xA;...
	    if(!ICU.IPR[IPR_TPU4_TGI4A].BIT.IPR)
	    {
	    	ICU.IPR[IPR_TPU4_TGI4A].BIT.IPR = 0xC;
	    	dummy = ICU.IPR[IPR_TPU4_TGI4A].BIT.IPR;
	    }
	    if(!ICU.IPR[IPR_TPU4_TGI4B].BIT.IPR)
	    {
	    	ICU.IPR[IPR_TPU4_TGI4B].BIT.IPR = 0xC;
	    	dummy = ICU.IPR[IPR_TPU4_TGI4B].BIT.IPR;
	    }
	    if(!ICU.IPR[IPR_TPU3_TGI3B].BIT.IPR)
	    {
	    	ICU.IPR[IPR_TPU3_TGI3B].BIT.IPR = 0x0;
	    	dummy = ICU.IPR[IPR_TPU3_TGI3B].BIT.IPR;
	    }
	    if(!ICU.IPR[IPR_ICU_GROUPE4].BIT.IPR)
	    {
	    	ICU.IPR[IPR_ICU_GROUPE4].BIT.IPR = 0xD;
	    	dummy = ICU.IPR[IPR_ICU_GROUPE4].BIT.IPR;
	    }

	    /* Select interrupt sources TPU3-TPU4 */
	      //ICU.SEL.BIT.CN ## x
		  //ICU.SEL.BIT.CN_TPU4_TGI4A = 0;
		  //ICU.SEL.BIT.CN_TPU4_TGI4B = 0;
		  //ICU.SEL.BIT.CN_TPU3_TGI3B = 0;

	    /* Enable interrupt sources */
	    //IEN(TPU, ) = 0;
	    ICU.IER[IER_TPU4_TGI4A].BIT.IEN_TPU4_TGI4A   = 1;
	    ICU.IER[IER_TPU4_TGI4B].BIT.IEN_TPU4_TGI4B   = 1;
	    ICU.IER[IER_TPU3_TGI3B].BIT.IEN_TPU3_TGI3B   = 1;
	    /* Enable group 4 interrupt */
	    ICU.IER[IER_ICU_GROUPE4].BIT.IEN_ICU_GROUPE4 = 1;
	    /* Enable source interrupt in the group */
	    //ICU.GEN[ GEN ## x ].BIT.EN ## x
	    ICU.GEN[GEN_TPU4_TCI4V].BIT.EN_TPU4_TCI4V = 1;
	    ICU.GEN[GEN_TPU4_TCI4U].BIT.EN_TPU4_TCI4U = 1;

	    /* ==== Setting of PFC(Pin Function Controller) ==== */
	    pin_enc3_init();

	    /* ==== TPUB timer start register(TSTR) ==== */
	    TPUA.TSTR.BYTE |= 0x18;      /* TCNT_3,4 performs count operation */
}

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
    //MTU1.TCR.BYTE = 0x20;
    /* 7-5:CCLR[2:0]=b'001: TCNT cleared by TGRA input capture */
	/* 4-3:CKEG[1:0]=b'00 : This setting is ignored
							in phase counting mode */
	/* 2-0:TPSC[2:0]=b'000: This setting is ignored
							in phase counting mode */
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
    MTU1.TIOR.BYTE = 0xcc;
	/* 7-4 IOB[3:0]=b'1100 :Input capture at generation of
							TGRC_0 compare match */
	/* 3-0 IOA[3:0]=b'1100 :Input capture at generation of
							TGRA_0 compare match */

    /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
    MTU1.TIER.BYTE = 0x33;
	/* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
	/* 6  :     =b'0 : reserve        */
	/* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
	/* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
	/* 3-2:     =b'00: reserve        */
	/* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
	/* 0  :TGIEA=b'1 : TGRA Interrupt Enable        */

    /** ==== Setting of MTU2( CH0 )  ==== */

   /* ---- Timer control register_0(TCR_0) ---- */
	MTU0.TCR.BYTE = 0xA0;
	/* 7-5:CCLR[2:0]=b'101: TCNT cleared by TGRC compare match */
	/* 4-3:CKEG[1:0]=b'00 : Count at rising edge */
	/* 2-0:TPSC[2:0]=b'000: Internal clock: counts on Pclock/1 */

	/* ---- Timer counter_0(TCNT_0) ---- */
    MTU0.TCNT = 0x0000;        /* Clear the timer counter */

    /* ---- Timer general register_0 ---- */
    MTU0.TGRA = CH0_TGRA_CYCLE; /* Set compare match register. Set speed cycle */
    MTU0.TGRC = CH0_TGRC_CYCLE; /* Set compare match register. Set position cycle   */
    MTU0.TGRB = 0x0000;         /* Clear Input capture register */
    MTU0.TGRD = 0x0000;         /* Clear TGRB buffer register   */

    /* ---- Timer mode register_0 (TMDR_0) ---- */
    MTU0.TMDR.BYTE = 0x20;
	/* 7  :   =b'0   : reserve        */
	/* 6  :BFE=b'0   : TGRE_0 and TGRF_0 operate normally */
	/* 5  :BFB=b'1   : TGRB and TGRD buffer operation */
	/* 4  :BFA=b'0   : TGRA and TGRC  operate normally */
	/* 3-0:MD[3:0]=b'0000: Normal operation    */

    /* ---- Timer I/O control register_0 (TIORH_0,TIORL_0) ---- */
    MTU0.TIORH.BYTE = 0xC0;
	/* 7-4 IOB[3:0]=b'1100 :
			Capture input source is channel 1 count clock.
			Input capture at TCNT_1 count-up/count-down.  */
	/* 3-0 IOA[3:0]=b'0000 :Output retained */

    MTU0.TIORL.BYTE = 0x00;
	/* 7-4 IOD[3:0]=b'0000 :Output retained */
	/* 3-0 IOC[3:0]=b'0000 :Output retained */

    /* ---- Timer interrupt enable register_0 (TIER_0) ---- */
    MTU0.TIER.BYTE = 0x02;
	/* 7  : TTGE=b'0 : A/D  Start Request Disabled */
	/* 6-5:     =b'0 : reserve */
	/* 4  :TCIEV=b'0 : Overflow Interrupt Disable   */
	/* 3  :TGIED=b'0 : TGID Interrupt Disable      */
	/* 2  :TGIEC=b'0 : TGIC Interrupt Disable      */
	/* 1  :TGIEB=b'1 : TGIB Interrupt Enable       */
	/* 0  :TGIEA=b'0 : TGIA Interrupt Disable      */
}

void tpu_init(void)
{

	/* ==== Setting of Power-Down Modes ==== */
	/* Startup channels 0 and 1 of tpu */
	MSTP(TPU0) = 0;
	MSTP(TPU1) = 0;

	/* ==== Setting of TPU  ==== */
    /* ---- reset TPU Timer Start Register (TSTR) ---- */
    TPUA.TSTR.BYTE &= 0xFC;      /* CH0-1 count operation is stopped */

    /* ==== Setting of TPU( CH1 )  ==== */
    /* ---- Timer control register_1(TCR_1) ---- */
    TPU1.TCR.BYTE = 0x04; /* TCNT clearing disabled
    						-External clock TCLKA*/

    /* ---- Timer counter_1(TCNT_1) ---- */
    TPU1.TCNT = 0x0000;        /* Clear the timer counter */

    /* ---- Timer general register_1 ---- */
    TPU1.TGRA = 0x0000;        /* Clear input capture register */
    TPU1.TGRB = 0x0000;        /* Clear input capture register */

    /* ---- Timer mode register_1 (TMDR_1) ---- */
    TPU1.TMDR.BYTE = 0x04;
	/* 7-4:       =b'0000:        */
	/* 3-0:MD[3:0]=b'0100: Phase counting mode 1    */

    /* ---- Timer I/O control register_1 (TIOR_1) ---- */
    TPU1.TIOR.BYTE = 0xcc;
	/* 7-4 IOB[3:0]=b'1100 :Input capture at generation of
							TGRC_0 compare match */
	/* 3-0 IOA[3:0]=b'1100 :Input capture at generation of
							TGRA_0 compare match */

    /* reset all TSR 1 flags */
    if(TPU1.TSR.BYTE)
     {
    	if(TPU1.TSR.BIT.TGFA)
    		TPU1.TSR.BIT.TGFA = 0;
    	if(TPU1.TSR.BIT.TGFB)
    		TPU1.TSR.BIT.TGFB = 0;
    	if(TPU1.TSR.BIT.TCFV)
    		TPU1.TSR.BIT.TCFV = 0;
    	if(TPU1.TSR.BIT.TCFU)
    		TPU1.TSR.BIT.TCFU = 0;
     }

    /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
    TPU1.TIER.BYTE = 0x33;
	/* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
	/* 6  :     =b'0 : reserve        */
	/* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
	/* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
	/* 3-2:     =b'00: reserve        */
	/* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
	/* 0  :TGIEA=b'1 : TGRA Interrupt Enable        */

   /* ==== Setting of TPU( CH0 )  ==== */
   /* ---- Timer control register_0(TCR_0) ---- */
	TPU0.TCR.BYTE = 0xA0;
            /* 7-5:CCLR[2:0]=b'101: TCNT cleared by TGRC compare match */
            /* 4-3:CKEG[1:0]=b'00 : Count at rising edge */
            /* 2-0:TPSC[2:0]=b'000: Internal clock: counts on Pclock/1 */

	/* ---- Timer counter_0(TCNT_0) ---- */
	TPU0.TCNT = 0x0000;        /* Clear the timer counter */

    /* ---- Timer general register_0 ---- */
	TPU0.TGRA = CH0_TGRA_CYCLE; /* Set compare match register. Set speed cycle */
	TPU0.TGRC = CH0_TGRC_CYCLE; /* Set compare match register. Set position cycle   */
	TPU0.TGRB = 0x0000;         /* Clear Input capture register */
	TPU0.TGRD = 0x0000;         /* Clear TGRB buffer register   */

    /* ---- Timer mode register_0 (TMDR_0) ---- */
	TPU0.TMDR.BYTE = 0x20;
	/* 7  :ICSELD=b'0   :  */
	/* 6  :ICSELB=b'0   :  */
	/* 5  :BFB=b'1   : TGRB and TGRD buffer operation */
	/* 4  :BFA=b'0   : TGRA and TGRC  operate normally */
	/* 3-0:MD[3:0]=b'0000: Normal operation    */

    /* ---- Timer I/O control register_0 (TIORH_0,TIORL_0) ---- */
	TPU0.TIORH.BYTE = 0xC0;
	/* 7-4 IOB[3:0]=b'1100 :
			Capture input source is channel 1 count clock.
			Input capture at TCNT_1 count-up/count-down.  */
	/* 3-0 IOA[3:0]=b'0000 :Output retained */
	TPU0.TIORL.BYTE = 0x00;
	/* 7-4 IOD[3:0]=b'0000 :Output retained */
	/* 3-0 IOC[3:0]=b'0000 :Output retained */

    /* reset all TSR 0 flags */
    if(TPU0.TSR.BYTE)
     {
    	if(TPU0.TSR.BIT.TGFA)
    		TPU0.TSR.BIT.TGFA = 0;
    	if(TPU0.TSR.BIT.TGFB)
    		TPU0.TSR.BIT.TGFB = 0;
    	if(TPU0.TSR.BIT.TGFC)
    		TPU0.TSR.BIT.TGFC = 0;
    	if(TPU0.TSR.BIT.TGFD)
    		TPU0.TSR.BIT.TGFD = 0;
    	if(TPU0.TSR.BIT.TCFV)
    		TPU0.TSR.BIT.TCFV = 0;
     }

    /* ---- Timer interrupt enable register_0 (TIER_0) ---- */
	TPU0.TIER.BYTE = 0x02;
	/* 7  : TTGE=b'0 : A/D  Start Request Disabled */
	/* 6-5:     =b'0 : reserve */
	/* 4  :TCIEV=b'0 : Overflow Interrupt Disable   */
	/* 3  :TGIED=b'0 : TGID Interrupt Disable      */
	/* 2  :TGIEC=b'0 : TGIC Interrupt Disable      */
	/* 1  :TGIEB=b'1 : TGIB Interrupt Enable       */
	/* 0  :TGIEA=b'0 : TGIA Interrupt Disable      */
}

void tpu_init_(void){
		/* ==== Setting of Power-Down Modes ==== */
		/* Startup channels 3 and 4 of TPU */
		MSTP(TPU3) = 0;
		MSTP(TPU4) = 0;

		/* ==== Setting of TPU  ==== */
	    /* ---- reset TPU Timer Start Register (TSTR) ---- */
	    TPUA.TSTR.BYTE &= 0xE7;      /* CH3-4 count operation is stopped */

	    /* ==== Setting of TPU( CH4 )  ==== */
	    /* ---- Timer control register_4(TCR_4) ---- */
	    TPU4.TCR.BYTE = 0x05; /* TCNT clearing disabled
	    						-External clock TCLKC
	    						*/

	    /* ---- Timer counter_4(TCNT_4) ---- */
	    TPU4.TCNT = 0x0000;        /* Clear the timer counter */

	    /* ---- Timer general register_4 ---- */
	    TPU4.TGRA = 0x0000;        /* Clear input capture register */
	    TPU4.TGRB = 0x0000;        /* Clear input capture register */

	    /* ---- Timer mode register_4 (TMDR_4) ---- */
	    TPU4.TMDR.BYTE = 0x04;
		/* 7-4:       =b'0000:         */
		/* 3-0:MD[3:0]=b'0100: Phase counting mode 1    */

	    /* ---- Timer I/O control register_4 (TIOR_4) ---- */
	    TPU4.TIOR.BYTE = 0xcc;
		/* 7-4 IOB[3:0]=b'1100 :Input capture at generation of
								TGRC_3 compare match */
		/* 3-0 IOA[3:0]=b'1100 :Input capture at generation of
								TGRA_3 compare match */

	    /* reset all TSR 4 flags */
	     if(TPU4.TSR.BYTE)
	     {
	    	if(TPU4.TSR.BIT.TGFA)
	    		TPU4.TSR.BIT.TGFA = 0;
	    	if(TPU4.TSR.BIT.TGFB)
	    		TPU4.TSR.BIT.TGFB = 0;
	    	if(TPU4.TSR.BIT.TCFV)
	    		TPU4.TSR.BIT.TCFV = 0;
	    	if(TPU4.TSR.BIT.TCFU)
	    		TPU4.TSR.BIT.TCFU = 0;
	     }

	    /* ---- Timer interrupt enable register_4 (TIER_4) ---- */
	    TPU4.TIER.BYTE = 0x33;
		/* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
		/* 6  :     =b'0 : reserve        */
		/* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
		/* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
		/* 3-2:     =b'00: reserve        */
		/* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
		/* 0  :TGIEA=b'1 : TGRA Interrupt Enable        */



	   /** ==== Setting of TPU( CH3 )  ==== */
	   /* ---- Timer control register_3(TCR_3) ---- */
		TPU3.TCR.BYTE = 0xA8;
		/* 7-5:CCLR[2:0]=b'101: TCNT cleared by TGRC compare match */
		/* 4-3:CKEG[1:0]=b'01 : Count at rising edge */
		/* 2-0:TPSC[2:0]=b'000: Internal clock: counts on Pclock/1 */

		/* ---- Timer counter_3(TCNT_3) ---- */
		TPU3.TCNT = 0x0000;        /* Clear the timer counter */

	    /* ---- Timer general register_3 ---- */
		TPU3.TGRA = CH0_TGRA_CYCLE; /* Set compare match register. Set speed cycle */
		TPU3.TGRC = CH0_TGRC_CYCLE; /* Set compare match register. Set position cycle   */
		TPU3.TGRB = 0x0000;         /* Clear Input capture register */
		TPU3.TGRD = 0x0000;         /* Clear TGRB buffer register   */

	    /* ---- Timer mode register_3 (TMDR_3) ---- */
		TPU3.TMDR.BYTE = 0x20;
		/* 7  :ICSELD=b'0   :  */
		/* 6  :ICSELB=b'0   :  */
		/* 5  :BFB=b'1   : TGRB and TGRD buffer operation */
		/* 4  :BFA=b'0   : TGRA and TGRC  operate normally */
		/* 3-0:MD[3:0]=b'0000: Normal operation    */


	    /* ---- Timer I/O control register_3 (TIORH_3,TIORL_3) ---- */
		TPU3.TIORH.BYTE = 0xC0;
		/* 7-4 IOB[3:0]=b'1100 :
				Capture input source is channel 4 count clock.
				Input capture at TCNT_4 count-up/count-down.  */
		/* 3-0 IOA[3:0]=b'0000 :Output retained */
		TPU3.TIORL.BYTE = 0x00;
		/* 7-4 IOD[3:0]=b'0000 :Output retained */
		/* 3-0 IOC[3:0]=b'0000 :Output retained */

	    /* reset all TSR 3 flags */
	    if(TPU3.TSR.BYTE)
	     {
	    	if(TPU3.TSR.BIT.TGFA)
	    		TPU3.TSR.BIT.TGFA = 0;
	    	if(TPU3.TSR.BIT.TGFB)
	    		TPU3.TSR.BIT.TGFB = 0;
	    	if(TPU3.TSR.BIT.TGFC)
	    		TPU3.TSR.BIT.TGFC = 0;
	    	if(TPU3.TSR.BIT.TGFD)
	    		TPU3.TSR.BIT.TGFD = 0;
	    	if(TPU3.TSR.BIT.TCFV)
	    		TPU3.TSR.BIT.TCFV = 0;
	     }

	    /* ---- Timer interrupt enable register_3 (TIER_3) ---- */
		TPU3.TIER.BYTE = 0x02;
		/* 7  : TTGE=b'0 : A/D  Start Request Disabled */
		/* 6-5:     =b'0 : reserve */
		/* 4  :TCIEV=b'0 : Overflow Interrupt Disable   */
		/* 3  :TGIED=b'0 : TGID Interrupt Disable      */
		/* 2  :TGIEC=b'0 : TGIC Interrupt Disable      */
		/* 1  :TGIEB=b'1 : TGIB Interrupt Enable       */
		/* 0  :TGIEA=b'0 : TGIA Interrupt Disable      */
}

void pin_enc1_init(void)
{
    /* porta P24, pin 25 conn. JN2 */
	MPC.P24PFS.BYTE  = 0x02; /* 1 defines P24 to be MTCLKA, with no IRQ */

    PORT2.PDR.BIT.B4 = 0;    /* Set P24 as input */

    PORT2.PMR.BIT.B4 = 1;    /* Set P24 as peripheral function bit */

    /* porta PA6, pin 6 conn. JN2 */
    MPC.PA6PFS.BYTE  = 0x02; /* 1 defines PA6 to be MTCLKB, with no IRQ */

    PORTA.PDR.BIT.B6 = 0;    /* Set PA6 as input */

    PORTA.PMR.BIT.B6 = 1;    /* Set PA6 as peripheral function bit */
}

void pin_enc2_init(void)
{
    /* porta P14, Capo della R42 vicino all'U12 porta usb-user */
	MPC.P14PFS.BYTE  = 0x04; /* defines P14 to be TCLKA, with no IRQ */

    PORT1.PDR.BIT.B4 = 0;    /* Set P14 as input */

    PORT1.PMR.BIT.B4 = 1;    /* Set P14 as peripheral function bit */

    /* porta P15, pin 23 J8 */
    MPC.P15PFS.BYTE  = 0x04; /* defines P15 to be TCLKB, with no IRQ */

    PORT1.PDR.BIT.B5 = 0;    /* Set P15 as input */

    PORT1.PMR.BIT.B5 = 1;    /* Set P15 as peripheral function bit */
}

void pin_enc3_init(void)
{
	/* porta P16, capo R36 o R37 */
	MPC.P16PFS.BYTE  = 0x04; /* defines P16 to be TCLKC, with no IRQ */

    PORT1.PDR.BIT.B6 = 0;    /* Set P16 as input */

    PORT1.PMR.BIT.B6 = 1;    /* Set P16 as peripheral function bit */

#if 1
    /* porta PC1, pin 9 conn. JN2 */
    MPC.PC1PFS.BYTE  = 0x03; /* defines PC1 to be TCLKD, with no IRQ */

    PORTC.PDR.BIT.B1 = 0;    /* Set PC1 as input */

    PORTC.PMR.BIT.B1 = 1;    /* Set PC1 as peripheral function bit */
#endif
}

/*
 * Calcola posizione e velocita' di un encoder; usa una macchina a stati
 * per non tenere continuamente impegnato il micro in phase counting.
 *
 * Torna 1 se ha successo, 0 altrimenti;
 * implementa una lettura non bloccante per il resto del codice
 *
 * */
void Query_Enc(encoder_data* enc, parametri_inr_config* p_i_c, identifica_enc* id_enc)
{
 switch(id_enc->state)
 {
  case INIT_STATE_ENC:
	   p_i_c->posizione_acquisita = 0;
	   p_i_c->speed_sample = 0;

	   /* Enable interrupt TGIA1 and TGIB1 */
	   /* ---- Timer interrupt enable register_1 (TIER_1) ---- */
	   if(id_enc->num_enc == 1)
		   MTU1.TIER.BYTE = 0x33;
	   if(id_enc->num_enc == 2)
		   TPU1.TIER.BYTE = 0x33;
	   if(id_enc->num_enc == 3)
		   TPU4.TIER.BYTE = 0x33;


	   /* 7  : TTGE=b'0 : A/D Converter Start Request Disabled */
	   /* 6  :     =b'0 : reserve        */
	   /* 5  :TCIEU=b'1 : Underflow Interrupt Enable   */
	   /* 4  :TCIEV=b'1 : Overflow Interrupt Enable    */
	   /* 3-2:     =b'00: reserve        */
	   /* 1  :TGIEB=b'1 : TGRB Interrupt Enable        */
	   /* 0  :TGIEA=b'1 : TGRA Interrupt Enable        */
	   id_enc->state = READ_STATE_ENC;
	   break;

  case READ_STATE_ENC:
	   if(p_i_c->posizione_acquisita == 1 && p_i_c->speed_sample == 1)
		{
		   p_i_c->posizione_acquisita = 0;
		   p_i_c->speed_sample = 0;

			/* calcolo posizione e velocita' in base ai valori acquisiti */

			/* Calcolo la posizione */
		    if(p_i_c->Contatore_di_overflow < 0)
		     {
		      /* absolute position */
		      enc->position = -(( ((0xFFFF + 1) * (p_i_c->Contatore_di_overflow + 1)) + (0xFFFF - p_i_c->TGRA1_data + 1) ) % p_i_c->MODULO_ENC);
		     } else
		     	 {
		    	 /* absolute position */
		    	  enc->position = ((((0xFFFF + 1) * p_i_c->Contatore_di_overflow) + p_i_c->TGRA1_data) % p_i_c->MODULO_ENC);
		     	 }

		    /* Calcolo la velocita' */
			/* non c'e' stato overflow ne' underflow (oppure c'e' stata un'oscillazione) */
		    /* Per generare una Query_enc singolare per i tre encoder abbiamo bisogno di separare la parte di configurazione
		     * che riguarda i registri di utilizzo di ogni encoder  */
		    if(id_enc->num_enc == 1)
		    {
		    	if(p_i_c->Read_overflow == p_i_c->Read_overflow_old)
				 {
					 p_i_c->speed_data = p_i_c->TGRB1_data_new - p_i_c->TGRB1_data_old; 	/* new - old  */
					 if((p_i_c->speed_data <= 0 && (MTU1.TSR.BIT.TCFD == 1)) || (p_i_c->speed_data >= 0 && (MTU1.TSR.BIT.TCFD == 0)))
					 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
					 else enc->oscillation = 0;
				 }else if(p_i_c->Read_overflow > p_i_c->Read_overflow_old) /* overflow */
						 {
					 p_i_c->speed_data = p_i_c->TGRB1_data_new + (0xFFFF - p_i_c->TGRB1_data_old + 1); 	/* new - old  */
							 if(p_i_c->speed_data >= 0 && (MTU1.TSR.BIT.TCFD == 0))
							 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
							 else enc->oscillation = 0;
						 } else if(p_i_c->Read_overflow < p_i_c->Read_overflow_old) /* underflow */
								 {
							p_i_c->speed_data = -((0xFFFF - p_i_c->TGRB1_data_new + 1) + p_i_c->TGRB1_data_old); 	/* new - old  */
									 if(p_i_c->speed_data <= 0 && (MTU1.TSR.BIT.TCFD == 1))
									 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
									 else enc->oscillation = 0;
								 }
		    }
		    if(id_enc->num_enc == 2)
		    {
		    	if(p_i_c->Read_overflow == p_i_c->Read_overflow_old)
				 {
					 p_i_c->speed_data = p_i_c->TGRB1_data_new - p_i_c->TGRB1_data_old; 	/* new - old  */
					 if((p_i_c->speed_data <= 0 && (TPU1.TSR.BIT.TCFD == 1)) || (p_i_c->speed_data >= 0 && (TPU1.TSR.BIT.TCFD == 0)))
					 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
					 else enc->oscillation = 0;
				 }else if(p_i_c->Read_overflow > p_i_c->Read_overflow_old) /* overflow */
						 {
					 p_i_c->speed_data = p_i_c->TGRB1_data_new + (0xFFFF - p_i_c->TGRB1_data_old + 1); 	/* new - old  */
							 if(p_i_c->speed_data >= 0 && (TPU1.TSR.BIT.TCFD == 0))
							 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
							 else enc->oscillation = 0;
						 } else if(p_i_c->Read_overflow < p_i_c->Read_overflow_old) /* underflow */
								 {
							p_i_c->speed_data = -((0xFFFF - p_i_c->TGRB1_data_new + 1) + p_i_c->TGRB1_data_old); 	/* new - old  */
									 if(p_i_c->speed_data <= 0 && (TPU1.TSR.BIT.TCFD == 1))
									 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
									 else enc->oscillation = 0;
								 }
		    }
		    if(id_enc->num_enc == 3)
			{
				if(p_i_c->Read_overflow == p_i_c->Read_overflow_old)
				 {
					 p_i_c->speed_data = p_i_c->TGRB1_data_new - p_i_c->TGRB1_data_old; 	/* new - old  */
					 if((p_i_c->speed_data <= 0 && (TPU4.TSR.BIT.TCFD == 1)) || (p_i_c->speed_data >= 0 && (TPU4.TSR.BIT.TCFD == 0)))
					 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
					 else enc->oscillation = 0;
				 }else if(p_i_c->Read_overflow > p_i_c->Read_overflow_old) /* overflow */
						 {
					 p_i_c->speed_data = p_i_c->TGRB1_data_new + (0xFFFF - p_i_c->TGRB1_data_old + 1); 	/* new - old  */
							 if(p_i_c->speed_data >= 0 && (TPU4.TSR.BIT.TCFD == 0))
							 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
							 else enc->oscillation = 0;
						 } else if(p_i_c->Read_overflow < p_i_c->Read_overflow_old) /* underflow */
								 {
							p_i_c->speed_data = -((0xFFFF - p_i_c->TGRB1_data_new + 1) + p_i_c->TGRB1_data_old); 	/* new - old  */
									 if(p_i_c->speed_data <= 0 && (TPU4.TSR.BIT.TCFD == 1))
									 { enc->oscillation = 1;} /* segnalo che e' una oscillazione */
									 else enc->oscillation = 0;
								 }
			}

			/* velocita' in impulsi al ms */
			enc->speed = (int)p_i_c->speed_data;

			/* posizione dell'albero in gradi */
			enc->position_in_degree = ((p_i_c->mDEGREE_PER_PULSE * enc->position) / 1000);

			/* Posizione in pi rad 								*/
			/* 180 : pi = deg : rad   =>   rad = pi * (deg/180) */
			enc->position_in_pigreek_radians_units = enc->position_in_degree/180;

			/* velocita' in gradi al secondo */
			enc->speed_in_degree_per_sec = (p_i_c->mDEGREE_PER_PULSE * enc->speed);

			/* velocita' in radianti al secondo */
			enc->speed_in_rad_per_sec = (PI_GREEK * enc->speed_in_degree_per_sec) / 180;

			/* posizione in radianti al secondo */
			enc->position_radiant = (enc->position_in_degree * PI_GREEK)/180;

			id_enc->state = INIT_STATE_ENC;
		}
	  break;
 }
}

/*
 * Restituisce la media delle velocita' sugli ultimi 10 campioni
 * di velocita' misurati; mette il risultato nella struttura enc
 * */
void Media_Speed(encoder_data* enc, parametri_inr_config* p_i_c)
{
	static int speed_vect[30] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static unsigned char queue_index = 0;
	unsigned int i;
	float velocita_media_1;

	velocita_media_1 = 0.0;

	speed_vect[queue_index] = enc->speed;
	queue_index++;
	if(queue_index >= 30) queue_index = 0;

	for(i = 0; i < 30; i++)
	{
		velocita_media_1 += (float)speed_vect[i];
	}

	velocita_media_1 = velocita_media_1 / 30.0;

	/* velocita' in gradi al secondo */
	enc->speed_in_degree_per_sec = (p_i_c->mDEGREE_PER_PULSE * velocita_media_1 );

	/* velocita' in radianti al secondo */
	enc->speed_in_rad_per_sec = (PI_GREEK * enc->speed_in_degree_per_sec) / 180;


}

/* Inizializzazione delle variabili usate per calcolo di posizione e velocita' */
void Init_Encoder_vars(parametri_inr_config* p_i_c, identifica_enc* id_enc, unsigned int num_enc)
{
	/* Numero di impulsi per giro dell'albero (con motoriduttore) */
	p_i_c->MODULO_ENC = (ENCODER_PPR * REDUCTION_GAIN * PHASE_COUNT_MODE);

	/* gradi per impulso per 1000 */
	if(p_i_c->MODULO_ENC)
		p_i_c->mDEGREE_PER_PULSE = (float)360000/(float)p_i_c->MODULO_ENC;

	/* resetto le variabili usate per i calcoli di posizione e velocita' */
	p_i_c->posizione_acquisita = 0;
	p_i_c->speed_sample = 0;
	p_i_c->Under_over_flow_cnt = 0;
	p_i_c->Read_overflow_old = 0;
	p_i_c->Read_overflow = 0;

	/* configurazione identificativo e macchina a stati */
	id_enc->num_enc = num_enc;
	id_enc->state = 1;
}

void Read_Vel_1LCD(encoder_data* enc)
{
	/* stampo i valori acquisiti */

	/* Declare display buffer */
	uint8_t  lcd_buffer[13];

	/* Display message on LCD */
	lcd_display(LCD_LINE1, "Enc data 1:");

	sprintf((char *)lcd_buffer, "r/s= %.3f" , enc->speed_in_rad_per_sec);
	lcd_display(LCD_LINE2, lcd_buffer);

}
void Read_Pos_1LCD(encoder_data* enc)
{
	/* stampo i valori acquisiti */

	/* Declare display buffer */
	uint8_t  lcd_buffer[13];


	sprintf((char *)lcd_buffer, "pos= %.3f" , enc->position_in_degree);
	lcd_display(LCD_LINE3, lcd_buffer);

}

void Read_Vel_2LCD(encoder_data* enc)
{
	/* stampo i valori acquisiti */

	/* Declare display buffer */
	uint8_t  lcd_buffer[13];

	/* Display message on LCD */
	lcd_display(LCD_LINE4, "Enc data 2:");

	sprintf((char *)lcd_buffer, "r/s= %.3f" , enc->speed_in_rad_per_sec);
	lcd_display(LCD_LINE5, lcd_buffer);

}
void Read_Pos_2LCD(encoder_data* enc)
{
	/* stampo i valori acquisiti */

	/* Declare display buffer */
	uint8_t  lcd_buffer[13];


	sprintf((char *)lcd_buffer, "pos= %.3f" , enc->position_in_degree);
	lcd_display(LCD_LINE6, lcd_buffer);

}

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

/* End of File */
