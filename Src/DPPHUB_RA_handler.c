/*
 * RA_driver.c
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: yaonn
 */
#include "main.h"
#include "stdlib.h"
#include "usbd_cdc_if.h"
#ifdef RADIOAVEONIKA_DRV_ENABLE

static uint8_t ramessage[24] = {0}; 			// ra message for receiving
static uint8_t rarevmess[24]; 					// the same ramessage, but bytes are reversed
static RA_Frame raframe;						// ra protocol
static uint32_t lastdpp=0;						// lastdpp code for compare with new one

static WAY_CODE_STATUS ra_code = NOT_READY;		// if received new ra package, that flag will be set
static FlagStatus ra_active = RESET;			// flag mean that dpp source is Radioaveonika

static DPP_Frame_to_sensor DPP_frame;			// dpp package sending with USB to Sensor PC
static DPP_Frame_Classical DPP_classic;			// dpp 7-byte package sending with RS-232 to consumers
static uint8_t DPP_to_other[4] = { 0 };			// dpp 4-byte package sending with RS-232 to other consumers

void start_ra_receiving(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_DMA(huart, ramessage, sizeof(ramessage));
}

static int findff(uint8_t *mes) // finding syncro
{
	int i=0;
	for (i=0; i<sizeof(raframe); i++)
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
void RA_handler(void)
{
	if (ra_code == READY)
	{
		int s = 0xff;
		s = findff(ramessage); // finding start index(synchro 4 FF bytes)
		if (s != 0xff)
		{
			ra_active = SET;
			for (int i = 0; i < sizeof(raframe); i++)
			{ // reversing each byte
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

			if ((raframe.syscoord-lastdpp) > 200)
			{
				// send to users
				lastdpp=raframe.syscoord;
				Send_classic_7byte_way_code(&DPP_classic, lastdpp);
				Send_old_4byte_way_code(DPP_to_other, lastdpp);
				Send_SENSOR_way_format(&DPP_frame);
				sd_write_in_file(&DPP_frame);
			}
		}
		ra_code = NOT_READY;
	}
}

FlagStatus Is_ra_active(void)
{
	return ra_active;
}

void RA_message_received(void)
{
	ra_code = READY;
}

void Send_SENSOR_way_format(DPP_Frame_to_sensor* 	DPP_frame)
{
	DPP_frame->syncro = 0x53;
	DPP_frame->size = sizeof(DPP_Frame_to_sensor)-3;
	DPP_frame->DPP = lastdpp;
	DPP_frame->speed = raframe.velocity;
	Get_Acc_Gyro_Avr_Values(&DPP_frame->MinAccXaxis, &DPP_frame->MaxAccXaxis,
			&DPP_frame->AvrAccXaxis, &DPP_frame->AvrAccYaxis,
			&DPP_frame->AvrAccZaxis, &DPP_frame->MinBump, &DPP_frame->MaxBump);
	uint8_t cs = 0;
	for (int i = 0; i < sizeof(DPP_Frame_to_sensor) - 1; i++)
		cs += *(uint8_t*) (&DPP_frame->syncro + i);
	DPP_frame->cs = cs;
	CDC_Transmit_FS((uint8_t*) DPP_frame, sizeof(DPP_Frame_to_sensor));
}
#endif

