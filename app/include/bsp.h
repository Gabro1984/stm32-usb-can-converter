#ifndef __BSP_H
#define __BSP_H

#include "stm32f1xx_hal.h"

#define LEDn                             1

#define LED1_PIN                         GPIO_PIN_15             /* PC.15*/
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()

/**
 * @brief LED Types Definition
 */
typedef enum
{
    LED1 = 0,
    LED_GREEN  = LED1,
} Led_TypeDef;

void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);
void Error_Handler(void);

#endif /* __BSP_H */
