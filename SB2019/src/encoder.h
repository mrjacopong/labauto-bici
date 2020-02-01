#ifndef ENCODER_H
#define ENCODER_H

/* NOTA: Se non c'e' il motoriduttore, mettere REDUCTION_GAIN a 1 */

#define ENCODER_PPR			16 	// ppr dell'encoder per il ballbot (Per la gamba 500)
#define REDUCTION_GAIN		100 // Rapporto di riduzione inverso totale (approssimato)
 	 	 	 	 	 	 	 	// dalle prove sembra che per ogni giro dell'albero ci siano
 	 	 	 	 	 	 	 	// circa 66 giri dell'encoder (per la gamba).
 	 	 	 	 	 	 	 	// Per il ballbot 100

#define PHASE_COUNT_MODE	4	// Phase counting mode x4 normally


/* Imposto ad 1ms gli interrupt di posizione e di velocita' sfalsandoli tra loro
 * per non sovrapporre le catture dei valori su TGRA e su TGRB della MTU1 */
#define CH0_TGRC_CYCLE   (unsigned short)(48000-1)
                                /* MTU2 CH0 TGRC compare match cycle(1ms)  	*/
                                /* 1ms / 21ns(@48MHz) = 48000 				*/
#define CH0_TGRA_CYCLE   (unsigned short)(40000-1)
                                /* MTU2 CH0 TGRC compare match cycle(0.84ms)  	*/
                                /* 0.84ms / 21ns(@48MHz) = 40000 				*/

/* pi per la conversione gradi-radianti */
#define PI_GREEK 3.1416

// Stati per la macchina a stati
#define INIT_STATE_ENC 1
#define READ_STATE_ENC 2

void MTU1_TCIV_TCUV_isr(void); /* ch1 TCNT Overflow Interrupt - ch1 TCNT Underflow Interrupt 	*/

/* Struttura principale che contiene i dati misurati di posizione e velocita' */
typedef struct
{
	int   position;  							/* posizione assoluta in numero di impulsi (con segno) a partire dalla posizione iniziale assunta uguale a 0 */
	int   speed;     							/* velocita' in numero di impulsi al ms */
	int   oscillation;  						/* segnala se l'encoder (l'albero) è fermo */
	float position_in_degree; 					/* posizione in gradi con segno a partire dalla posizione iniziale assunta uguale a 0 gradi */
	float position_radiant; 					/*posizione in radianti*/
	float position_in_pigreek_radians_units; 	/* posizione in unita' pigreco con segno */
	float speed_in_degree_per_sec;			 	/* velocita' in gradi al secondo con segno */
	float speed_in_rad_per_sec; 				/* velocita' in radianti al secondo con segno */
} encoder_data;

/* Stuttura che incorpora tutti i parametri comuni di utilizzo generale per il corretto funzionamento dell' encoder */
typedef struct{
	unsigned char posizione_acquisita;
	unsigned char speed_sample;
	signed long speed_data; 				/* numero di impulsi catturati ogni ms (velocita' in impulsi al ms) */
	unsigned int MODULO_ENC;				/* Numero di impulsi per giro dell'albero (tiene conto anche del motoriduttore) */
	int Read_overflow_old;					/* variabili usate per il calcolo della velocita' */
	int Read_overflow;
	float mDEGREE_PER_PULSE;				/* numero di gradi per ogni impulso moltiplicato per mille */
	unsigned short TGRA1_data;				/* variabile per il calcolo della posizione istantanea*/
	unsigned short TGRB1_data_old; 	    	/* MTU ch1 TGRB capture data(old)  */
	unsigned short TGRB1_data_new;			/* MTU ch1 TGRB capture data(new)  */
	signed short int Under_over_flow_cnt; 	/* Numero complessivo (con segno!) di overflow e di underflow */
	signed short int Contatore_di_overflow; /* variabile di appoggio con il numero di overflow-underflow */
	signed long TGRD0_B0_data_diff;			/* l'interrupt su TGRB0 e' definito ma non lo uso per ora */
}parametri_inr_config;

typedef struct{
	unsigned int num_enc;
	unsigned char state;
}identifica_enc;

/* Dichiarazione delle funzioni definite nel file encoder_MOT1.c */
void encoder_init_1(void);
void encoder_init_2(void);
void encoder_init_3(void);
void pin_enc1_init(void);
void pin_enc2_init(void);
void pin_enc3_init(void);
void mtu2_init(void);
void tpu_init(void); /*tpu0 tpu1*/
void tpu_init_(void);/*tpu3 tpu4*/
void Init_Encoder_vars(parametri_inr_config*, identifica_enc*,unsigned int);
void Query_Enc(encoder_data*, parametri_inr_config*,identifica_enc*);
void Media_Speed(encoder_data*, parametri_inr_config*);

void Read_Pos_1LCD(encoder_data* enc);
void Read_Vel_1LCD(encoder_data* enc);
void Read_Pos_2LCD(encoder_data* enc);
void Read_Vel_2LCD(encoder_data* enc);


//parametri_inr_config par_encoder_1;
//identifica_enc enc_id_1;
//parametri_inr_config par_encoder_2;
//identifica_enc enc_id_2;

#endif
