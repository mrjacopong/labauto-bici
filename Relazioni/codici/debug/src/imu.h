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
* File Name    : imu.h
* Version      : 1.00
* Description  : Header file for imu.c
* Author	   : Gruppo IMU-BIKE_2018
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 30.11.2018     1.00        First release
*******************************************************************************/


#ifndef SRC_IMU_H_
#define SRC_IMU_H_

#include "stddef.h"
#include "eMPL/inv_mpu.h"
#include "i2c.h"

typedef struct {
	float accRoll;
	float accPitch;
	float accYaw;
	float gyrRoll;
	float gyrPitch;
	float gyrYaw;
} IMU_raw;

typedef struct {
	uint16_t acc_sens;
	float gyr_sens;
} IMU_sens;

typedef struct {
	float accRoll;
	float accPitch;
	float accYaw;
	float gyrRoll;
	float gyrPitch;
	float gyrYaw;
} IMU_temp;

typedef struct{
	float Roll_Rad_Ref;
	float Pitch_Rad_Ref;
	float Yaw_Rad_Ref;
} IMU_rif;

/*******************************************************************************
* Function name: imu_init
* Description  : Dopo aver verificato varie condizioni (if),inizializza la
* 				 sensibilità dell'accellerometro e giroscopio.
* Arguments    : IMU_sens*
* Return value : 0 correct
* 				 1 error i2c
* 				 2 error mpu_init
* 				 3 error mpu_set_sensors
* 				 4 error mpu_set_sample_rate
*******************************************************************************/
int imu_init(IMU_sens*);

/*******************************************************************************
* Function name: imu_read
* Description  : Determina i valori (x,y,z) di accellerometro e giroscopio,
* 				 partendo dai valori grezzi che fornisce imu_read_raw
* Arguments    : IMU_raw*, IMU_sens*, IMU_temp*
* Return value : 0 correct
*******************************************************************************/
int imu_read(IMU_raw*, IMU_sens*, IMU_temp*);

void calibrazioneAngoliRiferimento(IMU_rif*);

#endif /* SRC_IMU_H_ */
