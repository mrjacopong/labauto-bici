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
* File Name    : imu.c
* Version      : 1.00
* Description  : Gestione e lettura accellerometro e giroscopio
* Author	   : Gruppo IMU-BIKE_2018
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 30.11.2018     1.00        First release
*******************************************************************************/

#include <imu.h>

int imu_read(IMU_raw* imu_raw, IMU_sens* imu_sens, IMU_temp* imu_temp){

	short data_acc[3] = {imu_raw->accRoll,imu_raw->accPitch,imu_raw->accYaw};
	mpu_get_accel_reg(data_acc, NULL);
	imu_raw->accRoll = data_acc[0];
	imu_raw->accPitch = data_acc[1];
	imu_raw->accYaw = data_acc[2];

	short data_gyr[3] = {imu_raw->gyrRoll, imu_raw->gyrPitch, imu_raw->gyrYaw};
	mpu_get_gyro_reg(data_gyr, NULL);
	imu_raw->gyrRoll = data_gyr[0];
	imu_raw->gyrPitch = data_gyr[1];
	imu_raw->gyrYaw = data_gyr[2];

	imu_temp->accRoll=imu_raw->accRoll / imu_sens->acc_sens;
	imu_temp->gyrRoll = imu_raw->gyrRoll / imu_sens->gyr_sens * 0.01745329252;
	imu_temp->accPitch = imu_raw->accPitch / imu_sens->acc_sens;
	imu_temp->gyrPitch = imu_raw->gyrPitch / imu_sens->gyr_sens * 0.01745329252;
	imu_temp->accYaw = imu_raw->accYaw / imu_sens->acc_sens;
	imu_temp->gyrYaw = imu_raw->gyrYaw / imu_sens->gyr_sens * 0.01745329252;
	return 0;
}



int imu_init(IMU_sens* imu_sens){
	if (i2c_init())
		return 0x1;
	if (mpu_init(0))
		return 0x2;
	if (mpu_set_sensors(INV_XYZ_ACCEL | INV_XYZ_GYRO))
		return 0x3;
	if (mpu_set_sample_rate(1000)) //Hz
		return 0x4;
	mpu_get_accel_sens(&imu_sens->acc_sens);
	mpu_get_gyro_sens(&imu_sens->gyr_sens);
	return 0x0;
}

void calibrazioneAngoliRiferimento(IMU_rif* imu_rif){
	// nel caso di errore eccessivo della imu in quanto posizione, effettuare filtro qui
}
