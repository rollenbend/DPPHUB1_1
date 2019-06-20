/*
 * CAN_DPPHUB_Driver.c
 *
 *  Created on: 18 дек. 2018 г.
 *      Author: ilya
 */
#include "DPPHUB1_CAN_Driver.h"

uint32_t LED_Turn_Count=0;
int Sent_USB = 0;
extern DPP_Frame_to_sensor DPP_frame_to_sensor;
extern DPP_Frame_Classical DPP_classic;

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
	DppTimSampleTypeDef  DppStruct;
	AveragesAccTypeDef   AverageAcc;
	BumpsTypeDef         Bumps;
	uint32_t rxfifo =    CAN_RX_FIFO0;
	CAN_RxHeaderTypeDef  rxheader;
	uint8_t rxdata[8] =  {0};
	uint8_t usbresult =  1;
	uint8_t DPP_to_other[4] = {0};

	for (int i = 0; i < 3; i++) {
		HAL_CAN_GetRxMessage(&hcan1, rxfifo, &rxheader, rxdata);
		switch (rxheader.StdId) {
		case DPP_Frame_StdId :
			DppStruct.DPP     = *(uint32_t*) (rxdata + 0);
			DppStruct.NowTime = *(uint32_t*) (rxdata + 4);
			break;
		case Acc_Frame_StdId :
			AverageAcc.MinAcc	  = *(int16_t*) (rxdata + 0);
			AverageAcc.MaxAcc     = *(int16_t*) (rxdata + 2);
			AverageAcc.AvrAcc     = *(int16_t*) (rxdata + 4);
			AverageAcc.WagonSpeed = *(int16_t*) (rxdata + 6);
			break;
		case Bumps_Frame_StdId :
			Bumps.MinBump 		= *(int16_t*) (rxdata + 0);
			Bumps.MaxBump 		= *(int16_t*) (rxdata + 2);
			Bumps.AvrAccYaxis 	= *(int16_t*) (rxdata + 4);
			Bumps.AvrAccZaxis 	= *(int16_t*) (rxdata + 6);
			break;
		}
	}

	DPP_classic.DPP = DppStruct.DPP;
	DPP_classic.cs = 0;
	for (int i=0; i<sizeof(DPP_classic)-1; i++) DPP_classic.cs += *(uint8_t*)(&DPP_classic.syncro+i);

	DPP_frame_to_sensor.DPP = DppStruct.DPP;
	DPP_frame_to_sensor.timer = DppStruct.NowTime;
	DPP_frame_to_sensor.MinAccXaxis = AverageAcc.MinAcc;
	DPP_frame_to_sensor.MaxAccXaxis = AverageAcc.MaxAcc;
	DPP_frame_to_sensor.AvrAccXaxis = AverageAcc.AvrAcc;
	DPP_frame_to_sensor.AvrAccYaxis = Bumps.AvrAccYaxis;
	DPP_frame_to_sensor.AvrAccZaxis = Bumps.AvrAccZaxis;
	DPP_frame_to_sensor.speed = AverageAcc.WagonSpeed;
	DPP_frame_to_sensor.MinBump = Bumps.MinBump;
	DPP_frame_to_sensor.MaxBump = Bumps.MaxBump;
	DPP_frame_to_sensor.cs = 0;
	for (int i=0; i<sizeof(DPP_frame_to_sensor)-1; i++) DPP_frame_to_sensor.cs += *(uint8_t*)(&DPP_frame_to_sensor.syncro+i);

	DPP_to_other[0] = (uint8_t)DppStruct.DPP & 63;
	DPP_to_other[1] = ((uint8_t)(DppStruct.DPP >> 6) & 127) | 64;
	DPP_to_other[2] = ((uint8_t)(DppStruct.DPP >> 12) & 191) | 128;
	DPP_to_other[3] = (uint8_t)(DppStruct.DPP >> 18) | 192;

	usbresult = CDC_Transmit_FS((uint8_t*) &DPP_frame_to_sensor, sizeof(DPP_frame_to_sensor));
		if (!usbresult) HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11); // green

	HAL_UART_Transmit_DMA(&huart3, (uint8_t*) &DPP_classic, sizeof(DPP_classic));
	HAL_UART_Transmit_DMA(&huart6, (uint8_t*) &DPP_to_other, sizeof(DPP_to_other));
}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

}
void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan)
{

}
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	hcan->ErrorCode;
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15); // red
}


void DPPHUB_CAN_InitFilter(void)
{
	CAN_FilterTypeDef filterconfig;
	filterconfig.FilterIdHigh = 0;
	filterconfig.FilterIdLow = 0;
	filterconfig.FilterMaskIdHigh = 0;
	filterconfig.FilterMaskIdLow = 0;
	filterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filterconfig.FilterBank = 0;
	filterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	filterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	filterconfig.FilterActivation = ENABLE;

	HAL_CAN_ConfigFilter(&hcan1, &filterconfig);

//	filterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
//	HAL_CAN_ConfigFilter(&hcan1, &filterconfig);
}
void DPPHUB_CAN_Init(void)
{
	DPPHUB_CAN_InitFilter();
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO0_FULL);
	HAL_CAN_Start(&hcan1);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//	DppTimSampleTypeDef DppStruct;
//	AveragesAccTypeDef  AverageAcc;
//	BumpsTypeDef        Bumps;
//	uint32_t rxfifo=CAN_RX_FIFO0;
//	CAN_RxHeaderTypeDef rxheader;
//	char strout[60]={0};
//	uint8_t rxdata[8]={0};
//	uint8_t usbresult = 1;
//
//	HAL_CAN_GetRxMessage(&hcan1, rxfifo, &rxheader, rxdata);
//	if (rxheader.StdId == DPP_Frame_StdId){
////		Sent_USB++;
////		DppStruct.DPP = *(uint32_t*)rxdata;
////		DppStruct.NowTime = *(uint32_t*)(rxdata+4);
////		//for (int i=0; i<60; i++) strout[i]=0;
////		if (Sent_USB%100) {
////			sprintf(strout, "%9d %9u\r\n",(int)DppStruct.DPP,(unsigned int)DppStruct.NowTime);
////			usbresult = CDC_Transmit_FS((uint8_t*)strout, sizeof(strout));
////		}
//	}
////	if (rxheader.StdId == Acc_Frame_StdId){
////		Sent_USB++;
////		AverageAcc.MinAcc = *(int16_t*)(rxdata+0);
////		AverageAcc.MaxAcc = *(int16_t*)(rxdata+2);
////		AverageAcc.AvrAcc = *(int16_t*)(rxdata+4);
//////		for (int i=0; i<60; i++) strout[i]=0;
//////		sprintf(strout, "%6d %6d %6d\r\n",(int)AverageAcc.MinAcc, (int)AverageAcc.MaxAcc, (int)AverageAcc.AvrAcc);
//////		usbresult = CDC_Transmit_FS((uint8_t*)strout, sizeof(strout));
////	}
//	if (rxheader.StdId == Bumps_Frame_StdId){
//		Sent_USB++;
//		Bumps.MinBump = *(int16_t*)(rxdata+0);
//		Bumps.MaxBump = *(int16_t*)(rxdata+2);
//		if (Sent_USB % 100) {
//			sprintf(strout, "%7d\r\n", (int) Bumps.MaxBump);
//			usbresult = CDC_Transmit_FS((uint8_t*) strout, sizeof(strout));
//		}
////		sprintf(strout, "%8d",(int)DppStruct.DPP);
////		usbresult = CDC_Transmit_FS((uint8_t*)strout, sizeof(strout));
//	}
//
////	if (Sent_USB == 3) {
////		if (++LED_Turn_Count%1==0) HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13); // orange
////		sprintf(strout, "%8d %6d %6d %6d %10u\r\n",
////				(int)DppStruct.DPP,
////				(int)AverageAcc.MinAcc, (int)AverageAcc.MaxAcc, (int)AverageAcc.AvrAcc, (unsigned int)DppStruct.NowTime);
////				//(int)Bumps.MinBump,(int)Bumps.MaxBump,(unsigned int)DppStruct.NowTime);
////		usbresult = CDC_Transmit_FS((uint8_t*)strout, sizeof(strout));
////		if (!usbresult)
////			if (LED_Turn_Count%1==0) HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); // green
////		Sent_USB=0;
////	}
}








