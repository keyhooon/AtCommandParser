/*
 * AtCommandExecuter.c
 *
 *  Created on: Nov 10, 2018
 *      Author: HP
 */

#include "AtCommandExecuter.h"
#include "cmsis_os.h"

void GetCommandString(char* commandText,
		CommandExecuter_TypeDef commandExecuter, Command_TypeDef command);

CommandExecuter_TypeDef * CommandExecuter_Init(osMessageQId messageId,
		char * commandLineTermination) {
	CommandExecuter_TypeDef * commandExecuter = pvPortMalloc(
			sizeof(CommandExecuter_TypeDef));

	commandExecuter->commandLineTerminationChar =
			commandLineTermination;
	osMutexDef_t mutex = { 0 };
	commandExecuter->mutexId = osMutexCreate(&mutex);
	commandExecuter->semaphoreId = osSemaphoreCreate(NULL, 1);
	commandExecuter->messageId = messageId;
	return commandExecuter;
}
void CommandExecuter_DeInit(CommandExecuter_TypeDef *commandExecuter) {
	osMutexDelete(commandExecuter->mutexId);
	AtTokenizerDeInit();
	vPortFree(commandExecuter);
}

Response_TypeDef CommandExecuter_Execute(
		CommandExecuter_TypeDef commandExecuter, Command_TypeDef command) {
	osMutexWait(commandExecuter.mutexId, osWaitForever);
	char commandString[40];
	GetCommandString(commandString, commandExecuter, command);
	GSM_IO_Write(commandString, strlen(commandString));
	osEvent event = osSemaphoreWait(commandExecuter.semaphoreId,
			command.type.maximumResponseTime);
	Response_TypeDef result;

	if (event.status == osEventMessage)
		result = ResponseParse(commandExecuter, event.value.v);
	else {
		result->status = ResponseStatusError_Timeout;
	}
	osMutexRelease(commandExecuter.mutexId);
	return result;
}
void GetCommandString(char* commandText,
		CommandExecuter_TypeDef commandExecuter, Command_TypeDef command) {
	switch (command.type.syntax) {
	case basic:
		sprintf(commandText, "at%s%d%c", command.type.text,
				*(int *) (*command.parameters),
				commandExecuter.commandLineTerminationChar);
		break;
	case sParameter:
		sprintf(commandText, "ats%d=%d%c", *(int *) (*command.parameters),
				*((int *) (*command.parameters) + 1),
				commandExecuter.commandLineTerminationChar);
		break;
	case extended:
		switch (command.action) {
		case Test:
			sprintf(commandText, "at+%s=?%c", command.type.text,
					commandExecuter.commandLineTerminationChar);
			break;
		case Read:
			sprintf(commandText, "at+%s?%c", command.type.text,
					commandExecuter.commandLineTerminationChar);
			break;
		case Write:
			sprintf(commandText, "at+%s=", command.type.text);
			uint32_t index = strlen(commandText);
			while (*command.parameters) {
				uint32_t charIndex = 0;
				while (*(char*) *command.parameters)
					*(commandText + index++) = *(((char*) *command.parameters)
							+ charIndex);
				commandText[index++] = ',';
				command.parameters++;
			}
			index--;
			commandText[index++] = commandExecuter.commandLineTerminationChar;
			commandText[index++] = 0;
			break;
		case Execute:
			sprintf(commandText, "at+%s%c", command.type.text,
					commandExecuter.commandLineTerminationChar);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
