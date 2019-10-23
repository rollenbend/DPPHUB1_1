/*
 * sd_card_driver.c
 *
 *  Created on: 1 сент. 2019 г.
 *      Author: yaonn
 */
#include "main.h"
#ifdef SD_CARD_ENABLE
#include <DPPHUB_SD_Card_Driver.h>
#include "fatfs.h"
#include "string.h"

extern char SDPath[4];

FATFS fileSystem;
FIL testFile;
UINT testBytes;
FRESULT res;

void sd_ini(void)
{
	f_mount(&fileSystem, SDPath, 1);
}

void sd_file_create(RTC_TimeTypeDef* sTime, RTC_DateTypeDef* sDate)
{
	char FileName[255];
	sprintf(FileName, "hub_%02xh%02xm%02xs_%02xd%02xm20%2xy.csv",
				sTime->Hours, sTime->Minutes, sTime->Seconds,
				sDate->Date, sDate->Month, sDate->Year);
	res = f_open(&testFile, FileName, FA_WRITE | FA_CREATE_NEW);
	if (res == FR_OK)
	{
		char headline[] = { "	DPP		Speed	Timer	AccX	AccY	AccZ	AccXmin		AccXmax		BumpMin		BumpMax\n" };
		f_write(&testFile, headline, strlen(headline), &testBytes);
	}
}

void sd_write_in_file(DPP_Frame_to_sensor* 	DPP_frame)
{
	char DPPdata[100];
	sprintf(DPPdata, "%8d%5d%9d%5d%5d%5d%5d%5d%5d%5d\n", (uint)DPP_frame->DPP,
			DPP_frame->speed, (uint)DPP_frame->timer, DPP_frame->AvrAccXaxis,
			DPP_frame->AvrAccYaxis, DPP_frame->AvrAccZaxis,
			DPP_frame->MinAccXaxis, DPP_frame->MaxAccXaxis,
			DPP_frame->MinBump, DPP_frame->MaxBump);
	res = f_write(&testFile, DPPdata, strlen(DPPdata), &testBytes);
}

void sd_file_close(void)
{
	res = f_close(&testFile); if (res!=FR_OK) {;}
}
#endif
