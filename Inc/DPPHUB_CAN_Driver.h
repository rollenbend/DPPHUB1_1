/*
 * CAN_DPPHUB_Driver.h
 *
 *  Created on: 18 дек. 2018 г.
 *      Author: user
 */

#ifndef DPPHUB_CAN_DRIVER_H_
#define DPPHUB_CAN_DRIVER_H_

#include "main.h"


typedef struct {
		uint32_t DPP;      // DPP counter
		uint32_t NowTime;  // 32-bit Timer
} DppTimSampleTypeDef;

typedef struct {
		int16_t MinAcc; 		  // minimal acc sample by 40 pulses encoder == 25 sm
		int16_t MaxAcc; 		  // maximum acc sample by 40 pulses encoder == 25 sm
		int16_t AvrAcc;  		  // average acc sample by 40 pulses encoder == 25 sm
		int16_t WagonSpeed; 	  // smooth speed (average for 200ms)
} AveragesAccTypeDef;

typedef struct {
		int16_t MinBump;		// minimal vertical motion relatively rail level (in micrometers)
		int16_t MaxBump;		// maximum vertical motion relatively rail level (in micrometers)
		int16_t AvrAccYaxis;  	// average Y axis acc sample by 40 pulses encoder == 25 sm
		int16_t AvrAccZaxis;  	// average Z axis acc sample by 40 pulses encoder == 25 sm
} BumpsTypeDef;

#define DPP_Frame_StdId          (uint32_t)0x0dd
#define Acc_Frame_StdId          (uint32_t)0x0cc
#define Bumps_Frame_StdId        (uint32_t)0x111


typedef struct {
		int16_t accdata;   // for X axis acc
		int16_t gyrodata;  // Z axis gyro
		uint32_t NowTime;  // 32-bit Timer
} AccGyroSampleTypeDef;

void DPPHUB_CAN_Init(void);


#endif /* DPPHUB_CAN_DRIVER_H_ */
