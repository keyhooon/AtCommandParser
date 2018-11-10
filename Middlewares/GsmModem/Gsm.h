/*
 * Gsm.h
 *
 *  Created on: Nov 5, 2018
 *      Author: HP
 */

#ifndef GSMMODEM_GSM_H_
#define GSMMODEM_GSM_H_

#include "CommandTokenizer.h"
#include "AtCommandExecuter.h"
#include "cmsis_os.h"

#define CARRIAGE_RETURN_CODE 		"\r"	// Carriage return
#define LINE_FEED_CODE				"\n"	// Line feed

#define COMMAND_LINE_TERMINATION_CAHR_DEFAULT	CARRIAGE_RETURN_CODE
#define RESPONSE_FORMATTING_CHAR_DEFAULT		LINE_FEED_CODE
#define COMMAND_ECHO_DEFAULT					1
#define RESPONSE_FORMAT_DEFAULT					1

typedef struct Gsm_Struct {
	CommandExecuter_TypeDef * commandExecuter;
	CommandTokenizer_TypeDef * commandTokenizer;
	osThreadId taskHandle;
} Gsm_TypeDef;

extern Gsm_TypeDef *GsmModem;
#define DefaultRetriesCount		5

#define CHECK_RESPONSE(response) ((response).status == ResponseStatusOk && (response).resultNumber == RESULT_NUMBER_OK)

#define EXECUTE_COMMAND(command, response) ({ \
	uint32_t register r = DefaultRetriesCount; \
	while (r--) { \
		*response = CommandExecuter_Execute(GsmModem->commandExecuter, command); \
		if (CHECK_RESPONSE(*response)) \
			break; \
	} \
	return r; \
})
#define EXECUTE_COMMAND_EX(commandType, action, parameters, response) ({ \
		Command_TypeDef command = {commandType, action, parameters}; \
		EXECUTE_COMMAND(command, response); \
})


#define EXECUTE_COMMAND_RESPONSELESS(command) ({ \
	uint32_t register r = DefaultRetriesCount; \
	Response_TypeDef res \
	while (r--) { \
		res = CommandExecuter_Execute(GsmModem->commandExecuter, command); \
		if (CHECK_RESPONSE(res)) \
			break; \
	} \
	CommandTokenizer_FreeTokenList(res.Tokens); \
	return r; \
})

#define EXECUTE_COMMAND_RESPONSELESS_EX(commandType, action, parameters) ({ \
		Command_TypeDef command = {commandType, action, parameters}; \
		EXECUTE_COMMAND_EX(command); \
})



#endif /* GSMMODEM_GSM_H_ */
