#include "can.h"
#include "main.h"
#include "queue.h"

QueueHandle_t CAN2_rxQueue;

HAL_StatusTypeDef CAN_Init(CAN_HandleTypeDef *hcan)
{
  CAN2_rxQueue = xQueueCreate(CAN2_RX_QUEUE_LEN, sizeof(Can_Frame));
  if (CAN2_rxQueue == 0)
    {
      Error_Handler();
    }

  /*Receive all messages on fifo 0, do not filter yet*/
  CAN_FilterTypeDef sFilterConfig;
  sFilterConfig.FilterIdHigh = 0xFFFF;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0xFFFF;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
  sFilterConfig.SlaveStartFilterBank = 0;

  if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig)!= HAL_OK)
    {
      Error_Handler();
    }

  if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING)!= HAL_OK)
    {
      Error_Handler();
    }

  if (HAL_CAN_Start(&hcan2) != HAL_OK)
    {
      Error_Handler();
    }

  return HAL_OK;
}

HAL_StatusTypeDef CAN_Send(CAN_HandleTypeDef *hcan,Can_Frame *can_frame)
{
  CAN_TxHeaderTypeDef canHeader;
  canHeader.StdId = (uint32_t)can_frame->id;
  canHeader.ExtId = 0;
  canHeader.IDE = CAN_ID_STD;
  canHeader.RTR = CAN_RTR_DATA;
  canHeader.DLC = can_frame->length;

  if(hcan->Instance == CAN2)
    {
      return HAL_CAN_AddTxMessage(&hcan2, &canHeader, can_frame->data, 0 );
    }

  return HAL_ERROR;
}

HAL_StatusTypeDef CAN_Receive(CAN_HandleTypeDef *hcan, Can_Frame *can_frame)
{
  if(hcan->Instance == CAN2)
    {
      if (xQueueReceive(CAN2_rxQueue, can_frame, CAN2_RX_TIMEOUT_MS) == pdTRUE)
	{
	  return HAL_OK;
	}
      else
	{
	  return HAL_TIMEOUT;
	}
    }

  return HAL_ERROR;
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{

}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{

}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{

}

void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan)
{

}

void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan)
{

}

void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan)
{

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  BaseType_t xHigherPriorityTaskWokenByPost = pdFALSE;
  CAN_RxHeaderTypeDef rxfifo0_header;
  uint8_t rxfifo0_rxData[8];

  if(hcan->Instance == CAN2)
    {
      HAL_CAN_GetRxMessage(&hcan2, 0, &rxfifo0_header, rxfifo0_rxData);

      Can_Frame can_frame;
      can_frame.id = (uint16_t)rxfifo0_header.StdId;
      can_frame.length = rxfifo0_header.DLC;

      for (int i = 0; i < can_frame.length; i++)
	{
	  can_frame.data[i] = rxfifo0_rxData[i];
	}
      if( xQueueSendFromISR(CAN2_rxQueue, &can_frame, &xHigherPriorityTaskWokenByPost) != pdTRUE)
	{
	  Error_Handler();
	}
    }
  portYIELD_FROM_ISR(xHigherPriorityTaskWokenByPost);
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
  Error_Handler();
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  Error_Handler();
}

void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan)
{
  Error_Handler();
}

void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan)
{
  Error_Handler();
}

void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan)
{
  Error_Handler();
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  Error_Handler();
}
