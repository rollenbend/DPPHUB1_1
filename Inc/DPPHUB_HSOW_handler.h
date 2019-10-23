/*
 * DPPHUB_HSOW_handler.h
 *
 *  Created on: 17 окт. 2019 г.
 *      Author: user
 */
#ifndef DPPHUB_USART_DRIVER_H_
#define DPPHUB_USART_DRIVER_H_
#pragma pack(push,1)
typedef struct {
	uint8_t syncro;
	uint8_t size;

	uint32_t DPP;
	int16_t speed;
	uint32_t timer;

	uint8_t status;
	uint8_t reply;

	int16_t MinAccXaxis;  		 // minimal acc sample by 40 pulses encoder
	int16_t MaxAccXaxis;  		 // maximum acc sample by 40 pulses encoder
	int16_t AvrAccXaxis;  		 // average acc sample by 40 pulses encoder
	int16_t AvrAccYaxis;  	// average Y axis acc sample by 40 pulses encoder == 25 sm
	int16_t AvrAccZaxis;  	// average Z axis acc sample by 40 pulses encoder == 25 sm

	int16_t MinBump;   // minimal vertical motion relatively rail level (in micrometers)
	int16_t MaxBump;   // maximum vertical motion relatively rail level (in micrometers)

	uint8_t cs;
} DPP_Frame_to_sensor;

typedef struct {
	uint8_t syncro;
	uint8_t size;
	uint32_t DPP;
	uint8_t cs;
} DPP_Frame_Classical;

typedef struct {
	int Sum_MinAccXaxis;  		// sum minimal acc sample by 40 pulses encoder
	int Sum_MaxAccXaxis;  		// sum maximum acc sample by 40 pulses encoder
	int Sum_AvrAccXaxis;  		// sum average acc sample by 40 pulses encoder
	int Sum_AvrAccYaxis;  		// sum average Y axis acc sample by 40 pulses encoder == 25 sm
	int Sum_AvrAccZaxis;  		// sum average Z axis acc sample by 40 pulses encoder == 25 sm

	int Sum_MinBump;   			// sum minimal vertical motion relatively rail level (in micrometers)
	int Sum_MaxBump;			// sum maximum vertical motion relatively rail level (in micrometers)

	int Count_Acc_Gyro_Values;	// count values
} Acc_Gyro_Values_Summ;
#pragma pack(pop)

typedef enum{
	NOT_READY,
	READY
} WAY_CODE_STATUS;

void HSOW_message_received(void);
void way_code_handler(void);
void start_dpp_receiving(UART_HandleTypeDef *huart);
void DPP_struct_init(void);

void Summ_Acc_Gyro_Values(void);
void Get_Acc_Gyro_Avr_Values(int16_t* MinAccXaxis, int16_t* MaxAccXaxis, int16_t* AvrAccXaxis,
		int16_t* AvrAccYaxis, int16_t* AvrAccZaxis, int16_t* MinBump, int16_t* MaxBump);
#endif /* DPPHUB_USART_DRIVER_H_ */
