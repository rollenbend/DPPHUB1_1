/*
 * commands.h
 *
 *  Created on: 22 рту. 2019 у.
 *      Author: user
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

typedef struct {
	uint8_t syncro;
	uint8_t size;
	uint8_t code;
	uint8_t cs;
} SENSOR_COMMANDS;

void cmd_ini(void);
void usb_receive(uint8_t* Buf, uint32_t Len);

#endif /* COMMANDS_H_ */
