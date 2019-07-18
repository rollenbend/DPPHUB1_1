/*
 * DPPHUB1_WAY.c
 *
 *  Created on: 1 февр. 2019 г.
 *      Author: user
 */

#include "main.h"

DPP_Frame_to_sensor DPP_frame;
DPP_Frame_Classical DPP_classic;
uint8_t DPP_to_other[4] = {0};

void start_dpp_receiving(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(USART1_DE_GPIO_Port, USART1_DE_Pin, DISABLE);
	HAL_UART_Receive_DMA(huart, (uint8_t*)&DPP_frame, sizeof(DPP_frame));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	uint8_t cs = 0;
	for (int i = 0; i < sizeof(DPP_frame) - 1; i++)
		cs += *(uint8_t*) (&DPP_frame.syncro + i);

	if (cs == DPP_frame.cs)
	{

		DPP_classic.DPP = DPP_frame.DPP;
		DPP_classic.cs = 0;
		for (int i = 0; i < sizeof(DPP_classic) - 1; i++)
			DPP_classic.cs += *(uint8_t*) (&DPP_classic.syncro + i);

		DPP_to_other[0] = (uint8_t) DPP_frame.DPP & 63;
		DPP_to_other[1] = ((uint8_t) (DPP_frame.DPP >> 6) & 127) | 64;
		DPP_to_other[2] = ((uint8_t) (DPP_frame.DPP >> 12) & 191) | 128;
		DPP_to_other[3] = (uint8_t) (DPP_frame.DPP >> 18) | 192;

		CDC_Transmit_FS((uint8_t*) &DPP_frame, sizeof(DPP_frame));

		HAL_UART_Transmit_DMA(&huart3, (uint8_t*) &DPP_classic, sizeof(DPP_classic));
		HAL_UART_Transmit_IT(&huart6, (uint8_t*) &DPP_to_other, sizeof(DPP_to_other));
	}
}


void DPP_struct_init(void)
{
	DPP_frame.syncro = 0x53;
	DPP_frame.size = sizeof(DPP_frame)-3;
	DPP_frame.DPP = 0;

	DPP_frame.speed = 0;
	DPP_frame.timer = 0;
	DPP_frame.status = 0;
	DPP_frame.reply = 0;

	DPP_frame.MinAccXaxis = 0;
	DPP_frame.MaxAccXaxis = 0;
	DPP_frame.AvrAccXaxis = 0;
	DPP_frame.AvrAccYaxis = 0;
	DPP_frame.AvrAccZaxis = 0;

	DPP_frame.MinBump = 0;
	DPP_frame.MaxBump = 0;
	DPP_frame.cs = 0;

	DPP_classic.syncro = 0x53;
	DPP_classic.size = 0x04;
	DPP_classic.DPP = 0;
	DPP_classic.cs = 0;

	cmd_ini();
}
