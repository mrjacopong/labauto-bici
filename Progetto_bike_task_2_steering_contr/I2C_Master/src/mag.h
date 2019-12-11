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
/******************************************************************************
* File Name     : mag.h
* Version       : 1.0
* Description   : Header file for mag.c
* Author		: Gruppo IMU-BIKE_2018
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 30.11.2018     1.00        First release
*******************************************************************************/

#ifndef SRC_MAG_H_
#define SRC_MAG_H_

#include "i2c.h"
#include "CMT.h"
#include "r_riic_rx600_master.h"

#ifdef DROTEK_IMU_10DOF_V2
#include "eMPL/inv_mpu.h"
#endif

typedef struct {
	short raw[3];
	float x,y,z;
	float sens;
	float bias[3];
	float scale[3];
	float ABS;
} MAG_data;

/*******************************************************************************
* Function name: mag_init
* Description  : Inizializza il magnetometro, settando i parametri per la lettura
* 				 dei valori. Richiama i2c_init() per verificare se il protocollo di
* 				 comunicazione è correttamete inizializzato. Valorizza i valori di
* 				 bias e sens della struttura mag, che serviranno per calcolare l'effettivo
* 				 valore del magnetometro.
* Arguments    : MAG_data*
* Return value : 0 correct
* 				 1 error
* 				 2 error
* 				 3 error
* 				 4 error
* 				 5 error
* 				 6 error
*******************************************************************************/
int mag_init(MAG_data*);
/*******************************************************************************
* Function name: mag_read_raw
* Description  : Legge i valori grezzi del magnetometro sfruttando il protocollo i2c.
* Arguments    : MAG_data*
* Return value : 0 correct
* 				 1 error i2c_read
*******************************************************************************/
int mag_read_raw(MAG_data*);
/*******************************************************************************
* Function name: mag_read
* Description  : Richiama mag_read_raw e determina a partire dai valori grezzi, i valori coerenti.
* Arguments    : MAG_data*
* Return value : 0 correct
* 				 1 error mag_read_raw
*******************************************************************************/
int mag_read(MAG_data*);
/*******************************************************************************
* Function name: mag_cal
* Description  : Calibra il magnetometro. Legge i valori dei campi magnetici nei dintorni del sensore.
* 				 Calcola dunque con delle medie, i valori di tali somme e li usa per correggere il valore reale.
* Arguments    : MAG_data*
* Return value : 0 sempre
*******************************************************************************/
int magcal(MAG_data*);


#endif /* SRC_MAG_H_ */
