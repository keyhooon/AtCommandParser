/*
 * Gsm.c
 *
 *  Created on: Nov 5, 2018
 *      Author: HP
 */

#include <Parser/AtCommandParser.h>
#include "Gsm.h"
#include "cmsis_os.h"

const char *ATCOMMAND_SEPERATOR = "\r\n";
const char *ATCOMMAND_FOOTER = "\r";

Gsm_TypeDef *GsmModem;
Gsm_TypeDef * Gsm_Init(BufferStream_TypeDef *bufferStream,
		const char * pthreadName)
{
	GsmModem = pvPortMalloc(sizeof(Gsm_TypeDef));
	osMessageQId GsmMessageId;
	osMessageQDef(Gsm, 1, unsigned int);
	osThreadDef_t osThreadDef = { pthreadName, (GSM_Proc), (osPriorityNormal),
			(0), (128) };
	GsmModem->commandExecuter = CommandExecuter_Init(
			osMessageCreate(osMessageQ(Gsm), NULL),
			COMMAND_LINE_TERMINATION_CAHR_DEFAULT);
	GsmModem->commandTokenizer = CommandTokenizer_Init(bufferStream,
			ATCOMMAND_SEPERATOR, ATCOMMAND_FOOTER);
	GsmModem->taskHandle = osThreadCreate(osThreadDef, GsmModem);
	BSP_GSM_Init();
	return GsmModem;
}

void GSM_Proc(void const * argument)
{
	Gsm_TypeDef *GsmModem = argument;
	Response_TypeDef response;
	while(1)
	{
		osEvent event = osMessageGet(GsmModem->commandExecuter->messageId,
				osWaitForever);
		if (event.status == osEventMessage)
		{
			if (osSemaphoreGetCount(GsmModem->commandExecuter->semaphoreId))
				osSemaphoreRelease(GsmModem->commandExecuter->semaphoreId);
			else
				response = ResponseParse(GsmModem->commandExecuter,
						event.value.v);
		}
	}
}

void GSM_DataReceivedCallback(uint32_t Length) {
	osMessagePut(GsmModem->commandExecuter->messageId, Length, osWaitForever);
}
