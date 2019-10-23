/*
 * RA_driver.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: yaonn
 */

#ifndef DPPHUB_RA_HANDLER_H_
#define DPPHUB_RA_HANDLER_H_

#include "main.h"

#pragma pack(push,1)
typedef struct {
	uint8_t separator;
	uint32_t sync;
	uint8_t flags; // needed us
	int16_t velocity; // needed us
	uint32_t curcoord;
	uint32_t syscoord; // needed us
	uint8_t frameflags;
	uint8_t mframecounter[3];
	uint32_t firstframecoord;
} RA_Frame;
#pragma pack(pop)

void start_ra_receiving(UART_HandleTypeDef *huart);
void RA_handler(void);
FlagStatus Is_ra_active(void);
void RA_message_received(void);
void Send_SENSOR_way_format(DPP_Frame_to_sensor* 	DPP_frame);
#endif /* DPPHUB_RA_HANDLER_H_ */
