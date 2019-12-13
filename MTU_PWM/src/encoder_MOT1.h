#ifndef ENCODER_MOT1_H
#define ENCODER_MOT1_H

/* NOTA: Se non c'e' il motoriduttore, mettere REDUCTION_GAIN a 1 */

#define ENCODER_PPR			16 	/* ppr dell'encoder per il ballbot */

#define REDUCTION_GAIN		131 /* Rapporto di riduzione inverso totale (approssimato) */

#define PHASE_COUNT_MODE	4	/* Phase counting mode x4 normally */

/* Imposto ad 1ms gli interrupt di posizione e di velocita' sfalsandoli tra loro
   per non sovrapporre le catture dei valori su TGRA e su TGRB della MTU1 */

/* Set position cycle */
/* MTU2 CH0 TGRC compare match cycle(1ms) */
/* 1ms / 21ns(@48MHz) = 48000 */
#define CH0_TGRC_CYCLE   (unsigned short)(48000-1)

/* Set speed cycle */
/* MTU2 CH0 TGRC compare match cycle(0.84ms)  	*/
/* 0.84ms / 21ns(@48MHz) = 40.........000 				*/
#define CH0_TGRA_CYCLE   (unsigned short)(40000-1)

/* Pi greco per la conversione gradi-radianti */
#define PI_GREEK 3.1416

/* Macchina a stati per la lettura */
#define INIT_STATE_ENC 	1
#define READ_STATE_ENC 	2

void MTU1_TCIV_TCUV_isr(void); /* ch1 TCNT Overflow Interrupt - ch1 TCNT Underflow Interrupt */

/* Struttura principale che contiene i dati misurati di posizione e velocita' */
typedef struct
{
	int		position; /* posizione assoluta in numero di impulsi (con segno) a partire dalla posizione iniziale assunta uguale a 0 */
	int		speed; /* velocita' in numero di impulsi al ms */
	int		oscillation; /* segnala se l'encoder (l'albero) è fermo */
	float	position_in_degree; /* posizione in gradi con segno a partire dalla posizione iniziale assunta uguale a 0 gradi */
	float	position_radiant; /*posizione in radianti*/
	float	position_in_pigreek_radians_units; /* posizione in unita' pigreco con segno */
	float	speed_in_degree_per_sec; /* velocita' in gradi al secondo con segno */
	float	speed_in_rad_per_sec; /* velocita' in radianti al secondo con segno */
} encoder_data_t_mt_1;

/* ==== dichiarazione prototipi ==== */
/**********************************************************************************
* Nome funzione: encoder_1_init
* Descrizione  : Inizializzazione della stuttura encoder_data_t_mt_1
* Argomenti    : void
* Valore di ritorno : void
**********************************************************************************/
void encoder_1_init(void);

/**********************************************************************************
* Nome funzione: Init_Encoder_1_vars
* Descrizione  : Inizializzazione delle variabili utilizzate per i calcoli
* 				 dell'encoder
* Argomenti    : void
* Valore di ritorno : void
**********************************************************************************/
void Init_Encoder_1_vars(void);

/**********************************************************************************
* Nome funzione: mtu2_init
* Descrizione  : Inizializzazione della periferica MTU
* Argomenti    : void
* Valore di ritorno : void
**********************************************************************************/
void mtu2_init(void);

/**********************************************************************************
* Nome funzione: pfc_init
* Descrizione  : Inizializzazione dei pin dell'encoder
* Argomenti    : void
* Valore di ritorno : void
**********************************************************************************/
void pfc_init(void);

/**********************************************************************************
* Nome funzione: Query_Enc_1
* Descrizione  : Calcolo dei valori dell'encoder e memorizzazione nella struttura
* 				 encoder_data_t_mt_1
* Argomenti    : Puntatore alla struttura encoder_data_t_mt_1 (encoder_data_t_mt_1*)
* Valore di ritorno : void
**********************************************************************************/
void Query_Enc_1(encoder_data_t_mt_1*);

/**********************************************************************************
* Nome funzione: Media_Speed_Enc_1
* Descrizione  : Utilizzo delle ultime 100 velocità lette mediate tra di loro per
* 				 ottenere un valore finale della velocità più preciso
* Argomenti    : Puntatore alla struttura encoder_data_t_mt_1 (encoder_data_t_mt_1*)
* Valore di ritorno : void
**********************************************************************************/
void Media_Speed_Enc_1(encoder_data_t_mt_1*);

#endif
