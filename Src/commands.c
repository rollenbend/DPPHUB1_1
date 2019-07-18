/*
 * commands.c
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: user
 */

#include "main.h"

SENSOR_COMMANDS cmd;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
int IsFileOpen = 0;

void usb_receive(uint8_t* Buf, uint32_t Len)
{
	uint8_t cs = 0;
	for (int i = 0; i < (Len - 1); i++)
		cs += Buf[i];

	if (Buf[0] == 0x54 && cs == Buf[Len - 1])
	{
		HAL_GPIO_WritePin(USART1_DE_GPIO_Port, USART1_DE_Pin, ENABLE);
		if (Len == 4)
		{
			//need close file
		}
		if (Len == 11)
		{
			cmd.code = Buf[2];
			cmd.cs = cmd.syncro + cmd.size + cmd.code;
			HAL_UART_Transmit_DMA(&huart1, (uint8_t*) &cmd, sizeof(cmd));

			RTC_TimeTypeDef sTime =
			{ 0 };
			RTC_DateTypeDef sDate =
			{ 0 };

			sTime.Hours = Buf[9];
			sTime.Minutes = Buf[8];
			sTime.Seconds = Buf[7];
			sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
			sTime.StoreOperation = RTC_STOREOPERATION_RESET;
			sDate.WeekDay = Buf[6];
			sDate.Month = Buf[5];
			sDate.Date = Buf[4];
			sDate.Year = Buf[3];
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
			// create file
		}
		HAL_UART_Transmit_DMA(&huart1, (uint8_t*) &cmd, sizeof(cmd));
		HAL_GPIO_WritePin(USART1_DE_GPIO_Port, USART1_DE_Pin, DISABLE);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		HAL_GPIO_WritePin(USART1_DE_GPIO_Port, USART1_DE_Pin, DISABLE);
	}
}
void cmd_ini(void)
{
	cmd.syncro = 0x53;
	cmd.size = 1;
	cmd.cs = 0;
}
