
/*******************************************************************************
* File Name    : mtu_pwm_demo_main.c
* Version      : 1.0 
* Device(s)	   : RX63N 
* Tool-Chain   : Renesas RX Standard Toolchain 1.0.0
* H/W Platform : YRDKRX63N 
* Description  : This is a basic entry point for the MTU PWM mode1 demo. 
*              : This sample application sets up a MTU channel (MTIOC4B)   
*              : to generate a pulse-width modulated waveform (PWM wave)
*              : on an output pin (PIN 25 of header JN2) that directly 
*              : can drives an electric load with a variable voltage 
*              : level between 0 and 3.3V. 
*              : A function named "Volt_2_PWMduty" generate a proper PWM
*              : signal having a duty-cycle such as the output voltage level 
*              : (range 0-3.3V) is the desired voltage level in volt 
*              : In addition, an interupt service routine changes
*              : the PWM duty cycle and toggles the port pins that drive 
*              : the channel.
*
* Operation	   : 1. Compile and download the sample code. Click 'Reset Go'
*                   to start the software.   
*                2. Observe PWM waveform at header JN2 pin 24 with an
*                   oscilloscope (GND on header BEAGLE PORT pin 2 or 10).
*
* note:        : Demo runs continuously.
*******************************************************************************/

/*****************************************************************************
* History : DD.MM.YYYY  Version  Description               Author
*         : 01.09.2014  1.00     First Release commented   Andrea Bonci
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "mtu_pwm_mode1_demo.h"
#include "cmt.h"
#include "s12adc.h"
#include "encoder_MOT1.h"





float Desired_Volt = 0.5; /* desired output Voltage (admissible range 0 - 3.3V) 

/******************************************************************************
* Function name: main
* Description  : Main program function. Initializes the LCD, the debug console,
*                and the MTU. Then it remains in an infinite while loop. The
*                demo executes continuously. 
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
	CMT_init();
	//init dell'encoder
	// Creazione variabile di tipo struttura encoder che verrà usata per le funzioni dell'encoder.
	encoder_data_t_mt_1* enc_1;
	char velocita[12];
	encoder_1_init();
	Init_Encoder_1_vars();
	pfc_init();
	//inizializzazione pwm
	//mtu2_init();



    /* Initialize LCD */
    lcd_initialize();
    
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    lcd_display(LCD_LINE1, " YRDKRX63N  ");
    lcd_display(LCD_LINE2, "MTU PWM Demo");
    lcd_display(LCD_LINE3, "ch. MTIOC4A ");
    lcd_display(LCD_LINE4, " test encod ");

    /*  STDOUT is routed through the virtual console window tunneled through 
        the JTAG debugger. Open the console window in HEW to see the output. */
    printf("This is the debug console\r\n");

    //mtu_initialize();   /* Set up the MTU. */
    
    //mtu_start();        /* Start the PWM demo.*/

   // S12ADC_init();
    //S12ADC_start ();

    /* Do Volt_2_PWMduty in main loop. Demo is performed generating a PWM signal
	 * whose duty generate an output having medium level equal to Desired_Volt. */
    while (1)
    {
		//Letture della velocità dall'Encoder con media.

		Query_Enc_1(enc_1);
		Media_Speed_Enc_1(enc_1);
		Read_Vel_Enc_1(enc_1);
		Read_Pos_Enc_1(enc_1);
		for(int i=0;i<100000;i++){}
    	//float adc_volts = ((float) S12ADC_read()) / MAX_COUNTS * (VREFH - VREFL);
    	//printf("%d \r\n",adc_volts);
    	//Volt_2_PWMduty(adc_volts);
    }
} /* End of function main() */

/*******************************************************************************
* End of file: mtu_pwm_demo_main.c
*******************************************************************************/
