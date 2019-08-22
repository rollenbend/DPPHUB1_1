/*
 * sd_card_driver.c
 *
 *  Created on: 22 рту. 2019 у.
 *      Author: user
 */

#include "sd_card_driver.h"

extern char SDPath[4];

	char str[64];
//	int DirNumber=1;
	int FileNumber = 1;
	int count=0;

	FATFS fileSystem;
	FIL testFile;
	UINT testBytes;
//	FRESULT res;

	  //  do sprintf(str, "%04d", DirNumber++); while (f_mkdir (str)!=FR_OK);
	  //  f_chdir (str);
void sd_card_init(void)
{
	  f_mount(&fileSystem, SDPath, 1);
}

void sd_create_file(void)
{
	  sprintf(str, "FILE_%03d.txt", FileNumber++);
	  f_open(&testFile, str, FA_WRITE | FA_CREATE_ALWAYS);
}

void sd_write(char str)
{
	sprintf(str, "%6d \n", count++);
	if (count <= 10)
	{
		f_write(&testFile, str, strlen(str), &testBytes);
	}
	if (count == 10) f_close(&testFile);
}

void sd_close_file(void)
{
	f_close(&testFile);
}
