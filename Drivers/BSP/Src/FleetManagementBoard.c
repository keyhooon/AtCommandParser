/*
 * FleetManagementBoard.c
 *
 *  Created on: Aug 26, 2017
 *      Author: home
 */

#include "FleetManagementBoard.h"

#define __FLEET_MANAGEMENT_BOARD_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __FLEET_MANAGEMENT_BOARD_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __FLEET_MANAGEMENT_BOARD_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __FLEET_MANAGEMENT_BOARD_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __FLEET_MANAGEMENT_BOARD_BSP_VERSION        ((__FLEET_MANAGEMENT_BOARD_BSP_VERSION_MAIN << 24)\
                                            |(__FLEET_MANAGEMENT_BOARD_BSP_VERSION_SUB1 << 16)\
                                            |(__FLEET_MANAGEMENT_BOARD_BSP_VERSION_SUB2 << 8 )\
                                            |(__FLEET_MANAGEMENT_BOARD_BSP_VERSION_RC))





UART_HandleTypeDef FLEET_COMx_HUART[COMn];
DMA_HandleTypeDef FLEET_COMx_HDMA_RX[COMn];
DMA_HandleTypeDef FLEET_COMx_HDMA_TX[COMn];

const USART_TypeDef* FLEET_COMx_USART[COMn] = {FLEET_COM1, FLEET_COM2};
const GPIO_TypeDef* FLEET_COMx_PORT[COMn] = {FLEET_COM1_GPIO_PORT, FLEET_COM2_GPIO_PORT};
const uint32_t FLEET_COMx_BAUDRATE[COMn] = {FLEET_COM1_BAUDRATE, FLEET_COM2_BAUDRATE};
const uint16_t FLEET_COMx_TX_PIN[COMn] = {FLEET_COM1_TX_PIN, FLEET_COM2_TX_PIN};
const uint16_t FLEET_COMx_RX_PIN[COMn] = {FLEET_COM1_RX_PIN, FLEET_COM2_RX_PIN};
const DMA_Channel_TypeDef * FLEET_COMX_RX_DMA_STREAM[COMn]={FLEET_COM1_RX_DMA_STREAM, FLEET_COM2_RX_DMA_STREAM};
const DMA_Channel_TypeDef * FLEET_COMX_TX_DMA_STREAM[COMn]={FLEET_COM1_TX_DMA_STREAM, FLEET_COM2_TX_DMA_STREAM};
const uint32_t FLEET_COMx_IRQn[COMn] = { FLEET_COM1_IRQn,
		FLEET_COM2_IRQn };

BufferStream_TypeDef *FLEET_COMx_BUFFER_STREAM[COMn];

static void COMx_MspInit(COM_TypeDef COM);
static void COMx_Init(COM_TypeDef COM);
static void COMx_DeInit(COM_TypeDef COM);
static void COMx_Open(COM_TypeDef COM);
static void COMx_Close(COM_TypeDef COM);
static void COMx_Write(COM_TypeDef COM, uint8_t *Value, uint8_t Size);
static void COMx_Write_DMA(COM_TypeDef COM, char *buffer, uint8_t length);
static void COMx_Read(COM_TypeDef COM, char * data, uint32_t length);
inline static void COMx_ReadBefore(COM_TypeDef COM, char * data,
		uint32_t length);
inline static char * COMx_Read_Char(COM_TypeDef COM);
void COMx_DataReceivedCallback(COM_TypeDef COM, uint32_t Length);
static void COMx_Error(COM_TypeDef COM);







/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* COM Routines *********************************/

/**
  * @brief  Initializes COM MSP.
  */
static void COMx_MspInit(COM_TypeDef COM)
{
	  GPIO_InitTypeDef gpio_init_structure;

	  /* Enable GPIO clock of GPIO port supporting COM_TX and COM_RX pins */
	  FLEET_COMx_GPIO_CLK_ENABLE(COM);

	  /* Enable USART clock */
	  FLEET_COMx_CLK_ENABLE(COM);

	  /* Configure USART as alternate function */
	  gpio_init_structure.Pin = FLEET_COMx_TX_PIN[COM];
	  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
	  gpio_init_structure.Pull = GPIO_PULLUP;
	  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(FLEET_COMx_PORT[COM], &gpio_init_structure);

	  gpio_init_structure.Pin = FLEET_COMx_RX_PIN[COM];
	  gpio_init_structure.Mode = GPIO_MODE_INPUT;
	  gpio_init_structure.Pull = GPIO_NOPULL;
	  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(FLEET_COMx_PORT[COM], &gpio_init_structure);

	  HAL_NVIC_SetPriority(FLEET_COMx_IRQn[COM], 5, 0);
	  HAL_NVIC_EnableIRQ(FLEET_COMx_IRQn[COM]);

}


/**
  * @brief  Configures COM port.
  * @param  COM: COM port to be configured.
  *          This parameter can be one of the following values:
  *            @arg  COM1
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
static void COMx_Init(COM_TypeDef COM)
{
	COMx_DeInit(COM);
    FLEET_COMx_CLK_ENABLE(COM);
    FLEET_COMx_RX_DMA_CLK_ENABLE(COM);
    /* USART configuration */
	FLEET_COMx_HUART[COM].Instance = FLEET_COMx_USART[COM];
	FLEET_COMx_HUART[COM].Init.BaudRate = FLEET_COMx_BAUDRATE[COM];
	FLEET_COMx_HUART[COM].Init.WordLength = UART_WORDLENGTH_8B;
	FLEET_COMx_HUART[COM].Init.StopBits = UART_STOPBITS_1;
	FLEET_COMx_HUART[COM].Init.Parity = UART_PARITY_NONE;
	FLEET_COMx_HUART[COM].Init.Mode = UART_MODE_TX_RX;
	FLEET_COMx_HUART[COM].Init.HwFlowCtl = UART_HWCONTROL_NONE;
	FLEET_COMx_HUART[COM].Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&FLEET_COMx_HUART[COM]) != HAL_OK)
	{
	 COMx_Error(COM);
	}
	COMx_MspInit(COM);

	/* USART DMA Init */
	/* USART_RX Init */
	FLEET_COMx_HDMA_RX[COM].Instance = FLEET_COMX_RX_DMA_STREAM[COM];
	FLEET_COMx_HDMA_RX[COM].Init.Direction = DMA_PERIPH_TO_MEMORY;
	FLEET_COMx_HDMA_RX[COM].Init.PeriphInc = DMA_PINC_DISABLE;
	FLEET_COMx_HDMA_RX[COM].Init.MemInc = DMA_MINC_ENABLE;
	FLEET_COMx_HDMA_RX[COM].Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	FLEET_COMx_HDMA_RX[COM].Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	FLEET_COMx_HDMA_RX[COM].Init.Mode = DMA_CIRCULAR;
	FLEET_COMx_HDMA_RX[COM].Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&FLEET_COMx_HDMA_RX[COM]) != HAL_OK)
	{
		 COMx_Error(COM);
	}

	__HAL_LINKDMA(&FLEET_COMx_HUART[COM],hdmarx,FLEET_COMx_HDMA_RX[COM]);

	/* USART DMA Init */
	/* USART_TX Init */
	FLEET_COMx_HDMA_TX[COM].Instance = FLEET_COMX_TX_DMA_STREAM[COM];
	FLEET_COMx_HDMA_TX[COM].Init.Direction = DMA_PERIPH_TO_MEMORY;
	FLEET_COMx_HDMA_TX[COM].Init.PeriphInc = DMA_PINC_DISABLE;
	FLEET_COMx_HDMA_TX[COM].Init.MemInc = DMA_MINC_ENABLE;
	FLEET_COMx_HDMA_TX[COM].Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	FLEET_COMx_HDMA_TX[COM].Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	FLEET_COMx_HDMA_TX[COM].Init.Mode = DMA_CIRCULAR;
	FLEET_COMx_HDMA_TX[COM].Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&FLEET_COMx_HDMA_TX[COM]) != HAL_OK)
	{
		 COMx_Error(COM);
	}

	__HAL_LINKDMA(&FLEET_COMx_HUART[COM],hdmatx,FLEET_COMx_HDMA_TX[COM]);

	/* DMA interrupt init */
	  /* DMA_Stream_IRQn interrupt configuration */
//	  HAL_NVIC_SetPriority(FLEET_COMx_RX_DMA_IRQ[COM], 5, 0);
//	  HAL_NVIC_EnableIRQ(FLEET_COMx_RX_DMA_IRQ[COM]);
}

/**
  * @brief  DeInit COM port.
  * @param  COM: COM port to be configured.
  *          This parameter can be one of the following values:
  *            @arg  COM1
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
static void COMx_DeInit(COM_TypeDef COM)
{
  /* USART configuration */
	UART_HandleTypeDef huart;
	huart.Instance = FLEET_COMx_USART[COM];
  HAL_UART_DeInit(&huart);

  /* Enable USART clock */
  FLEET_COMx_CLK_DISABLE(COM);

}



static void COMx_Open(COM_TypeDef COM) {
	FLEET_COMx_BUFFER_STREAM[COM] = BufferStreamInit(DEFAULT_BUFFER_LENGTH);
	if (HAL_UART_Receive_DMA(&FLEET_COMx_HUART[COM],
			FLEET_COMx_BUFFER_STREAM[COM]->buffer,
			FLEET_COMx_BUFFER_STREAM[COM]->length) != HAL_OK)
		COMx_Error(COM);
	__HAL_UART_ENABLE_IT(&FLEET_COMx_HUART[COM], UART_IT_IDLE);
}

static void COMx_Close(COM_TypeDef COM) {
	BufferStreamDeinit(FLEET_COMx_BUFFER_STREAM[COM]);
	if (HAL_UART_AbortReceive(&FLEET_COMx_HUART[COM]) != HAL_OK)
		COMx_Error(COM);
	__HAL_UART_DISABLE_IT(&FLEET_COMx_HUART[COM], UART_IT_IDLE);
}



/**
  * @brief  Writes data.
  * @param  COM: Com address
  * @param  Value *: Data to be written
  * @param  Size: Register address
  * @retval HAL status
  */
static void COMx_Write(COM_TypeDef COM, uint8_t *buffer, uint8_t length)
{
  if(HAL_UART_Transmit(&FLEET_COMx_HUART[COM],
		  buffer,
          (uint8_t)length,
          100) != HAL_OK)
 	  COMx_Error(COM);
}

/**
  * @brief  Writes data.
  * @param  COM: Com address
  * @param  Value *: Data to be written
  * @param  Size: Register address
  * @retval HAL status
  */
static void COMx_Write_DMA(COM_TypeDef COM, char *buffer, uint8_t length)
{
  if(HAL_UART_Transmit_DMA(&FLEET_COMx_HUART[COM],
		  (uint8_t*) buffer,
          (uint8_t)length) != HAL_OK)
	  COMx_Error(COM);
}


/**
  * @brief  Reads data.
  * @param  COM: Com address
  * @param  * Value: Pointer to first data Address
  * @param  Size: Size of Buffer
  */
static void COMx_Read(COM_TypeDef COM, char * data, uint32_t length) {
	BufferStreamRead(FLEET_COMx_BUFFER_STREAM[COM], data, length);
}
inline static void COMx_ReadBefore(COM_TypeDef COM, char * data,
		uint32_t length) {
	BufferStreamReadBefore(FLEET_COMx_BUFFER_STREAM[COM], data, length);
}

inline static char * COMx_Read_Char(COM_TypeDef COM)
{
	return BufferStreamReadChar(FLEET_COMx_BUFFER_STREAM[COM]);
}


/**
 * @brief  Callback raise when data received from COMx
 * @param  COM: Com address
 * @param  * Value: Pointer to first data Address
 * @param  Size: Size of Buffer
 */
__weak void COMx_DataReceivedCallback(COM_TypeDef COM, uint32_t Length) {

}

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  */
static void COMx_Error(COM_TypeDef COM)
{
  /* De-initialize the COM comunication bus */
  HAL_UART_DeInit(&FLEET_COMx_HUART[COM]);

  /* Re-Initiaize the COM comunication bus */
  COMx_Init(COM);
}

void BSP_UART_IRQHandler(COM_TypeDef COM) {
	if (__HAL_UART_GET_IT_SOURCE(&FLEET_COMx_HUART[COM], UART_IT_IDLE)) {
		__HAL_UART_CLEAR_IDLEFLAG(&FLEET_COMx_HUART[COM]);
		uint32_t rxBufferHead = FLEET_COMx_BUFFER_STREAM[COM]->length
				- (__HAL_DMA_GET_COUNTER((FLEET_COMx_HUART[COM]).hdmarx));
		int32_t count = rxBufferHead - FLEET_COMx_BUFFER_STREAM[COM]->head;
		if (count < 0)
			count += FLEET_COMx_BUFFER_STREAM[COM]->length;
		FLEET_COMx_BUFFER_STREAM[COM]->head = rxBufferHead;
		COMx_DataReceivedCallback(COM, count);
		switch (COM) {
		case GSM_COM:
			GSM_DataReceivedCallback(count);
			break;
		default:
			break;
		}



	}
}


/*******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK GSM ***********************************/

/**
  * @brief  Initializes GSM low level.
  * @retval None
  */
void GSM_IO_Init(void)
{
	COMx_Init(GSM_COM);
	COMx_Open(GSM_COM);
}


/**
  * @brief  DeInitializes GSM low level.
  * @retval None
  */
void GSM_IO_DeInit(void)
{
	COMx_Close(GSM_COM);
	COMx_DeInit(GSM_COM);
}

/**
  * @brief  GSM delay
  * @param  Delay: Delay in ms
  * @retval None
  */
void GSM_IO_Delay(uint32_t Delay)
{
	osDelay(Delay);
}

/**
  * @brief  GSM writes data.
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  */
void GSM_IO_Write(char * buffer, uint32_t length) {
	COMx_Write_DMA(GSM_COM,buffer,length);
}

void GSM_IO_Read(char * data, uint32_t length) {
	COMx_Read(GSM_COM, data, length);
}

void GSM_IO_ReadBefore(char * data, uint32_t length) {
	COMx_ReadBefore(GSM_COM, data, length);
}

char* GSM_IO_Read_Char() {
	return COMx_Read_Char(GSM_COM);
}

BufferStream_TypeDef * GSM_IO_GetBuffer() {
	return FLEET_COMx_BUFFER_STREAM[GSM_COM];

}

__weak void GSM_DataReceivedCallback(uint32_t Length) {

}

/**
  * @}
  */

