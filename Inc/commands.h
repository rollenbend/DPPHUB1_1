/*
 * commands.h
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: user
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#pragma pack(push,1)
typedef struct{
	uint8_t syncro;
	uint8_t size;
	uint8_t code;
	uint8_t cs;
} SENSOR_COMMANDS;
//typedef struct{
//	uint8_t Syncro;
//	uint8_t Size;
//	uint8_t Code;
//	uint8_t YY;
//	uint8_t MM;
//	uint8_t DD;
//	uint8_t WW;
//	uint8_t HH;
//	uint8_t mm;
//	uint8_t SS;
//	uint8_t cs;
//} SENSOR_COMMANDS_TO_HUB;
#pragma pack(pop)

void cmd_ini(void);
void usb_receive(uint8_t* Buf, uint32_t Len);

#endif /* COMMANDS_H_ */
