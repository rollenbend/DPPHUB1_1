/*
 * commands.c
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: user
 */

#include <DPPHUB_SD_Card_Driver.h>
#include "main.h"
#include <stdio.h>

extern RTC_HandleTypeDef hrtc;

HUB_PROTOCOL_OUT_TYP_DEF 	hub_out = {0};
SENSOR_PROTOCOL_IN_TYP_DEF 	sens_in = {0};
COMMAND_STATUS 				cmd_status = CMD_NONE;
COMMAND_STATUS				last_cmd_status = CMD_NONE;
FlagStatus Need_to_transmit_cmd = RESET;

void usb_receive(uint8_t* Buf, uint32_t Len)
{
	uint8_t cs = 0;
	for (int i = 0; i < (Len - 1); i++)
		cs += Buf[i];

	if (Buf[0] == 0x53 && cs == Buf[Len - 1])
	{
		if (Len == sizeof(hub_out))
		{
			cmd_status = CMD_STOP;
		}
		if (Len == sizeof(sens_in))
		{
			for (int i = 0; i < 10; i++) *((uint8_t*)(&sens_in.Syncro + i)) = Buf[i];
			cmd_status = CMD_START;
		}
	}
}

void sensor_cmd_handler(void)
{
	if (cmd_status == CMD_START)
	{
		if (last_cmd_status == CMD_START) sd_file_close(); // if twice want open file

		/* creation file */
		RTC_TimeTypeDef sTime = { 0 };
		RTC_DateTypeDef sDate = { 0 };

		sTime.Hours = sens_in.Hour;
		sTime.Minutes = sens_in.Minute;
		sTime.Seconds = sens_in.Seconds;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		sDate.Month = sens_in.Month;
		sDate.Date = sens_in.Day;
		sDate.Year = sens_in.Year;
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		sd_file_create(&sTime, &sDate);

		Add_cmd_to_HSOW(sens_in.Code);

		start_ra_receiving(RA_huart);
		start_dpp_receiving(HSOW_huart);

		last_cmd_status = cmd_status;
		cmd_status = CMD_NONE;
	}
	if (cmd_status == CMD_STOP)
	{
		Add_cmd_to_HSOW(STOP_HSOW_CMD);
		sd_file_close();
		HAL_UART_DMAStop(RA_huart);
		HAL_UART_DMAStop(HSOW_huart);
		last_cmd_status = cmd_status;
		cmd_status = CMD_NONE;
	}
}
/*
 * Add cmd to queue. Transmitting will be when
 * RS-485 channel will be free
 */
void Add_cmd_to_HSOW(uint8_t Code)
{
	Need_to_transmit_cmd = SET;

	hub_out.syncro = 0x53;
	hub_out.size = 1;
	hub_out.code = Code;
	hub_out.cs = hub_out.syncro + hub_out.size + hub_out.code;

//	if (last_cmd_status != CMD_START)
//	{
		Try_transm_cmd_to_HSOW();
//	}
}
/*
 * We can transmit cmd to HSOW only if RS-485 channel is free.
 * Because of this we transmit cmd immediately AFTER receiving package
 * from HSOW
 */
void Try_transm_cmd_to_HSOW(void)
{
	if (Need_to_transmit_cmd == SET)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, ENABLE);
		HAL_UART_Transmit_IT(HSOW_huart, (uint8_t*) &hub_out.syncro, 4);
		Need_to_transmit_cmd = RESET;
	}
}
DWORD RTC_make_fattime(void)
{
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	return ((DWORD) (sDate.Year + 20) << 25 | (DWORD) sDate.Month << 21 | (DWORD) sDate.Date << 16 |
			(DWORD) sTime.Hours << 11 | (DWORD) sTime.Minutes << 5 | (DWORD) (sTime.Seconds / 2));
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == HSOW_huart)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, DISABLE);
	}
}

