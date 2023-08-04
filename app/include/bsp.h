#ifndef __BSP_H
#define __BSP_H

#include "stm32f1xx_hal.h"

#define LEDn 3

#define LED1_PIN       GPIO_PIN_15 /* PC.15 */
#define LED1_GPIO_PORT GPIOC

#define LED2_PIN       GPIO_PIN_14 /* PC.14 */
#define LED2_GPIO_PORT GPIOC

#define LED3_PIN       GPIO_PIN_13 /* PC.13 */
#define LED3_GPIO_PORT GPIOC

#define LED_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()

/**
 * @brief LED Types Definition
 */
typedef enum
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,

    LED_YELLOW = LED1,
    LED_GREEN  = LED2,
    LED_RED    = LED3

} Led_TypeDef;

void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);
void Error_Handler(void);

/* TODO: for debug purposes D+ pull-up connected to PC14*/
#define TURN_ON_USB_DEVICE()  BSP_LED_On(LED2)
#define TURN_OFF_USB_DEVICE() BSP_LED_On(LED2)

#endif /* __BSP_H */
