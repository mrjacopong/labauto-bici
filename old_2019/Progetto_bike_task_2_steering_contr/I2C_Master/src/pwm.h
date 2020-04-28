/********************************************************************************************************************************
 * Nome file: pwm.h
 * Versione: 1.0
 * Descrizione:Viene impiegata per l’inizializzazione, il calcolo e il settaggio dei segnali di PWM (Pulse With Modulation)
 * e direzione che poi saranno inviati al driver per il comando del motore;
 *  *******************************************************************************************************************************/
/********************************************************************************************************************************
 * Hystory: file: DD.MM.YYYY    Versione   Descrizione
 *                06/03/2019    1.0        Prima Release
 *******************************************************************************************************************************/

#ifndef PWM_H_
#define PWM_H_
#define Max_Duty				99

typedef struct
{
	float volt_signal;   //voltaggio istantaneo
	float duty;          //percentuale della tensione in uscita dal PWM
    float max_volt;      //voltaggio massimo
	unsigned char ch;    //canale del motore
	int direction_signal;//valore direzionale
} Motor;


/********************************************************************************************************
 * NOTE:
 * con le impostazioni della MTU2 nella funzione PWM_Init(),
 * a 20kHz ho un controllo sul duty di +/- 0.2%
 * a 10kHz il controllo sul duty di  +/- 0.1%
 * a  1kHz il controllo sul duty di  +/- 0.01%
 * Per infittire la 'granularita'' del controllo sul duty in alta frequenza si puo' agire sul prescaler.
 * Per ora lo imposto a 4, per cui TCNT conta a (RSK_PERIPHERAL_CLOCK_FREQ/4).
 * Il valore di TGRA, TGRA_x_VAL, determina il periodo dell'onda PWM,
 * (dipende dalla frequenza desiderata dell'onda PWM)
 * e viene impostato con le direttive che seguono.
 * I canali numerati come 1 e 2 sono uscite del timer 3 della MTU2,
 * i canali numerati come 3 e 4 sono uscite del timer 4 della MTU2.
 * I canali 1 e 2 lavorano in coppia e danno in output onde pwm
 * della stessa frequenza (ma il duty puo' essere diverso!).
 * Lo stesso accade per la coppia di canali 3 e 4.
 * Dato che si usa una sola PWM_FREQ uguale per tutti i canali
 * tutte le uscite PWM hanno stessa frequenza.
 * Ma le PWM_FREQ si possono differenziare tra loro, per cui modificando il codice
 * si possono ottenere in output frequenze diverse per ciascun timer della MTU2.
 * I duty dei 4 canali sono indipendenti tra loro.
 ********************************************************************************************************/

#define V_MAX		3.300		/* volt */
#define YRDK_PERIPHERAL_CLOCK_FREQ     48000000 /* System PCLK Frequency in HZ. */
/* Max counting value for TGRA_3=> FFFE (hex, 65534 dec) */
#define TGRA_3_VAL	((unsigned short int)((YRDK_PERIPHERAL_CLOCK_FREQ/4)/PWM_FREQ))
/* Max counting value for TGRA_4=> FFFE (hex, 65534 dec) */
#define TGRA_4_VAL	((unsigned short int)((YRDK_PERIPHERAL_CLOCK_FREQ/4)/PWM_FREQ))
/* in Hz: MIN 160Hz, MAX 20kHz  */
#define PWM_FREQ 1000		/* scegliere il valore tra 160 e 20000 (Hz) */


/**********************************************************************************
* Nome funzione: PWM_Init
* Descrizione  : Inizializzazione dei canali per il PWM
* Argomenti    : unsigned char, float, Motor
* Valore di ritorno : Niente
**********************************************************************************/
void PWM_Init(unsigned char channel_number,float max_volt,Motor* m);

/*******************************************************************************
* Function name: motor_direction
* Description  : Controllo della direzione del motore ed eventuale negazione
*                del segnale di tensione
* Arguments    : Motor
* Return value : none
*******************************************************************************/
void motor_direction(Motor* m);

/*******************************************************************************
* Function name: DutyCycle_to_Motor
* Description  : Calcolo del duty cycle e della direzione del motore
* Arguments    : Motor
* Return value : none
*******************************************************************************/
void DutyCycle_to_Motor(Motor* m);

/**********************************************************************************
* Nome funzione: Init_Port_Dir
* Descrizione  : Inizializzazione dei pin per l'uscita del PWM
* Argomenti    : void
* Valore di ritorno : void
**********************************************************************************/
void Init_Port_Dir(void);

/**********************************************************************************
* Nome funzione: DutyCycle
* Descrizione  : Settaggio valore registri per duty cycle del PWM
* Argomenti    : Valore duty cycle e numero canale (float, unsigned char)
* Valore di ritorno : void
**********************************************************************************/
void DutyCycle (float duty_cycle, unsigned char channel);

#endif /* PWM_H_ */
