/*
 * RA_driver.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: yaonn
 */

#ifndef RA_DRIVER_H_
#define RA_DRIVER_H_

#include "main.h"

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


#endif /* RA_DRIVER_H_ */
