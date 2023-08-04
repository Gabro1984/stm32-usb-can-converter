#include "stm32f1xx_hal.h"
#include "bsp.h"
#include "can_conf.h"
#include "usbd_customhid.h"
#include "main.h"

extern USBD_HandleTypeDef USBD_Device;

CAN_HandleTypeDef     CanHandle;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[DATA_LENGTH];

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

    /* CAN1 Periph clock enable */
    CANx_CLK_ENABLE();
    /* Enable GPIO clock ****************************************/
    CANx_GPIO_CLK_ENABLE();
    /* Enable AFIO clock and Remap CAN PINs to PB8 and PB9*******/
    CANx_AFIO_REMAP_CLK_ENABLE();
    CANx_AFIO_REMAP_RX_TX_PIN();

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
    CanHandle.Init.AutoBusOff = ENABLE;
    CanHandle.Init.AutoWakeUp = DISABLE;
    CanHandle.Init.AutoRetransmission = ENABLE;
    CanHandle.Init.ReceiveFifoLocked = DISABLE;
    CanHandle.Init.TransmitFifoPriority = DISABLE;
    CanHandle.Init.Mode = CAN_MODE_NORMAL;
    /* CAN_baud_rate = 250kHz,
       T_quantum = APB1_clk/Prescaler = 36MHz/12 number of APB1 clk in one time quantum)
       BitTime = SyncJumpWidth + TimeSeg1 + TimeSeg2 = 12 * T_quantum
*/
    CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
    CanHandle.Init.TimeSeg1 = CAN_BS1_8TQ;
    CanHandle.Init.TimeSeg2 = CAN_BS2_3TQ;
    CanHandle.Init.Prescaler = 12;

    if (HAL_CAN_Init(&CanHandle) != HAL_OK)
    {
	/* Initialization Error */
	Error_Handler();
    }

    /* Configure the CAN Filter */
    /* Filter register format:
     High: STD_ID[10:0] | EXT_ID[17:13]
     Low: EXT_ID[12:0] | IDE | RTR | 0
     Note: IDE must be "1" for 29-bit address
*/
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh =  (uint16_t)(LISTEN_CAN_DEVICE_ID_1 >> 13);
    sFilterConfig.FilterIdLow =  (uint16_t)(LISTEN_CAN_DEVICE_ID_1 << 3) | 0x04;
    sFilterConfig.FilterMaskIdHigh = (uint16_t)(LISTEN_CAN_DEVICE_ID_2 >> 13);
    sFilterConfig.FilterMaskIdLow = (uint16_t)(LISTEN_CAN_DEVICE_ID_2 << 3) | 0x04;
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

    /* Activate CAN RX0 and errors notifications */
    if (HAL_CAN_ActivateNotification(&CanHandle,
				     CAN_IT_RX_FIFO0_MSG_PENDING|
				     CAN_IT_ERROR |
				     CAN_IT_BUSOFF |
				     CAN_IT_LAST_ERROR_CODE) != HAL_OK)
    {
	/* Notification Error */
	Error_Handler();
    }
}

/**
 * @brief  Rx Fifo 0 message pending callback in non blocking mode
 * @param  CanHandle: pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
    /* TODO: adjust message size */
    uint8_t DataToHID[MSG_LENGTH] = {0};

    /* Get RX message */
    if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	return;

    if ((RxHeader.IDE == CAN_ID_EXT) && (RxHeader.DLC == DATA_LENGTH))
    {
	BSP_LED_Toggle(LED3);

	switch(RxHeader.ExtId) {
	case LISTEN_CAN_DEVICE_ID_1:
	    DataToHID[0] = GET_FROM_CAN_ID_1;
	    break;
	case LISTEN_CAN_DEVICE_ID_2:
	    DataToHID[0] = GET_FROM_CAN_ID_2;
	    break;
	default:
	    return;
	};

	memcpy(DataToHID+1, RxData, sizeof(RxData));
	USBD_CUSTOM_HID_SendReport(&USBD_Device, DataToHID, sizeof(DataToHID));
    }
}

/**
  * @brief  Error CAN callback.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    static uint32_t tickstart;
    /* TODO: adjust message size */
    uint8_t DataToHID[MSG_LENGTH] = {0};

    if(!tickstart)
	tickstart = HAL_GetTick();

    uint32_t error = HAL_CAN_GetError(hcan);

    if((HAL_GetTick() - tickstart) > CAN_ERROR_NOTIFY_TIMEOUT )
    {
	DataToHID[0] = INFO_RESPONSE;
	DataToHID[1] = 0x0F;
	DataToHID[2]  = (uint8_t)((error >> 24) & 0xFF);
	DataToHID[3]  = (uint8_t)((error >> 16) & 0xFF);
	DataToHID[4]  = (uint8_t)((error >> 8) & 0xFF);
	DataToHID[5]  = (uint8_t)((error) & 0xFF);

	USBD_CUSTOM_HID_SendReport(&USBD_Device, DataToHID, sizeof(DataToHID));

	tickstart  = HAL_GetTick();
    }
}
