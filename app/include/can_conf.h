/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_CONF_H
#define __CAN_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
//#include "bsp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor CANx instance used and associated
   resources */
/* Definition for CANx clock resources */
#define CANx                           CAN1
#define CANx_CLK_ENABLE()              __HAL_RCC_CAN1_CLK_ENABLE()
#define CANx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

#define CANx_FORCE_RESET()             __HAL_RCC_CAN1_FORCE_RESET()
#define CANx_RELEASE_RESET()           __HAL_RCC_CAN1_RELEASE_RESET()

/* Definition for CANx Pins */
#define CANx_TX_PIN                    GPIO_PIN_9
#define CANx_TX_GPIO_PORT              GPIOB
#define CANx_RX_PIN                    GPIO_PIN_8
#define CANx_RX_GPIO_PORT              GPIOB

/* Definition for CANx AFIO Remap */
#define CANx_AFIO_REMAP_CLK_ENABLE()   __HAL_RCC_AFIO_CLK_ENABLE()
#define CANx_AFIO_REMAP_RX_TX_PIN()    __HAL_AFIO_REMAP_CAN1_2()

/* Definition for CAN's NVIC */
/* TODO: propably can be removed*/
#define CANx_RX_IRQn                   USB_LP_CAN1_RX0_IRQn
#define CANx_RX_IRQHandler             USB_LP_CAN1_RX0_IRQHandler

/* Listen device ID*/
#define    LISTEN_CAN_DEVICE_ID_1 ((uint32_t)0x18000002)
#define    LISTEN_CAN_DEVICE_ID_2 ((uint32_t)0x18FAAF52)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
    void CAN_Config(void);

#endif /* __CAN_CONF_H */
