/*
 * DPPHUB1_WAY.c
 *
 *  Created on: 1 февр. 2019 г.
 *      Author: user
 */

#include "main.h"

DPP_Frame_to_sensor DPP_frame_to_sensor;
DPP_Frame_Classical DPP_classic;

void DPP_struct_init(void)
{
	DPP_frame_to_sensor.syncro = 0x53;
	DPP_frame_to_sensor.size = sizeof(DPP_frame_to_sensor)-3;
	DPP_frame_to_sensor.DPP = 0;

	DPP_frame_to_sensor.speed = 0;
	DPP_frame_to_sensor.timer = 0;
	DPP_frame_to_sensor.status = 0;
	DPP_frame_to_sensor.reply = 0;

	DPP_frame_to_sensor.MinAccXaxis = 0;
	DPP_frame_to_sensor.MaxAccXaxis = 0;
	DPP_frame_to_sensor.AvrAccXaxis = 0;
	DPP_frame_to_sensor.AvrAccYaxis = 0;
	DPP_frame_to_sensor.AvrAccZaxis = 0;

	DPP_frame_to_sensor.MinBump = 0;
	DPP_frame_to_sensor.MaxBump = 0;
	DPP_frame_to_sensor.cs = 0;

	DPP_classic.syncro = 0x53;
	DPP_classic.size = 0x04;
	DPP_classic.DPP = 0;
	DPP_classic.cs = 0;
}
