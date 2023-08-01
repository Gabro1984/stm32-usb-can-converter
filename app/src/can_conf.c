#include "stm32f1xx_hal.h"
#include "bsp.h"
#include "can_conf.h"

CAN_HandleTypeDef     CanHandle;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;

// TODO: customize rx size
uint8_t               RxData[8];

/**
 * @brief CAN MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 *           - NVIC configuration for DMA interrupt request enable
 * @param hcan: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
    GPIO_InitTypeDef   GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* CAN1 Periph clock enable */
    CANx_CLK_ENABLE();
    /* Enable GPIO clock ****************************************/
    CANx_GPIO_CLK_ENABLE();
    /* Enable AFIO clock and Remap CAN PINs to PB8 and PB9*******/
    CANx_AFIO_REMAP_CLK_ENABLE();
    CANx_AFIO_REMAP_RX_TX_PIN();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* CAN1 TX GPIO pin configuration */
    GPIO_InitStruct.Pin = CANx_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);

    /* CAN1 RX GPIO pin configuration */
    GPIO_InitStruct.Pin = CANx_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(CANx_RX_GPIO_PORT, &GPIO_InitStruct);

    /* NVIC configuration for CAN1 Reception complete interrupt */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
}
// CAN1_RX0_IRQn
/**
 * @brief CAN MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO to their default state
 * @param hcan: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
    /*##-1- Reset peripherals ##################################################*/
    CANx_FORCE_RESET();
    CANx_RELEASE_RESET();

    /* De-initialize the CAN1 TX GPIO pin */
    HAL_GPIO_DeInit(CANx_TX_GPIO_PORT, CANx_TX_PIN);
    /* De-initialize the CAN1 RX GPIO pin */
    HAL_GPIO_DeInit(CANx_RX_GPIO_PORT, CANx_RX_PIN);

    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
}

/**
 * @brief  Configures the CAN.
 * @param  None
 * @retval None
 */
void CAN_Config(void)
{
    CAN_FilterTypeDef  sFilterConfig;

    /* Configure the CAN peripheral */
    CanHandle.Instance = CANx;

    /* TODO: customize can config */
    CanHandle.Init.TimeTriggeredMode = DISABLE;
    CanHandle.Init.AutoBusOff = DISABLE;
    CanHandle.Init.AutoWakeUp = DISABLE;
    CanHandle.Init.AutoRetransmission = ENABLE;
    CanHandle.Init.ReceiveFifoLocked = DISABLE;
    CanHandle.Init.TransmitFifoPriority = DISABLE;
    CanHandle.Init.Mode = CAN_MODE_NORMAL;
    CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
    CanHandle.Init.TimeSeg1 = CAN_BS1_6TQ;
    CanHandle.Init.TimeSeg2 = CAN_BS2_2TQ;
    CanHandle.Init.Prescaler = 4;

    if (HAL_CAN_Init(&CanHandle) != HAL_OK)
    {
	/* Initialization Error */
	Error_Handler();
    }

    /* Configure the CAN Filter */
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
    {
	/* Filter configuration Error */
	Error_Handler();
    }

    /* Start the CAN peripheral */
    if (HAL_CAN_Start(&CanHandle) != HAL_OK)
    {
	/* Start Error */
	Error_Handler();
    }

    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(&CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
	/* Notification Error */
	Error_Handler();
    }

    /* Configure Transmission process */
    TxHeader.StdId = 0x321;
    TxHeader.ExtId = 0x01;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 2;
    TxHeader.TransmitGlobalTime = DISABLE;
}

/**
 * @brief  Rx Fifo 0 message pending callback in non blocking mode
 * @param  CanHandle: pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
    /* Get RX message */
    if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
	/* Reception Error */
	Error_Handler();
    }

    /* TODO: process received data*/

    /* TODO: remove */
    /* /\* Display LEDx *\/ */
    /* if ((RxHeader.StdId == 0x321) && (RxHeader.IDE == CAN_ID_STD) && (RxHeader.DLC == 2)) */
    /* { */
    /* 	LED_Display(RxData[0]); */
    /* 	ubKeyNumber = RxData[0]; */
    /* } */
}
