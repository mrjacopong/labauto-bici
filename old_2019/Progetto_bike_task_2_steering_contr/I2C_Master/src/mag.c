/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : mag.c
* Version      : 1.00
* Description  : Gestione e lettura magnetometro
* Author	   : Gruppo IMU-BIKE_2018
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 30.11.2018     1.00        First release
*******************************************************************************/

#include "mag.h"

#define HMC5983_ADDRESS 0x1E
#define HMC5983_WRITE	0x3C
#define HMC5983_READ 	0x3D

// MEMORY MAPPING
/*
Address Location	Name 		Access
---------------------------------------------------
0x00	Configuration Register A	Read/Write
0x01	Configuration Register B	Read/Write
0x02	Mode Register			Read/Write
0x03	Data Output X MSB Register	Read
0x04	Data Output X LSB Register	Read
0x05	Data Output Z MSB Register	Read
0x06	Data Output Z LSB Register	Read
0x07	Data Output Y MSB Register	Read
0x08	Data Output Y LSB Register	Read
0x09	Status Register			Read
0x0A	Identification Register A	Read
0x0B	Identification Register B	Read
0x0C	Identification Register C	Read
0x31	Temperature Output MSB Register	Read
0x32	Temperature Output LSB Register Read
 */

#define HMC5983_CONF_A		0x00
#define HMC5983_CONF_B		0x01
#define HMC5983_MODE		0x02
#define HMC5983_OUT_X_MSB	0x03
#define HMC5983_OUT_X_LSB	0x04
#define HMC5983_OUT_Z_MSB	0x05
#define HMC5983_OUT_Z_LSB	0x06
#define HMC5983_OUT_Y_MSB	0x07
#define HMC5983_OUT_Y_LSB	0x08
#define HMC5983_STATUS		0x09
#define HMC5983_ID_A		0x0A
#define HMC5983_ID_B		0x0B
#define HMC5983_ID_C		0x0C
#define HMC5983_TEMP_OUT_MSB	0x31
#define HMC5983_TEMP_OUT_LSB	0x32

#define HMC5983_ID_A_VAL    0x48


/*

CONF_A ========================================================

|BIT1|BIT0| MEASUREMENT MODE  |
| 0  | 0  | normal (default)  |
| 0  | 1  | positive bias	    |
| 1  | 0  | negative bias	    |
| 1  | 1  | temp only			    |

|BIT4|BIT3|BIT2| MEASUREMENT RATE [Hz] (in continuous mode)
| 0  | 0  | 0  | 0.75			    |
| 0  | 0  | 1  | 1.5 			    |
| 0  | 1  | 0  | 3				    |
| 0  | 1  | 1  | 7.5				  |
| 1  | 0  | 0  | 15	(default) |
| 1  | 0  | 1  | 30					  |
| 1  | 1  | 0  | 75					  |
| 1  | 1  | 1  | 220				  |

|BIT6|BIT5| AVERAGING WINDOW [samples]  |
| 0  | 0  | 1 (default)				          |
| 0  | 1  | 2           	              |
| 1  | 0  | 4           	              |
| 1  | 1  | 8       			              |

BIT7 = enable temperature sensor

CONF_B =========================================================

|BIT7|BIT6|BIT5| RESOLUTION [mG/LSb]  | Campo max (Gauss)
| 0  | 0  | 0  | 0.73			            | 0.88
| 0  | 0  | 1  | 0.92 (default)			  | 1.3
| 0  | 1  | 0  | 1.22				          | 1.9
| 0  | 1  | 1  | 1.52				          | 2.5
| 1  | 0  | 0  | 2.27                 | 4.0
| 1  | 0  | 1  | 2.56 			          | 4.7
| 1  | 1  | 0  | 3.03				          | 5.6
| 1  | 1  | 1  | 4.35 			          | 8.1

MODE ===========================================================

|BIT0| OPERATING MODE   |
| 0  | continuous		    |
| 1  | single (default)	|

BIT1 = enable idle mode
BIT7 = enable 3400khz i2c mode
BIT5 = enter low power mode
 */

int mag_init(MAG_data* mag_data){

	uint8_t tmp;

	// se stiamo usando il modulo drotek chiediamo ad MPU6050 di
	// farci accedere al magnetometro che è suo slave
	// altrimenti basta avviare l'i2c
#ifdef DROTEK_IMU_10DOF_V2
	if(mpu_set_bypass(1))
		return 0x1;
#else
	if(i2c_init())
		return 0x1;
#endif

	// leggiamo il registro identificativo e verifichiamo che abbia
	// il valore costante come da datasheet
	if(i2c_read(HMC5983_ADDRESS, HMC5983_ID_A, 1, &tmp))
		return 0x2;
	if(tmp != HMC5983_ID_A_VAL)
		return 0x3;


	// abilitiamo la continuous mode
	tmp = 0;
	if(i2c_write(HMC5983_ADDRESS, HMC5983_MODE, 1, &tmp))
		return 0x4;
	//scarico registro CONF A
	if(i2c_read(HMC5983_ADDRESS, HMC5983_CONF_A, 1, &tmp))
		return 0x4;
	//imposto frequenza a 220 Hz
	tmp |= 0x1C;
	//carico il registro modificato
	if(i2c_write(HMC5983_ADDRESS, HMC5983_CONF_A, 1, &tmp))
		return 0x5;
	//scarico registro CONF B
	if(i2c_read(HMC5983_ADDRESS, HMC5983_CONF_B, 1, &tmp))
		return 0x6;
	tmp = tmp >> 5;
	switch(tmp) {
	case 0: mag_data->sens = 0.73; break;
	case 1: mag_data->sens = 0.92; break;
	case 2: mag_data->sens = 1.22; break;
	case 3: mag_data->sens = 1.52; break;
	case 4: mag_data->sens = 2.27; break;
	case 5: mag_data->sens = 2.56; break;
	case 6: mag_data->sens = 3.03; break;
	case 7: mag_data->sens = 4.35; break;
	default: break;
	}

	mag_data->scale[0] = 1.024449;
	mag_data->scale[1] = 0.992659;
	mag_data->scale[2] = 0.983796;
	mag_data->bias[0]  = -253.000;
	mag_data->bias[1]  = -175.260;
	mag_data->bias[2]  = -425.040;
	mag_data->ABS 	 = 208088.016;

	return 0x0;


}

int mag_read_raw(MAG_data* mag_data){
	uint8_t tmp[6];

	if(i2c_read(HMC5983_ADDRESS, HMC5983_OUT_X_MSB, 6, tmp))
		return 0x1;

	mag_data->raw[0] = (tmp[0] << 8) | tmp[1];
	mag_data->raw[2] = (tmp[2] << 8) | tmp[3];
	mag_data->raw[1] = (tmp[4] << 8) | tmp[5];

	return 0x0;
}

int mag_read(MAG_data* mag_data){
	if (mag_read_raw(mag_data))
		return 0x1;

	mag_data->x = ((float)mag_data->raw[0] * mag_data->sens - mag_data->bias[0]) * mag_data->scale[0];
	mag_data->y = ((float)mag_data->raw[1] * mag_data->sens - mag_data->bias[1]) * mag_data->scale[1];
	mag_data->z = ((float)mag_data->raw[2] * mag_data->sens - mag_data->bias[2]) * mag_data->scale[2];

	return 0x0;
}

int magcal(MAG_data* mag_data)
{
	mag_data->scale[0] = 1.0;
	mag_data->scale[1] = 1.0;
	mag_data->scale[2] = 1.0;
	mag_data->bias[0]  = 0.0;
	mag_data->bias[1]  = 0.0;
	mag_data->bias[2]  = 0.0;

	uint16_t ii = 0, sample_count = 3000;

	float 	mag_rad[3] = {0, 0, 0},
			mag_max[3] = {-32767, -32767, -32767},
			mag_min[3] = {32767, 32767, 32767};

	delay_ms(4000);

	for(ii = 0; ii < sample_count; ii++) {

		mag_read(mag_data);  // Read the mag data

		if(mag_data->x > mag_max[0]) mag_max[0] = mag_data->x;
		if(mag_data->y > mag_max[1]) mag_max[1] = mag_data->y;
		if(mag_data->z > mag_max[2]) mag_max[2] = mag_data->z;
		if(mag_data->x < mag_min[0]) mag_min[0] = mag_data->x;
		if(mag_data->y < mag_min[1]) mag_min[1] = mag_data->y;
		if(mag_data->z < mag_min[2]) mag_min[2] = mag_data->z;

		delay_ms(5);
	}

	// Get hard iron correction
	mag_data->bias[0]  = (mag_max[0] + mag_min[0])/2;  // get average x mag bias in counts
	mag_data->bias[1]  = (mag_max[1] + mag_min[1])/2;  // get average y mag bias in counts
	mag_data->bias[2]  = (mag_max[2] + mag_min[2])/2;  // get average z mag bias in counts

	// Get soft iron correction estimate
	mag_rad[0]  = (mag_max[0] - mag_min[0])/2;  // get average x axis max chord length in counts
	mag_rad[1]  = (mag_max[1] - mag_min[1])/2;  // get average y axis max chord length in counts
	mag_rad[2]  = (mag_max[2] - mag_min[2])/2;  // get average z axis max chord length in counts

	float avg_rad = mag_rad[0] + mag_rad[1] + mag_rad[2];
	avg_rad /= 3.0;

	mag_data->scale[0] = avg_rad/((float)mag_rad[0]);
	mag_data->scale[1] = avg_rad/((float)mag_rad[1]);
	mag_data->scale[2] = avg_rad/((float)mag_rad[2]);
	mag_data->ABS = avg_rad*avg_rad;
	return 0;
}

