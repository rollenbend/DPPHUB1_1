/*
 * DPPHUB_USART_Driver.c
 *
 *  Created on: 1 февр. 2019 г.
 *      Author: user
 */
#include "main.h"

#ifdef USART_WAY_IN_ENABLE
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == RA_huart)
	{
		RA_message_received();
	}
	if (huart == HSOW_huart)
	{
		HSOW_message_received();
	}
}
#endif

void Send_classic_7byte_way_code(DPP_Frame_Classical* DPP_classic, uint32_t way_code)
{
	DPP_classic->syncro = 0x53;
	DPP_classic->size = 0x04;
	DPP_classic->DPP = way_code;
	DPP_classic->cs = 0;
	for (int i = 0; i < sizeof(DPP_classic) - 1; i++)
		DPP_classic->cs += *(uint8_t*) (&DPP_classic->syncro + i);

	HAL_UART_Transmit_DMA(Classic_huart, (uint8_t*) DPP_classic, sizeof(DPP_classic));
}
void Send_old_4byte_way_code(uint8_t* DPP_to_other, uint32_t way_code)
{
	DPP_to_other[0] = (uint8_t) way_code & 63;
	DPP_to_other[1] = ((uint8_t) (way_code >> 6) & 127) | 64;
	DPP_to_other[2] = ((uint8_t) (way_code >> 12) & 191) | 128;
	DPP_to_other[3] = (uint8_t) (way_code >> 18) | 192;

	HAL_UART_Transmit_IT(Other_huart, DPP_to_other, sizeof(DPP_to_other));
}
