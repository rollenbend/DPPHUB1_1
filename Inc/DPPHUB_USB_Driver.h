/*
 * commands.h
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: user
 */

#ifndef DPPHUB_USB_DRIVER_H_
#define DPPHUB_USB_DRIVER_H_

#include "integer.h"
#pragma pack(push,1)
typedef struct{
	uint8_t syncro; 	// must be 0x53
	uint8_t size; 		// must be 4
	uint8_t code; 		// see @ref "Code_Field"
	uint8_t cs; 		// Control summ
} HUB_PROTOCOL_OUT_TYP_DEF;

typedef struct{
	uint8_t Syncro; 	// must be 0x54
	uint8_t Size; 		// 7 or 1 bytes. 7 - for opening file, 1 - for closing (without date and time)
	uint8_t Code; 		// see @ref "Code_Field"
	uint8_t Year; 		// binary format (0-99)
	uint8_t Month; 		// binary format (1-12)
	uint8_t Day; 		// binary format (1-31)
	uint8_t Hour; 		// binary format (0-23)
	uint8_t Minute; 	// binary format (0-59)
	uint8_t Seconds; 	// binary format (0-59)
	uint8_t cs; 		// Control summ
} SENSOR_PROTOCOL_IN_TYP_DEF;
#pragma pack(pop)

/* @defgroup Code_Field can be:
 * 0 - encoder work, open file in DPPHUB
 * 1 - imitation 250km/h in HSOW, open file in DPPHUB
 * 2 - imitation 125km/h in HSOW, open file in DPPHUB
 * 3 - imitation 62.5km/h in HSOW, open file in DPPHUB
 * 4 - imitation 31.25km/h in HSOW, open file in DPPHUB
 * 5 - imitation 15.6km/h in HSOW, open file in DPPHUB
 * 6 - imitation 8km/h in HSOW, open file in DPPHUB
 * 7 - imitation 4km/h in HSOW, open file in DPPHUB
 * 8 - stop encoder and timers in HSOW, close file in DPPHUB
 * */
#define STOP_HSOW_CMD		8
typedef enum {
	CMD_NONE,
	CMD_START,
	CMD_STOP
} COMMAND_STATUS;

DWORD RTC_make_fattime(void);
void HUB_out_mess_init(void);
void usb_receive(uint8_t* Buf, uint32_t Len);
void sensor_cmd_handler(void);
void Add_cmd_to_HSOW(uint8_t Code);
void Try_transm_cmd_to_HSOW(void);

#endif /* DPPHUB_USB_DRIVER_H_ */
