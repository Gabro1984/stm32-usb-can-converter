/**
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/Src/main.c
  * @author  MCD Application Team
  * @brief   USB device CustomHID application main file.
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

/* Includes ------------------------------------------------------------------ */
#include "main.h"


/** @addtogroup STM32F1xx_HAL_Validation
  * @{
  */

/** @addtogroup STANDARD_CHECK
  * @{
  */

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
USBD_HandleTypeDef USBD_Device;
DevInfo info;

/* Private function prototypes ----------------------------------------------- */
void SystemClock_Config(void);
void GetDeviceInfo(void);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  SystemClock_Config();

  /* Init LEDs*/
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Fill DevInfo structure */
  GetDeviceInfo();

  /* TODO: hack for debug purposes. Need to remove in production.*/
  TURN_ON_USB_DEVICE();

  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, &USBD_CUSTOM_HID);

  /* Add Custom HID callbacks */
  USBD_CUSTOM_HID_RegisterInterface(&USBD_Device, &USBD_CustomHID_fops);

  /* Start Device Process */
  USBD_Start(&USBD_Device);

    /* Configure the CAN peripheral */
  CAN_Config();

  /* Infinite loop */
  while (1)
  {
      BSP_LED_Toggle(LED1);
      HAL_Delay(500);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = { 0 };
  RCC_OscInitTypeDef oscinitstruct = { 0 };
  RCC_PeriphCLKInitTypeDef rccperiphclkinit = { 0 };

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;

  oscinitstruct.PLL.PLLState = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  if (HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }

  /* USB clock selection */
  rccperiphclkinit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  rccperiphclkinit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV3;
  HAL_RCCEx_PeriphCLKConfig(&rccperiphclkinit);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
   * clocks dividers */
  clkinitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
}

void GetDeviceInfo()
{
    uint32_t dev_id_0, dev_id_1, dev_id_2;

    dev_id_0     = *(uint32_t*)DEVICE_ID1;
    dev_id_1     = *(uint32_t*)DEVICE_ID2;
    dev_id_2     = *(uint32_t*)DEVICE_ID3;

    info.data[0]  = (uint8_t)((dev_id_0 >> 24) & 0xFF);
    info.data[1]  = (uint8_t)((dev_id_0 >> 16) & 0xFF);
    info.data[2]  = (uint8_t)((dev_id_0 >> 8) & 0xFF);
    info.data[3]  = (uint8_t)((dev_id_0) & 0xFF);
    info.data[4]  = (uint8_t)((dev_id_1 >> 24) & 0xFF);
    info.data[5]  = (uint8_t)((dev_id_1 >> 16) & 0xFF);
    info.data[6]  = (uint8_t)((dev_id_1 >> 8) & 0xFF);
    info.data[7]  = (uint8_t)((dev_id_1) & 0xFF);
    info.data[8]  = (uint8_t)((dev_id_2 >> 24) & 0xFF);
    info.data[9]  = (uint8_t)((dev_id_2 >> 16) & 0xFF);
    info.data[10]  = (uint8_t)((dev_id_2 >> 8) & 0xFF);
    info.data[11]  = (uint8_t)((dev_id_2) & 0xFF);

    info.data[12]  = (uint8_t)((SW_VER >> 24) & 0xFF);
    info.data[13]  = (uint8_t)((SW_VER >> 16) & 0xFF);
    info.data[14]  = (uint8_t)((SW_VER >> 8) & 0xFF);
    info.data[15]  = (uint8_t)((SW_VER) & 0xFF);
}

void SendInfo()
{
    /* TODO: adjust repsponze length */
    uint8_t Response[MSG_LENGTH] = {0};

    info.tx_in_progress = 1;
    info.block_num = 1;

    Response[0] = INFO_RESPONSE;
    Response[1] = 1;
    Response[2] = info.block_num;
    memcpy(Response+3, info.data, INFO_BLOCK_DATA_SIZE);

    USBD_CUSTOM_HID_SendReport(&USBD_Device, Response, sizeof(Response));
    ++info.block_num;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
   * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
   * line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */
