/*
 * DPPHUB_USART_Driver.h
 *
 *  Created on: 1 февр. 2019 г.
 *      Author: user
 */


#ifndef DPPHUB_HSOW_HANDLER_H_
#define DPPHUB_HSOW_HANDLER_H_

#include <DPPHUB_HSOW_handler.h>
void Send_classic_7byte_way_code(DPP_Frame_Classical* DPP_classic, uint32_t way_code);
void Send_old_4byte_way_code(uint8_t* DPP_to_other, uint32_t way_code);

#endif /* DPPHUB_HSOW_HANDLER_H_ */

