/*
 * sd_card_driver.h
 *
 *  Created on: 22 рту. 2019 у.
 *      Author: user
 */

#ifndef SD_CARD_DRIVER_H_
#define SD_CARD_DRIVER_H_

#include "fatfs.h"

void sd_card_init(void);
void sd_create_file(void);
void sd_write(char str);
void sd_close_file(void);

#endif /* SD_CARD_DRIVER_H_ */
