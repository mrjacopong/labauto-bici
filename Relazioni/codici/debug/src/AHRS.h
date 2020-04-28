#ifndef AHRS_h
#define AHRS_h

#include <imu.h>

#include <math.h>
#include <mathf.h>
#include <stdio.h>
#include "platform.h"
#include "mag.h"

typedef struct{
	float RollRad;        // Corrisponde all'uscita in radianti dell'asse x di AHRS
	float PitchRad;       // Corrisponde all'uscita in radianti dell'asse y di AHRS
	float YawRad;         // Corrisponde all'uscita in radianti dell'asse z di AHRS
	float RollDeg;        // Corrisponde all'uscita in gradi dell'asse x di AHRS
	float PitchDeg;       // Corrisponde all'uscita in gradi dell'asse y di AHRS
	float YawDeg;         // Corrisponde all'uscita in gradi dell'asse z di AHRS
	float omegaRollRad;        // Corrisponde all'uscita in radianti dell'asse x di AHRS (velocità angolare)
	float omegaPitchRad;       // Corrisponde all'uscita in radianti dell'asse y di AHRS (velocità angolare)
	float omegaYawRad;         // Corrisponde all'uscita in radianti dell'asse z di AHRS (velocità angolare)
	float omegaRollDeg;        // Corrisponde all'uscita in gradi dell'asse x di AHRS (velocità angolare)
	float omegaPitchDeg;       // Corrisponde all'uscita in gradi dell'asse y di AHRS (velocità angolare)
	float omegaYawDeg;         // Corrisponde all'uscita in gradi dell'asse z di AHRS (velocità angolare)
}AHRS_data;

/*
Il filtro ha bisogno di sapere la frequenza con cui sarà chiamata la funzione
madgwickFilterUpdate(...)
Prima di tutto va impostato questo valore
*/
void setAHRSFrequency(float f);

/*
Questa funzione ricalcola l'assetto facendo uso di nuovi dati forniti dall'IMU.
Per avere una buona stima dell'assetto è ideale chiamare questa funzione con la
frequenza più alta possibile.
*/
void madgwickFilterUpdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

/*
Questa funzione ricava gli angoli di Tait-Bryan.
I valori vengono scritti nelle variabili passate per riferimento.
Questa funzione può essere chiamata con una frequenza più bassa di quella con
cui viene aggiornato il filtro, per esempio alla frequenza dell'algoritmo di
controllo.
*/
void getAHRS(float* pitch, float* yaw, float* roll);
void getQuat(float* qa, float* qb, float* qc, float* qd);
void getYPR(MAG_data*, IMU_temp*, AHRS_data*);
void calibrationYPR(char*, MAG_data*);


#endif
