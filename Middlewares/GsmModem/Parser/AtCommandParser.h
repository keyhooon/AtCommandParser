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


typedef enum {
	final, intermediate, unsolicited,
} ResponseType_TypeDef;



typedef struct {
	const uint8_t number;
	const char * code;
	const ResponseType_TypeDef type;
} ResponseResult_TypeDef;

typedef enum {
	ResponseStatusOk,
	ResponseStatusError_Timeout,
	ResponseStatusError_ResultIsntThere,
	ResponseStatusError_BadFormat,
} ResponseStatus_TypeDef;

typedef struct {
	int32_t resultNumber;
	ResponseStatus_TypeDef status;
	CommandTokensList_TypeDef Tokens;
} Response_TypeDef;


#endif /* ATCOMMANDPARSER_COMMAND_H_ */
