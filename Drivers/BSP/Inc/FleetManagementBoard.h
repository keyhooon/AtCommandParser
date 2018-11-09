/*
 * FleetManagementBoard.h
 *
 *  Created on: Aug 26, 2017
 *      Author: home
 */

#ifndef BSP_INC_MANAGEMENTBOARD_H_
#define BSP_INC_MANAGEMENTBOARD_H_

#include "stm32f1xx_hal.h"
#include "bufferStream.h"
typedef struct {
	uint32_t index;
	uint32_t length;
} COM_DataRecv_TypeDef;

typedef enum {
	COM1 = 0, COM2 = 1, GSM_COM = COM1, GPS_COM = COM2
} COM_TypeDef;

#define DEFAULT_BUFFER_LENGTH		1024
#define DEFAULT_QUEUE_LENGTH		5



typedef void (*COM_DataRecv_Callback_TypeDef)(COM_DataRecv_TypeDef DataRecv);

/**
 * @brief Definition for COM
 */

#define COMn                             ((uint8_t)2)

#define FLEET_COM1                          USART1
#define FLEET_COM1_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
#define FLEET_COM1_CLK_DISABLE()            __HAL_RCC_USART1_CLK_DISABLE()
#define FLEET_COM1_IRQn                     USART1_IRQn
#define FLEET_COM1_IRQHANDLER             	USART1_IRQHandler

#define FLEET_COM1_BAUDRATE					115200

#define FLEET_COM1_GPIO_PORT            	 GPIOA
#define FLEET_COM1_GPIO_CLK_ENABLE()     	__HAL_RCC_GPIOA_CLK_ENABLE()
#define FLEET_COM1_GPIO_CLK_DISABLE()    	__HAL_RCC_GPIOA_CLK_DISABLE()

#define FLEET_COM1_TX_PIN                   GPIO_PIN_9
#define FLEET_COM1_TX_DMA_STREAM			DMA1_Channel5
#define FLEET_COM1_TX_DMA_IRQ  				DMA1_Channel5_IRQn
#define FLEET_COM1_TX_DMA_IRQHandler		DMA1_Channel5_IRQHandler
#define FLEET_COM1_TX_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE()

#define FLEET_COM1_RX_PIN                   GPIO_PIN_10
#define FLEET_COM1_RX_DMA_STREAM			DMA1_Channel5
#define FLEET_COM1_RX_DMA_IRQ  				DMA1_Channel5_IRQn
#define FLEET_COM1_RX_DMA_IRQHandler		DMA1_Channel5_IRQHandler
#define FLEET_COM1_RX_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE()

#define FLEET_COM2                          USART3
#define FLEET_COM2_BAUDRATE					9600
#define FLEET_COM2_CLK_ENABLE()             __HAL_RCC_USART3_CLK_ENABLE()
#define FLEET_COM2_CLK_DISABLE()            __HAL_RCC_USART3_CLK_DISABLE()
#define FLEET_COM2_IRQn                     USART3_IRQn
#define FLEET_COM2_IRQHANDLER             	USART3_IRQHandler

#define FLEET_COM2_GPIO_PORT             	GPIOB
#define FLEET_COM2_GPIO_CLK_ENABLE()     	__HAL_RCC_GPIOB_CLK_ENABLE()
#define FLEET_COM2_GPIO_CLK_DISABLE()    	__HAL_RCC_GPIOB_CLK_DISABLE()

#define FLEET_COM2_TX_PIN                   GPIO_PIN_10
#define FLEET_COM2_TX_DMA_STREAM			DMA1_Channel3
#define FLEET_COM2_TX_DMA_IRQ  				DMA1_Channel3_IRQn
#define FLEET_COM2_TX_DMA_IRQHandler		DMA1_Channel3_IRQHandler
#define FLEET_COM2_TX_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE()

#define FLEET_COM2_RX_PIN                   GPIO_PIN_11
#define FLEET_COM2_RX_DMA_STREAM			DMA1_Channel3
#define FLEET_COM2_RX_DMA_IRQ  				DMA1_Channel3_IRQn
#define FLEET_COM2_RX_DMA_IRQHandler		DMA1_Channel3_IRQHandler
#define FLEET_COM2_RX_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE()

#define FLEET_COMx_CLK_ENABLE(__INDEX__)    	  do { 	if((__INDEX__) == COM1) FLEET_COM1_CLK_ENABLE(); else\
																					FLEET_COM2_CLK_ENABLE(); } while(0)
#define FLEET_COMx_CLK_DISABLE(__INDEX__)    	  do { 	if((__INDEX__) == COM1) FLEET_COM1_CLK_DISABLE(); else\
																					FLEET_COM2_CLK_DISABLE(); } while(0)
#define FLEET_COMx_GPIO_CLK_ENABLE(__INDEX__)  do { 	if((__INDEX__) == COM1) FLEET_COM1_GPIO_CLK_ENABLE(); else\
																					FLEET_COM2_GPIO_CLK_ENABLE();} while(0)
#define FLEET_COMx_GPIO_CLK_DISABLE(__INDEX__) do { 	if((__INDEX__) == COM1) FLEET_COM1_RX_TX_GPIO_CLK_DISABLE(); else\
																					FLEET_COM2_RX_TX_GPIO_CLK_DISABLE(); } while(0)
#define FLEET_COMx_RX_DMA_CLK_ENABLE(__INDEX__)  do { 	if((__INDEX__) == COM1) FLEET_COM1_RX_DMA_CLK_ENABLE(); else\
																					FLEET_COM2_RX_DMA_CLK_ENABLE(); } while(0)

extern BufferStream_TypeDef *FLEET_COMx_BUFFER_STREAM[COMn];
void BSP_UART_IRQHandler(COM_TypeDef COM);
void GSM_IO_Init(void);
void GSM_IO_DeInit(void);
void GSM_IO_Delay(uint32_t Delay);
void GSM_IO_Write(char * buffer, uint32_t length);
void GSM_IO_Read(char * data, uint32_t length);
void GSM_IO_ReadBefore(char * data, uint32_t length);
char * GSM_IO_Read_Char();
int GSM_IO_Check_Chars_Equality(char *data, int length);
BufferStream_TypeDef * GSM_IO_GetBuffer();

void GSM_DataReceivedCallback(uint32_t Length);

#endif /* BSP_INC_MANAGEMENTBOARD_H_ */
