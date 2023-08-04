/**
 ******************************************************************************
 * @file    USB_Device/CustomHID_Standalone/Src/usbd_customhid_if.c
 * @author  MCD Application Team
 * @brief   USB Device Custom HID interface file.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2015 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------ */
#include "usbd_customhid_if.h"

#include "can_conf.h"
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
static int8_t CustomHID_Init(void);
static int8_t CustomHID_DeInit(void);
static int8_t CustomHID_OutEvent(uint8_t cmd, uint8_t* data);
/* Private variables --------------------------------------------------------- */
extern USBD_HandleTypeDef USBD_Device;
extern CAN_HandleTypeDef  CanHandle;

__ALIGN_BEGIN static uint8_t CustomHID_ReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END = {
  0x06,
  0x00,
  0xFF, /* USAGE_PAGE (Vendor Page: 0xFF00) */
  0x09,
  0x01, /* USAGE (Demo Kit) */
  0xa1,
  0x01, /* COLLECTION (Application) */
  /* 6 */

  0x15,
  0x00, /*     LOGICAL_MINIMUM (0)        */
  0x25,
  0xFF, /*     LOGICAL_MAXIMUM (255)        */
  0x75,
  0x08, /*     REPORT_SIZE (8)            */

  0x95,
  CUSTOM_HID_EPIN_SIZE, /*   REPORT_COUNT (9)           */
  0x09,
  0x01, /*   USAGE (Vendor Usage 1)  */
  0x81,
  0x00, /*   INPUT (Data,Ary,Abs) */

  0x95,
  CUSTOM_HID_EPOUT_SIZE, /*   REPORT_COUNT (9)           */
  0x09,
  0x01, /*   USAGE (Vendor Usage 1)  */
  0x91,
  0x00, /*   OUTPUT (Data,Ary,Abs) */

  0xc0 /* END_COLLECTION */
};

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops = {
  CustomHID_ReportDesc,
  CustomHID_Init,
  CustomHID_DeInit,
  CustomHID_OutEvent,
};

/* Private functions --------------------------------------------------------- */

/**
 * @brief  CustomHID_Init
 *         Initializes the CUSTOM HID media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CustomHID_Init(void)
{

    return (0);
}

/**
 * @brief  CustomHID_DeInit
 *         DeInitializes the CUSTOM HID media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CustomHID_DeInit(void)
{
    /*
     * Add your deinitialization code here */
    return (0);
}

/**
 * @brief  CustomHID_OutEvent
 *         Manage the CUSTOM HID class Out Event
 * @param  cmd: packet designation
 * @param  data: packet payload
 */
static int8_t CustomHID_OutEvent(uint8_t cmd, uint8_t* data)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t            TxMailbox;

    /* Configure Transmission process */
    TxHeader.StdId              = 0;
    TxHeader.ExtId              = CAN_DEVICE_ID;
    TxHeader.RTR                = CAN_RTR_DATA;
    TxHeader.IDE                = CAN_ID_EXT;
    TxHeader.DLC                = DATA_LENGTH;
    TxHeader.TransmitGlobalTime = DISABLE;

    BSP_LED_Toggle(LED3);

    switch (cmd)
    {
        case TRANSMIT_TO_CAN:
            HAL_CAN_AddTxMessage(&CanHandle, &TxHeader, data, &TxMailbox);
            break;
        case INFO_REQUEST:
            SendInfo();
            break;
    };

    return (0);
}
