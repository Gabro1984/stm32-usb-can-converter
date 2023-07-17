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

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
static int8_t CustomHID_Init(void);
static int8_t CustomHID_DeInit(void);
static int8_t CustomHID_OutEvent(uint8_t cmd, uint8_t *data);
/* Private variables --------------------------------------------------------- */
extern USBD_HandleTypeDef USBD_Device;

__ALIGN_BEGIN static uint8_t
  CustomHID_ReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END = {
  0x06, 0xFF, 0x00,             /* USAGE_PAGE (Vendor Page: 0xFF00) */
  0x09, 0x01,                   /* USAGE (Demo Kit) */
  0xa1, 0x01,                   /* COLLECTION (Application) */
  /* 6 */

  0x15, 0x00,                            /*     LOGICAL_MINIMUM (0)        */
  0x25, 0xFF,                            /*     LOGICAL_MAXIMUM (255)        */
  0x75, 0x08,                            /*     REPORT_SIZE (8)            */
  0x95, 64,                            /*     REPORT_COUNT (64)           */

  0x09, 0x01,
  0x81, 0x02,
  0x95, 64,

  0x09, 0x01,
  0x91, 0x02,
  0x95, 0x01,

  0x09, 0x01,
  0xB1, 0x02,

  0xc0                          /* END_COLLECTION */
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
  /* Configure LED */
  BSP_LED_Init(LED1);

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
/* TODO: add logic */
static int8_t CustomHID_OutEvent(uint8_t cmd, uint8_t *data)
{

  return (0);
}
