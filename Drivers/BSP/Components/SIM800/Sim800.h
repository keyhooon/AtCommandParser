/*
 * Sim800.h
 *
 *  Created on: Sep 15, 2017
 *      Author: home
 */


#ifndef BSP_COMPONENTS_SIM800_SIM800_H_
#define BSP_COMPONENTS_SIM800_SIM800_H_

#include "stdint.h"


#define RESPONSE_BUFFER_SIZE				100
#define COMMAND_BUFFER_SIZE					50

#define CLOSE_BEARER						0
#define OPEN_BEARER							1
#define QUERY_BEARER						2
#define SET_BEARER_PARAMETERS				3
#define GET_BEARER_PARAMETER				4

#define CONTYPE_BEARER_PARAMETER			(const char *) "CONTYPE"
#define APN_BEARER_PARAMETER				(const char *) "APN"
#define USER_BEARER_PARAMETER				(const char *) "USER"
#define PWD_BEARER_PARAMETER				(const char *) "PWD"
#define PHONENUM_BEARER_PARAMETER			(const char *) "PHONENUM"
#define RATE_BEARER_PARAMETER				(const char *) "RATE"


#define URL_HTTP_PARAMETER					(const char *) "URL"
#define UA_HTTP_PARAMETER					(const char *) "UA"
#define PROIP_HTTP_PARAMETER				(const char *) "PROIP"
#define PROPORT_HTTP_PARAMETER				(const char *) "PROPORT"
#define REDIR_HTTP_PARAMETER				(const char *) "REDIR"
#define BREAK_HTTP_PARAMETER				(const char *) "BREAK"
#define BREAKEND_HTTP_PARAMETER				(const char *) "BREAKEND"
#define TIMEOUT_HTTP_PARAMETER				(const char *) "TIMEOUT"
#define CONTENT_HTTP_PARAMETER				(const char *) "CONTENT"
#define USERDATA_HTTP_PARAMETER				(const char *) "USERDATA"

#define GET_HTTP_METHOD						0
#define POST_HTTP_METHOD					1
#define HEAD_HTTP_METHOD					2

extern const char * Bearer_Parameter[6];

extern const char * Http_Parameter[10];




#define SIM_SUCCESS									0
#define SIM_Error_HTTP_INIT							2
#define SIM_Error_HTTP_PARAMETER_SETUP				3
#define SIM_Error_HTTP_WRITE_DATA					4
#define SIM_Error_HTTP_SEND_POST					5
#define SIM_Error_HTTP_POST_RESPONSE_NOT_SUPPORTED	6
#define SIM_Error_HTTP_NOT_RESPONDING_POST			7
#define SIM_Error_HTTP_TERMiNATE					8
#define SIM_Error_HTTP_SEND_GET						9
#define SIM_Error_General_Reset_Configuration		10
#define SIM_Error_General_Echo_						11
#define SIM_Error_IO_Problem						12
#define SIM_Error_GPRS_Problem						13
#define SIM_Error_General_Get_Signal				14
#define SIM_Error_General_Get_Imei					15






/* Acknowledges execution of a Command
 * */
#define SIM_OK								1
#define SIM_OK_TEXT							(const char *) "OK"

/* A connection has been established; the DCE is moving from
 * Command state to online data state
 * */
#define SIM_CONNECT							2
#define SIM_CONNECT_TEXT					(const char *) "CONNECT"


/* The DCE has detected an incoming call signal from
 * network
 * */
#define SIM_RING							4
#define SIM_RING_TEXT						(const char *) "RING"

/* The connection has been terminated or the attempt to
 * establish a connection failed
 * */
#define SIM_NO_CARRIER						8
#define SIM_NO_CARRIER_TEXT					(const char *) "NO CARRIER"
/* Command not recognized, Command line maximum length
 * exceeded, parameter value invalid, or other problem with
 * processing the Command line
 * */
#define SIM_ERROR							16
#define SIM_ERROR_TEXT						(const char *) "ERROR"


/* No dial tone detected
 * */
#define SIM_NO_DIALTONE						32
#define SIM_NO_DIALTONE_TEXT				(const char *) "NO DIALTONE"


/* Engaged (busy) signal detected
 * */
#define SIM_BUSY							64
#define SIM_BUSY_TEXT						(const char *) "BUSY"
/* "@" (Wait for Quiet Answer) dial modifier was used, but
 * remote ringing followed by five seconds of silence was not
 * detected before expiration of the connection timer (S7)
 * */
#define SIM_NO_ANSWER						128
#define SIM_NO_ANSWER_TEXT					(const char *) "NO ANSWER"

/* "@" (Wait for Quiet Answer) dial modifier was used, but
 * remote ringing followed by five seconds of silence was not
 * detected before expiration of the connection timer (S7)
 * */
#define SIM_CONNECT							256
#define SIM_CONNECT_TEXT					(const char *) "CONNECT"




#define SIM_WAIT_FOREVER					HAL_MAX_DELAY
#define SIM_BUFFER_SIZE  					0x3ff
#define HTTP_RESPONSE_MAXIMUM_LENGTH		64

extern const char *Sim_Result_Text[10];
typedef enum
{
	ReceiveIdle = 0,
	Receiving = 1,
	SpecifiedResponseOccure = 2,
	SpecifiedTimeOccure = 3,
	TimeoutOccure = 4,
	BufferOverflowOccure = 5,
}SIM_Receive_Status_TypeDef;

typedef enum
{
	BasicCommand,
	ParameterCommand,
	TestCommand,
	ReadCommand,
	WriteCommand,
	ExecutionCommand,
}SIM_Command_Type_TypeDef;



typedef struct
{
	uint32_t 				WaitForSpecifiedResponse;
	uint32_t 				WaitForSpecifiedTime;
	uint32_t 				Timeout;
}SIM_Receive_Option_TypeDef;


typedef struct
{
	char Response[RESPONSE_BUFFER_SIZE];
	char *TokenList[10];
	uint8_t TokenListSize;
	uint32_t Result;
	SIM_Receive_Status_TypeDef ReceiveStatus;

}SIM_Receive_Buffer_TypeDef;






uint32_t GSM_Init();
uint32_t GPRS_Init();

/* Bearer Configure */
uint32_t GPRS_Open(char *IP);
uint32_t GPRS_Close();
uint32_t GPRS_Query(char *Data);
uint32_t GPRS_Config(uint8_t BearerParameter, char * BearerValue);

/* HTTP Method */
uint32_t Http_Open(char * HttpAddress, uint32_t RetriesCount);
uint32_t Http_GET(uint32_t RetriesCount, uint32_t Timeout,uint8_t * ResponseData, uint32_t * ResponseStatusCode, uint32_t * ResponseLength);
uint32_t Http_Post(uint8_t * Data, uint32_t Length, uint32_t *ResponseStatusCode, uint8_t *Response, uint8_t *ResponseLength, uint32_t RetriesCount, uint32_t Timeout);
uint32_t Http_Close();

uint32_t GetSignalLevel();
uint32_t GetImei(uint64_t *imei);

void GSM_IO_Init(void);
void GSM_IO_PowerDown(void);
void GSM_IO_PowerUp(void);
void GSM_IO_Delay(uint32_t Delay);
int8_t GSM_IO_Write(char * Command, uint32_t Length);
int32_t GSM_IO_Read(uint8_t* Buffer, uint32_t Length, uint32_t TimeOut );
int32_t GSM_IO_Buffer_Reset(void);
int8_t GSM_IO_Run_AT_Command(char* cmd, uint32_t Length, SIM_Receive_Buffer_TypeDef * ReceiveBuffer, SIM_Receive_Option_TypeDef ReceiveOption);
uint8_t GSM_IO_Wait_for_Response(char * Response, uint32_t Timeout);

#endif /* BSP_COMPONENTS_SIM800_SIM800_H_ */
