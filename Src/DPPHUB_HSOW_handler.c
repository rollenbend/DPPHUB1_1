/*
 * DPPHUB_HSOW_handler.c
 *
 *  Created on: 17 окт. 2019 г.
 *      Author: user
 */

#include "main.h"
#include "usbd_cdc_if.h"
static DPP_Frame_to_sensor 	DPP_frame = { 0 };			// dpp package sending with USB to Sensor PC
static DPP_Frame_Classical 	DPP_classic = { 0 };		// dpp 7-byte package sending with RS-232 to consumers
static uint8_t 				DPP_to_other[4] = { 0 };	// dpp 4-byte package sending with RS-232 to other consumers
static WAY_CODE_STATUS 		way_code = NOT_READY;		// if received new HSOW package, that flag will be set
static Acc_Gyro_Values_Summ acc_summ = { 0 };

void start_dpp_receiving(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_DMA(huart, (uint8_t*)&DPP_frame, sizeof(DPP_frame));
//	HAL_UART_Receive_IT(huart, (uint8_t*)&cmd, sizeof(cmd));
}

void HSOW_message_received(void)
{
	uint8_t cs = 0;
	for (int i = 0; i < sizeof(DPP_frame) - 1; i++)
		cs += *(uint8_t*) (&DPP_frame.syncro + i);

	if (DPP_frame.syncro == 0x53 && DPP_frame.cs == cs)
	{
		way_code = READY;
	}
}


void way_code_handler(void)
{
	if (way_code == READY)
	{
		way_code = NOT_READY;
		Try_transm_cmd_to_HSOW();
		if (Is_ra_active() != SET)
		{
			Send_classic_7byte_way_code(&DPP_classic, DPP_frame.DPP);
			Send_old_4byte_way_code(DPP_to_other, DPP_frame.DPP);
			CDC_Transmit_FS((uint8_t*) &DPP_frame, sizeof(DPP_frame));

			sd_write_in_file(&DPP_frame);
		}
		else // that means that message received, but RA way code is more
			//priority and need to give RA_handler.c Acc values
		{
			Summ_Acc_Gyro_Values();
		}
	}
}


void Summ_Acc_Gyro_Values(void)
{
	acc_summ.Sum_MinAccXaxis += (int)DPP_frame.MinAccXaxis;
	acc_summ.Sum_MaxAccXaxis += (int)DPP_frame.MaxAccXaxis;
	acc_summ.Sum_AvrAccXaxis += (int)DPP_frame.AvrAccXaxis;
	acc_summ.Sum_AvrAccYaxis += (int)DPP_frame.AvrAccYaxis;
	acc_summ.Sum_AvrAccZaxis += (int)DPP_frame.AvrAccZaxis;

	acc_summ.Sum_MinBump += (int)DPP_frame.MinBump;
	acc_summ.Sum_MaxBump += (int)DPP_frame.MaxBump;

	acc_summ.Count_Acc_Gyro_Values++;
}

void Get_Acc_Gyro_Avr_Values(int16_t* MinAccXaxis, int16_t* MaxAccXaxis, int16_t* AvrAccXaxis,
		int16_t* AvrAccYaxis, int16_t* AvrAccZaxis, int16_t* MinBump, int16_t* MaxBump)
{
	if (acc_summ.Count_Acc_Gyro_Values != 0)
	{
		*MinAccXaxis = (int16_t)(acc_summ.Sum_MinAccXaxis/acc_summ.Count_Acc_Gyro_Values);
		*MaxAccXaxis = (int16_t)(acc_summ.Sum_MaxAccXaxis/acc_summ.Count_Acc_Gyro_Values);
		*AvrAccXaxis = (int16_t)(acc_summ.Sum_AvrAccXaxis/acc_summ.Count_Acc_Gyro_Values);
		*AvrAccYaxis = (int16_t)(acc_summ.Sum_AvrAccYaxis/acc_summ.Count_Acc_Gyro_Values);
		*AvrAccZaxis = (int16_t)(acc_summ.Sum_AvrAccZaxis/acc_summ.Count_Acc_Gyro_Values);
		*MinBump = (int16_t)(acc_summ.Sum_MinBump/acc_summ.Count_Acc_Gyro_Values);
		*MaxBump = (int16_t)(acc_summ.Sum_MaxBump/acc_summ.Count_Acc_Gyro_Values);
	}
	else
	{
		*MinAccXaxis = 0;
		*MaxAccXaxis = 0;
		*AvrAccXaxis = 0;
		*AvrAccYaxis = 0;
		*AvrAccZaxis = 0;
		*MinBump = 0;
		*MaxBump = 0;
	}
	acc_summ.Count_Acc_Gyro_Values = 0;
	acc_summ.Sum_MinAccXaxis = 0;
	acc_summ.Sum_MaxAccXaxis = 0;
	acc_summ.Sum_AvrAccXaxis = 0;
	acc_summ.Sum_AvrAccYaxis = 0;
	acc_summ.Sum_AvrAccZaxis = 0;
	acc_summ.Sum_MinBump = 0;
	acc_summ.Sum_MaxBump = 0;
}

