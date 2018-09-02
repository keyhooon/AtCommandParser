/*
 * command.c
 *
 *  Created on: Apr 26, 2018
 *      Author: home
 */
#include "../AtCommandParser/command.h"

#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "FleetManagementBoard.h"

#define CARRIAGE_RETURN_CODE 		13	// Carriage return
#define LINE_FEED_CODE				10	// Line feed

#define COMMAND_LINE_TERMINATION_CAHR_DEFAULT	CARRIAGE_RETURN_CODE
#define RESPONSE_FORMATTING_CHAR_DEFAULT		LINE_FEED_CODE
#define COMMAND_ECHO_DEFAULT					1
#define RESPONSE_FORMAT_DEFAULT					1

#define Perform_Command(Command,Response)																\
({																										\
			Response = Command_Executer(commandParser, command)											\
			if (Response.status != ResponseStatusOk)													\
				BSP_GSM_ErrorHandler();																	\
			Response.resultNumber;																		\
})


AtCommandExecuter_TypeDef * CommandExecuter_Init(
		BufferStream_TypeDef * inputBuffer,
		osMessageQId messageId,
		void (*write)(char *, uint32_t)) {
	AtCommandExecuter_TypeDef * commandExecuter = pvPortMalloc(
			sizeof(AtCommandExecuter_TypeDef));
	commandExecuter->IO_Write = write;
	commandExecuter->commandEcho = COMMAND_ECHO_DEFAULT;
	commandExecuter->commandLineTerminationChar =
	COMMAND_LINE_TERMINATION_CAHR_DEFAULT;
	commandExecuter->responseForamettingChar = RESPONSE_FORMATTING_CHAR_DEFAULT;
	commandExecuter->responseFormat = RESPONSE_FORMAT_DEFAULT;
	commandExecuter->ResponseTokenizer = AtTokenizerInit(inputBuffer);
	osMutexDef_t mutex = { 0 };
	commandExecuter->mutexId = osMutexCreate(&mutex);
	commandExecuter->messageId = messageId;
	return commandExecuter;
}
void CommandExecuter_DeInit(AtCommandExecuter_TypeDef *commandExecuter) {
	osMutexDelete(commandExecuter->mutexId);
	AtTokenizerDeInit(commandExecuter->ResponseTokenizer);
	vPortFree(commandExecuter);
}



Response_TypeDef * ExecuteCommand(
		AtCommandExecuter_TypeDef commandExecuter,
		Command_TypeDef command) {
	osMutexWait(commandExecuter.mutexId, osWaitForever);
	char commandString[40];
	GetCommandString(commandString, commandExecuter, command);
	commandExecuter.IO_Write(commandString, strlen(commandString));
	osEvent event = osMessageGet(commandExecuter.messageId,
			command.type.maximumResponseTime);
	Response_TypeDef * result = 0;

	if (event.status == osEventMessage)
		result = ResponseReceived(commandExecuter, event.value.v);
	else {
		result = pvPortMalloc(sizeof(Response_TypeDef));
		result->status = ResponseStatusError_Timeout;
	}
	osMutexRelease(commandExecuter.mutexId);
	return result;
}
void GetCommandString(char* commandText,
		AtCommandExecuter_TypeDef commandExecuter,
		Command_TypeDef command) {
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

Response_TypeDef * ResponseReceived(AtCommandExecuter_TypeDef commandExecuter,
		unsigned int length) {
	TokensList_TypeDef tokensList = AtTokenizedResponse(
			commandExecuter.ResponseTokenizer, length);
	Response_TypeDef * result;
	result = pvPortMalloc(sizeof(Response_TypeDef));
	result->resultNumber = -1;
	result->status = ResponseStatusError_ResultIsntThere;
	memcpy(&result->Tokens, &tokensList, sizeof(TokensList_TypeDef));
	register char* string =
			result->Tokens.Items[result->Tokens.Count - 1].String;
	register unsigned int digit;
	for (; string != 0; string += 1) {
		digit = *string - '0';
		if (digit > 9)
		{
			result->status = ResponseStatusOk;
			break;
		}
		result->resultNumber = (10 * result->resultNumber) + digit;

	}

	return result;
}

void CommandExecuter_ResponseRelease(Response_TypeDef * response) {
	if (response->Tokens.IndexNeedToBeReleased != -1)
		vPortFree(
				response->Tokens.Items[response->Tokens.IndexNeedToBeReleased].String);
	vPortFree(response);
}
