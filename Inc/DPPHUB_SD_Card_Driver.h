/*
 * sd_card_driver.h
 *
 *  Created on: 1 сент. 2019 г.
 *      Author: yaonn
 */

#ifndef DPPHUB_SD_CARD_DRIVER_H_
#define DPPHUB_SD_CARD_DRIVER_H_
#include "integer.h"
#include "main.h"
DWORD RTC_make_fattime(void);

void sd_ini(void);
void sd_file_create(RTC_TimeTypeDef* sTime, RTC_DateTypeDef* sDate);
void sd_write_in_file(DPP_Frame_to_sensor* 	DPP_frame);
void sd_file_close(void);

#endif /* DPPHUB_SD_CARD_DRIVER_H_ */
