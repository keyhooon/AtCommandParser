/*
 * command.h
 *
 *  Created on: Apr 26, 2018
 *      Author: home
 */

#ifndef ATCOMMANDPARSER_COMMAND_H_
#define ATCOMMANDPARSER_COMMAND_H_

#include <BufferTokenizer.h>
#include "stdint.h"
#include "cmsis_os.h"
/* Acknowledges execution of a Command
 * */
#define RESULT_NUMBER_OK					0
#define RESULT_CODE_OK						(const char *) "OK"

/* A connection has been established; the DCE is moving from
 * Command state to online data state
 * */
#define RESULT_NUMBER_CONNECT				1
#define RESULT_CODE_CONNECT					(const char *) "CONNECT"

/* The DCE has detected an incoming call signal from
 * network
 * */
#define RESULT_NUMBER_RING					2
#define RESULT_CODE_RING					(const char *) "RING"

/* The connection has been terminated or the attempt to
 * establish a connection failed
 * */
#define RESULT_NUMBER_NOCARRIER				3
#define RESULT_CODE_NOCARRIER				(const char *) "NO CARRIER"

/* Command not recognized, Command line maximum length
 * exceeded, parameter value invalid, or other problem with
 * processing the Command line
 * */
#define RESULT_NUMBER_ERROR					4
#define RESULT_CODE_ERROR					(const char *) "ERROR"

/* No dial tone detected
 * */
#define RESULT_NUMBER_NODIALTONE			6
#define RESULT_CODE_NODIALTONE				(const char *) "NO DIALTONE"

/* Engaged (busy) signal detected
 * */
#define RESULT_NUMBER_BUSY					7
#define RESULT_CODE_BUSY					(const char *) "BUSY"
/* "@" (Wait for Quiet Answer) dial modifier was used, but
 * remote ringing followed by five seconds of silence was not
 * detected before expiration of the connection timer (S7)
 * */
#define RESULT_NUMBER_NOANSWER				8
#define RESULT_CODE_NOANSWER				(const char *) "NO ANSWER"

typedef struct CommandParser_Struct
{
	char commandLineTerminationChar;			// s3
	char responseForamettingChar;				// s4
	char commandEcho;							// e
	char responseFormat;						// v
	Tokenizer_TypeDef * ResponseTokenizer;
	osMutexId mutexId;
	osSemaphoreId semaphoreId;
	osMessageQId messageId;
} AtCommandExecuter_TypeDef;

typedef enum {
	basic, sParameter, extended,
} CommandSyntax_TypeDef;

typedef enum {
	Test, Read, Write, Execute
} CommandAction_TypeDef;

typedef struct {
	const char * text;
	uint32_t maximumResponseTime;
	CommandSyntax_TypeDef syntax;
} CommandType_TypeDef;

typedef struct {
	CommandType_TypeDef type;
	CommandAction_TypeDef action;
	void ** parameters;
} Command_TypeDef;

typedef enum {
	final, intermediate, unsolicited,
} ResponseType_TypeDef;

typedef enum {
	ResponseStatusOk,
	ResponseStatusError_Timeout,
	ResponseStatusError_ResultIsntThere,
} ResponseStatus_TypeDef;

typedef struct {
	const uint8_t number;
	const char * code;
	const ResponseType_TypeDef type;
} ResponseResult_TypeDef;

typedef struct {
	int32_t resultNumber;
	ResponseStatus_TypeDef status;
	TokensList_TypeDef Tokens;
} Response_TypeDef;

#define DefaultRetriesCount		5

#define CHECK_RESPONSE(response) ((response)->status == ResponseStatusOk && (response)->resultNumber == RESULT_NUMBER_OK)

#define EXECUTE_COMMAND(command, response) ({ \
	uint32_t register r = DefaultRetriesCount; \
	if (response == NULL) { \
		Response_TypeDef ** response; \
		while (r--) { \
			*response = CommandExecuter_Execute(*GsmCommandExecuter, command); \
			if (CHECK_RESPONSE(*response)) \
				break; \
		} \
		CommandExecuter_ResponseRelease(*response); \
	} else { \
		while (r--) { \
			*response = CommandExecuter_Execute(*GsmCommandExecuter, command); \
			if (CHECK_RESPONSE(*response)) \
				break; \
		} \
	} \
	return r; \
})

AtCommandExecuter_TypeDef * CommandExecuter_Init(osMessageQId messageId);
void CommandExecuter_DeInit(AtCommandExecuter_TypeDef *commandExecuter);
Response_TypeDef * CommandExecuter_Execute(
		AtCommandExecuter_TypeDef commandExecuter, Command_TypeDef command);
void GetCommandString(char* commandText,
		AtCommandExecuter_TypeDef commandExecuter, Command_TypeDef command);
Response_TypeDef * ResponseReceived(AtCommandExecuter_TypeDef commandExecuter,
		unsigned int length);
void CommandExecuter_ResponseRelease(Response_TypeDef * response);
#endif /* ATCOMMANDPARSER_COMMAND_H_ */
