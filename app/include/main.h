/**
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "bsp.h"
#include "usbd_core.h"
#include "stm32f1xx_hal_pcd.h"
#include "usbd_desc.h"
#include "usbd_customhid_if.h"
#include "can_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define SW_VER 0x01020304

#define TRANSMIT_TO_CAN    1
#define  GET_FROM_CAN_ID_1 2
#define INFO_REQUEST       3
#define INFO_RESPONSE      4
#define  GET_FROM_CAN_ID_2 5

#define DEV_INFO_SIZE 16
#define INFO_BLOCK_CNT 3
#define INFO_BLOCK_DATA_SIZE 6

typedef struct DeviceInfo {
    uint8_t data[DEV_INFO_SIZE];
    uint8_t tx_in_progress;
    uint8_t block_num;
} DevInfo;

/* Exported functions ------------------------------------------------------- */
void SendInfo(void);

#endif /* __MAIN_H */
