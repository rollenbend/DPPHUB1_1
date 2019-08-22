/*
 * RA_driver.c
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: yaonn
 */
#ifdef RA_DRIVER

#include "RA_driver.h"
extern UART_HandleTypeDef huart1;
uint8_t ramessage[24];
uint8_t rarevmess[24]; // the same ramessage, but bytes are reversed
RA_Frame raframe;
uint32_t lastdpp=0;

static int findff(uint8_t *mes) // finding syncro
{
	int i=0;
	for (i=0; i<24; i++)
	{
		if ((mes[(i+1)%24] & mes[(i+2)%24] & mes[(i+3)%24] & mes[(i+4)%24]) == 0xFF)
			return i;
	}
	return 0xff;
}

static uint8_t reversebyte(uint8_t bytetoreverse) //change lsb and msb in byte
{
	uint8_t byte=0;
	for (int i=0; i<8; i++)
	{
		if (bytetoreverse & (1<<i)) byte |= 1<<(8-i);
	}
	return byte;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		int s = 0xff;
		s = findff(ramessage); // finding start index(synchro 4 FF bytes)
		if (s != 0xff) {
			for (int i = 0; i < 24; i++) { // reversing each byte
				rarevmess[i] = reversebyte(ramessage[(i + s) % 24]);
			}

			raframe.flags = rarevmess[5];
			raframe.velocity = *(int16_t*) (rarevmess + 6);
			raframe.curcoord = *(int32_t*) (rarevmess + 8);
			raframe.syscoord = *(int32_t*) (rarevmess + 12);
			raframe.frameflags = rarevmess[16];
			raframe.mframecounter[0] = rarevmess[17];
			raframe.mframecounter[1] = rarevmess[18];
			raframe.mframecounter[2] = rarevmess[19];
			raframe.firstframecoord = *(int32_t*) (rarevmess + 20);

			if (raframe.syscoord-lastdpp>250)
			{
				// send to users
				lastdpp=raframe.syscoord;
			}
		}
	}
}



void start_ra_receiving(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_DMA(huart, ramessage, sizeof(ramessage));
}

#endif

