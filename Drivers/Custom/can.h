/*
 * can.h
 *
 *  Created on: 18.11.2022
 *      Author: Maximilian Betz
 */

#ifndef CAN_H_
#define CAN_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "FreeRTOS.h"

#define CAN2_RX_QUEUE_LEN 100
#define CAN2_RX_TIMEOUT_MS 100
extern CAN_HandleTypeDef hcan2;


typedef struct
{
  uint16_t id;  //0 to 0x7FF
  uint8_t length;  // 0 to 8
  uint8_t data[8];
}Can_Frame;

HAL_StatusTypeDef CAN_Init(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef CAN_Send(CAN_HandleTypeDef *hcan,Can_Frame *can_frame);
HAL_StatusTypeDef CAN_Receive(CAN_HandleTypeDef *hcan, Can_Frame *can_frame);


#ifdef __cplusplus
}
#endif

#endif
