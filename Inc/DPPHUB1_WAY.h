/*
 * DPPHUB1_WAY.h
 *
 *  Created on: 1 февр. 2019 г.
 *      Author: user
 */

#ifndef DPPHUB1_WAY_H_
#define DPPHUB1_WAY_H_
#pragma pack(push,1)
typedef struct {
	uint8_t syncro;
	uint8_t size;
	uint32_t DPP;

	int16_t speed;
	uint32_t timer;
	uint8_t status;
	uint8_t reply;

	int16_t MinAccXaxis;  		 // minimal acc sample by 40 pulses encoder
	int16_t MaxAccXaxis;  		 // maximum acc sample by 40 pulses encoder
	int16_t AvrAccXaxis;  		 // average acc sample by 40 pulses encoder
	int16_t AvrAccYaxis;  	// average Y axis acc sample by 40 pulses encoder == 25 sm
	int16_t AvrAccZaxis;  	// average Z axis acc sample by 40 pulses encoder == 25 sm

	int16_t MinBump;   // minimal vertical motion relatively rail level (in micrometers)
	int16_t MaxBump;   // maximum vertical motion relatively rail level (in micrometers)

	uint8_t cs;
} DPP_Frame_to_sensor;

typedef struct {
	uint8_t syncro;
	uint8_t size;
	uint32_t DPP;
	uint8_t cs;
} DPP_Frame_Classical;
#pragma pack(pop)

void DPP_struct_init(void);

#endif /* DPPHUB1_WAY_H_ */
