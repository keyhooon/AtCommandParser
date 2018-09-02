/*
 * Sim800.c
 *
 *  Created on: Sep 15, 2017
 *      Author: home
 */

#include "Sim800.h"


#define SELECT_SMS_MESSAGE_FORMAT			(const char *) "CMGF"
#define SELECT_TE_CHARACTER_SET				(const char *) "CSCS"
#define SEND_SMS_MESSAGE					(const char *) "CMGS"

#define SET_COMMAND_ECHO_MODE				(const char *) "E"
#define RESET_DEFAULT_CONFIGURATION			(const char *) "Z"
#define SIGNAL_QUALITY_REPORT				(const char *) "CSQ"
#define NETWORK_REGISTRATION				(const char *) "CREG"
#define REQUEST_TA_SERIAL_NUMBER			(const char *) "GSN"

/* AT Commands for IP Application */
#define BEARER_SETTINGS_FOR_APPLICATION_BASED_ON_IP				(const char *) "SAPBR"

/* AT Commands for HTTP Application */
#define INITIALIZE_HTTP_SERVICE									(const char *) "HTTPINIT"
#define TERMINATE_HTTP_SERVICE									(const char *) "HTTPTERM"
#define SET_HTTP_PARAMETERS_VALUE								(const char *) "HTTPPARA"
#define INPUT_HTTP_DATA											(const char *) "HTTPDATA"
#define HTTP_METHOD_ACTION										(const char *) "HTTPACTION"
#define READ_THE_HTTP_SERVER_RESPONSE							(const char *) "HTTPREAD"
#define SAVE_HTTP_APLICATION_CONTEXT							(const char *) "HTTPSCONT"
#define READ_HTTP_STATUS										(const char *) "HTTPSTATUS"
#define READ_THE_HTTP_HEADER_INFORMATION_OF_SERVER_RESPONSE		(const char *) "HTTPHEAD"


const char *Sim_Result_Text[10]={SIM_OK_TEXT, SIM_CONNECT_TEXT, SIM_RING_TEXT, SIM_NO_CARRIER_TEXT, SIM_ERROR_TEXT,
								SIM_NO_DIALTONE_TEXT, SIM_BUSY_TEXT, SIM_NO_ANSWER_TEXT, SIM_PROCEEDING_TEXT, SIM_DOWNLOAD_TEXT};
const SIM_Receive_Option_TypeDef DefaultReceiveOption =
{
		SIM_OK | SIM_ERROR,
		2000,
		10000
};
const char * Bearer_Parameter[6] = { CONTYPE_BEARER_PARAMETER,
		APN_BEARER_PARAMETER,
		USER_BEARER_PARAMETER,
		PWD_BEARER_PARAMETER,
		PHONENUM_BEARER_PARAMETER,
		RATE_BEARER_PARAMETER};

const char *Http_Parameter[10] = {
		 URL_HTTP_PARAMETER,
		UA_HTTP_PARAMETER,
		PROIP_HTTP_PARAMETER,
		PROPORT_HTTP_PARAMETER,
		REDIR_HTTP_PARAMETER,
		BREAK_HTTP_PARAMETER,
		BREAKEND_HTTP_PARAMETER,
		TIMEOUT_HTTP_PARAMETER,
		CONTENT_HTTP_PARAMETER,
		USERDATA_HTTP_PARAMETER};


SIM_Receive_Buffer_TypeDef Sim_Buffer_Response;

/*

#define Perform_Basic_Command(CommandCode, Argument, ReceiveOption)			({  \
	char Sim_Buffer_Command[100];												\
	sprintf(Sim_Buffer_Command,"AT%s%d\r\n", CommandCode, Argument);			\
	GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption); \
	Sim_Buffer_Response.Result;													\
})
#define Perform_Parameter_Command(RegisterIndex, value, ReceiveOption)		({  \
		char Sim_Buffer_Command[100];											\
		sprintf(Sim_Buffer_Command, "ATS%d=%d\r\n", RegisterIndex, value);		\
		GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption); \
		Sim_Buffer_Response.Result;												\
})
#define Perform_Test_Command(CommandCode, ReceiveOption)					({	\
		char Sim_Buffer_Command[100];											\
		sprintf(Sim_Buffer_Command, "AT+%s=?\r\n", CommandCode);				\
		GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption); \
		Sim_Buffer_Response.Result;												\
})
#define Perform_Read_Command(CommandCode, ReceiveOption)					({	\
		char Sim_Buffer_Command[100];											\
		sprintf(Sim_Buffer_Command, "AT+%s?\r\n", CommandCode);					\
		GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption); \
		Sim_Buffer_Response.Result;												\
})
#define Perform_Write_Command(CommandCode, Parameters, ReceiveOption)		({  \
		char Sim_Buffer_Command[100];											\
		sprintf(Sim_Buffer_Command, "AT+%s=%s\r\n", CommandCode, Parameters);	\
		GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption); \
		Sim_Buffer_Response.Result;												\
})
#define Perform_Execution_Command(CommandCode, ReceiveOption)				({	\
		char Sim_Buffer_Command[100];											\
		sprintf(Sim_Buffer_Command, "AT+%s\r\n", CommandCode);					\
		if(GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption) == 1) \
		while(1){};																		\
		Sim_Buffer_Response.Result;												\
})
*/

/*
#define Perform_Write_Data(Data, Length, ReceiveOption)						({	\
		GSM_IO_Run_AT_Command(Data, Length, & Sim_Buffer_Response, ReceiveOption); \
		Sim_Buffer_Response.Result;												\
})
*/

static inline uint32_t Perform_Basic_Command(const char * Command, int32_t Argument, SIM_Receive_Option_TypeDef ReceiveOption);
static inline uint32_t Perform_Parameter_Command(uint8_t RegisterIndex, uint8_t value, SIM_Receive_Option_TypeDef ReceiveOption);
static inline uint32_t Perform_Test_Command(const char * CommandCode, SIM_Receive_Option_TypeDef ReceiveOption);
static inline uint32_t Perform_Read_Command(const char * CommandCode, SIM_Receive_Option_TypeDef ReceiveOption);
static inline uint32_t Perform_Write_Command(const char * CommandCode, char * Parameters, SIM_Receive_Option_TypeDef ReceiveOption);
static inline uint32_t Perform_Execution_Command(const char * CommandCode, SIM_Receive_Option_TypeDef ReceiveOption);
static inline uint32_t Perform_Write_Data(const char * Data,uint16_t Length, SIM_Receive_Option_TypeDef ReceiveOption);

uint32_t GSM_Init()
{
	GSM_IO_Init();
	uint32_t RetriesCount = 2;
	while (Perform_Basic_Command(RESET_DEFAULT_CONFIGURATION, 0, DefaultReceiveOption) != SIM_OK)
	{
		if (RetriesCount -- == 0)
			return SIM_Error_General_Reset_Configuration;
	}
	return SIM_SUCCESS;
}
uint32_t GPRS_Init()
{
	uint32_t RetriesCount = 5;
	while (GPRS_Config(0,"GPRS") != SIM_SUCCESS)
	{
		if (RetriesCount -- == 0)
			return SIM_Error_General_Reset_Configuration;
	}
	while (GPRS_Config(1,"mtnirancell") != SIM_SUCCESS)
	{
		if (RetriesCount -- == 0)
			return SIM_Error_General_Reset_Configuration;
	}
	return SIM_SUCCESS;
}

uint32_t GetSignalLevel()
{
	if (Perform_Execution_Command(SIGNAL_QUALITY_REPORT, DefaultReceiveOption) == SIM_OK)
		return SIM_SUCCESS;
	return SIM_Error_General_Get_Signal;
}

uint32_t GetImei(uint64_t *imei)
{
	if (Perform_Execution_Command(SIGNAL_QUALITY_REPORT, DefaultReceiveOption) == SIM_OK)
	{
		char * token = Sim_Buffer_Response.TokenList[Sim_Buffer_Response.TokenListSize-2];
		*imei = atol(token);
		return SIM_SUCCESS;
	}
	return SIM_Error_General_Get_Imei;
}



 /* Bearer Configure */

uint32_t GPRS_Open(char *ip)
{
	SIM_Receive_Option_TypeDef ReceiveOption = DefaultReceiveOption;
	ReceiveOption.WaitForSpecifiedTime = 50000;
	Perform_Write_Command(BEARER_SETTINGS_FOR_APPLICATION_BASED_ON_IP,"1,1", ReceiveOption);
	return GPRS_Query(ip);
}
uint32_t GPRS_Query(char * ip)
{
	if (Perform_Write_Command(BEARER_SETTINGS_FOR_APPLICATION_BASED_ON_IP,"2,1", DefaultReceiveOption) == SIM_OK)
	{
		char * token = Sim_Buffer_Response.TokenList[Sim_Buffer_Response.TokenListSize-2];
		strtok (token, "\"");
		char * temp = (strtok (0, "\""));
		strcpy(ip,temp);
		if(strcmp(ip,"0.0.0.0") == 0)
			return SIM_Error_GPRS_Problem;
		return SIM_SUCCESS;
	}
	return SIM_Error_GPRS_Problem;
}
uint32_t GPRS_Close()
{
	SIM_Receive_Option_TypeDef ReceiveOption = DefaultReceiveOption;
	ReceiveOption.WaitForSpecifiedTime = 50000;
	if (Perform_Write_Command(BEARER_SETTINGS_FOR_APPLICATION_BASED_ON_IP,"0,1", ReceiveOption) == SIM_OK)
		return SIM_SUCCESS;
	return SIM_Error_GPRS_Problem;
}

uint32_t GPRS_Config(uint8_t BearerParameter, char * BearerValue)
{
	char cmd[COMMAND_BUFFER_SIZE];
	sprintf(cmd,"3,1,\"%s\",\"%s\"",Bearer_Parameter[BearerParameter],BearerValue);
	if (Perform_Write_Command(BEARER_SETTINGS_FOR_APPLICATION_BASED_ON_IP,cmd, DefaultReceiveOption)== SIM_OK)
		return SIM_SUCCESS;
	return SIM_Error_GPRS_Problem;
}



 /* HTTP Method */

uint32_t Http_Open(char * HttpAddress, uint32_t RetriesCount)
{

	uint32_t result  ;
	while(Perform_Execution_Command(INITIALIZE_HTTP_SERVICE, DefaultReceiveOption) != SIM_OK)
	{
		Http_Close();
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_INIT;
	}
	char temp[50];

	while(Perform_Write_Command(SET_HTTP_PARAMETERS_VALUE,"\"CID\",1", DefaultReceiveOption) != SIM_OK)
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_PARAMETER_SETUP;
	sprintf(temp,"\"%s\",\"%s\"",URL_HTTP_PARAMETER, HttpAddress);
	while(Perform_Write_Command(SET_HTTP_PARAMETERS_VALUE, temp, DefaultReceiveOption) != SIM_OK)
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_PARAMETER_SETUP;
	sprintf(temp,"\"%s\",\"application/octet-stream\"",CONTENT_HTTP_PARAMETER);
	while(Perform_Write_Command(SET_HTTP_PARAMETERS_VALUE, temp, DefaultReceiveOption) != SIM_OK)
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_PARAMETER_SETUP;
	sprintf(temp,"\"%s\",\"30\"",TIMEOUT_HTTP_PARAMETER);
	while(Perform_Write_Command(SET_HTTP_PARAMETERS_VALUE, temp, DefaultReceiveOption) != SIM_OK)
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_PARAMETER_SETUP;
	return SIM_SUCCESS;
}
uint32_t Http_Post(uint8_t * Data, uint32_t Length, uint32_t *ResponseStatusCode, uint8_t *Response, uint8_t *ResponseLength, uint32_t RetriesCount, uint32_t Timeout)
{
	uint32_t result ;
	char param[15];
	char response[100];
	SIM_Receive_Option_TypeDef ReceiveOption = DefaultReceiveOption;
	ReceiveOption.WaitForSpecifiedResponse = SIM_DOWNLOAD | SIM_ERROR;
	sprintf(param,"%d,%d", Length, Timeout);
	do{
		result = Perform_Write_Command(INPUT_HTTP_DATA, param, ReceiveOption);
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_WRITE_DATA;
	} while (result != SIM_DOWNLOAD);

	ReceiveOption = DefaultReceiveOption;
	ReceiveOption.WaitForSpecifiedTime = Timeout;
	do{
		result = Perform_Write_Data(Data, Length, ReceiveOption);
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_WRITE_DATA;
	} while (result!= SIM_OK);

	do {
		result = Perform_Write_Command(HTTP_METHOD_ACTION, "1", DefaultReceiveOption);
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_SEND_POST;
	} while (result != SIM_OK);

	while (GSM_IO_Wait_for_Response(response,30000) == 0)
	{
		if (memcmp(response, "+HTTPACTION",strlen("+HTTPACTION"))==0)
		{
			strtok (response, ",");
			* ResponseStatusCode = atoi(strtok (0, ","));
			* ResponseLength = atoi(strtok (0, ","));
			if (ResponseLength == 0)
				return SIM_SUCCESS;
			// if Response get, Response Length higher than zero, and we should take response and pass it.
			Perform_Execution_Command(READ_THE_HTTP_SERVER_RESPONSE, DefaultReceiveOption);
			if (strlen(Sim_Buffer_Response.TokenList[Sim_Buffer_Response.TokenListSize - 2]) >= HTTP_RESPONSE_MAXIMUM_LENGTH)
				*(Sim_Buffer_Response.TokenList[Sim_Buffer_Response.TokenListSize - 2] + HTTP_RESPONSE_MAXIMUM_LENGTH - 1) = 0;
			strcpy(Response,Sim_Buffer_Response.TokenList[Sim_Buffer_Response.TokenListSize - 2]);
			return SIM_SUCCESS;
		}
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_POST_RESPONSE_NOT_SUPPORTED;
	}

	return SIM_Error_HTTP_NOT_RESPONDING_POST;
}
uint32_t Http_GET(uint32_t RetriesCount, uint32_t Timeout,uint8_t * ResponseData, uint32_t * ResponseStatusCode, uint32_t * ResponseLength)
{
	char response[100];
	uint32_t result ;
	do
	{
		result = Perform_Write_Command(HTTP_METHOD_ACTION, "0", DefaultReceiveOption);
		if (RetriesCount -- == 0)
			return SIM_Error_HTTP_SEND_GET;
	} while (result != SIM_OK);

	while (GSM_IO_Wait_for_Response(response,5000) == 0)
	{
		if (memcmp(response, "+HTTPACTION",strlen("+HTTPACTION"))==0)
		{
			strtok (response, ",");
			* ResponseStatusCode = atoi((strtok (0, ",")));
			* ResponseLength = atoi((strtok (0, ",")));
			if (* ResponseStatusCode == 200)
			{
				do
				{
					result = Perform_Execution_Command(READ_THE_HTTP_SERVER_RESPONSE, DefaultReceiveOption);
					if (RetriesCount -- < 0)
						return SIM_Error_HTTP_WRITE_DATA;
				} while (result!= SIM_OK);
				strtok (Sim_Buffer_Response.TokenList[Sim_Buffer_Response.TokenListSize-3], "+HTTPREAD: ");
				memcpy(ResponseData, Sim_Buffer_Response.TokenList[Sim_Buffer_Response.TokenListSize-2], * ResponseLength);
			}
			return SIM_SUCCESS;
		}
		if (RetriesCount -- < 0)
			break;
	}
	return SIM_Error_HTTP_POST_RESPONSE_NOT_SUPPORTED;

}
uint32_t Http_Close()
{
	if (Perform_Execution_Command(TERMINATE_HTTP_SERVICE, DefaultReceiveOption)==SIM_OK)
		return SIM_SUCCESS;
	return SIM_Error_HTTP_TERMiNATE;

}




static inline uint32_t Perform_Basic_Command(const char * CommandCode, int32_t Argument, SIM_Receive_Option_TypeDef ReceiveOption)
{
	char Sim_Buffer_Command[COMMAND_BUFFER_SIZE];
	sprintf(Sim_Buffer_Command,"AT%s%d\r\n", CommandCode, Argument);
	GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption);
	return Sim_Buffer_Response.Result;
}

static inline uint32_t Perform_Parameter_Command(uint8_t RegisterIndex, uint8_t value, SIM_Receive_Option_TypeDef ReceiveOption)
{
	char Sim_Buffer_Command[COMMAND_BUFFER_SIZE];
	sprintf(Sim_Buffer_Command, "ATS%d=%d\r\n", RegisterIndex, value);
	GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption);
	return Sim_Buffer_Response.Result;
}

static inline uint32_t Perform_Test_Command(const char * CommandCode, SIM_Receive_Option_TypeDef ReceiveOption)
{
	char Sim_Buffer_Command[COMMAND_BUFFER_SIZE];
	sprintf(Sim_Buffer_Command, "AT+%s=?\r\n", CommandCode);
	GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption);
	return Sim_Buffer_Response.Result;
}

static inline uint32_t Perform_Read_Command(const char * CommandCode, SIM_Receive_Option_TypeDef ReceiveOption)
{
	char Sim_Buffer_Command[COMMAND_BUFFER_SIZE];
	sprintf(Sim_Buffer_Command, "AT+%s?\r\n", CommandCode);
	GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption);
	return Sim_Buffer_Response.Result;
}

static inline uint32_t Perform_Write_Command(const char * CommandCode, char * Parameters, SIM_Receive_Option_TypeDef ReceiveOption)
{
	char Sim_Buffer_Command[COMMAND_BUFFER_SIZE];
	sprintf(Sim_Buffer_Command, "AT+%s=%s\r\n", CommandCode, Parameters);
	GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption);
	return Sim_Buffer_Response.Result;
}


static inline uint32_t Perform_Execution_Command(const char * CommandCode, SIM_Receive_Option_TypeDef ReceiveOption)
{
	char Sim_Buffer_Command[COMMAND_BUFFER_SIZE];
	sprintf(Sim_Buffer_Command, "AT+%s\r\n", CommandCode);
	GSM_IO_Run_AT_Command(Sim_Buffer_Command, strlen(Sim_Buffer_Command), & Sim_Buffer_Response, ReceiveOption);
	return Sim_Buffer_Response.Result;
}

static inline uint32_t Perform_Write_Data(const char * Data,uint16_t Length, SIM_Receive_Option_TypeDef ReceiveOption)
{
	GSM_IO_Run_AT_Command(Data, Length, & Sim_Buffer_Response, ReceiveOption);
	return Sim_Buffer_Response.Result;
}



int8_t GSM_IO_Run_AT_Command(char* Command, uint32_t Length, SIM_Receive_Buffer_TypeDef * ReceiveBuffer, SIM_Receive_Option_TypeDef ReceiveOption)
{
	uint32_t tokenStartidx = 0;
	uint32_t idx = 0;
	char * ResultListText[8];
	uint32_t ResultList[8];
	int	ResultListCount = 0;
	ReceiveBuffer->TokenListSize = 0;
	ReceiveBuffer->ReceiveStatus = Receiving;
	ReceiveBuffer->Result = 0;

	if (ReceiveOption.WaitForSpecifiedTime)
		ReceiveOption.Timeout = ReceiveOption.WaitForSpecifiedTime;
	for (int i = 0 ; i < 10 ; i++)
		if ((ReceiveOption.WaitForSpecifiedResponse>>i & 1) == 1)
		{
			ResultListText[ResultListCount] = Sim_Result_Text[i];
			ResultList[ResultListCount++] = 1 << i;
		}

	memset(ReceiveBuffer->Response,'\0',RESPONSE_BUFFER_SIZE);

	/* Send the command */
	if (GSM_IO_Buffer_Reset() == 1 || GSM_IO_Write(Command, Length) == 1)
	{
		return 1;
	}

	/* Wait for reception */
	while (1)
	{
		/* Wait to recieve data */
		if (GSM_IO_Read(&ReceiveBuffer->Response[idx], 1, ReceiveOption.Timeout) == 0)
			return 	(ReceiveOption.WaitForSpecifiedTime ? 0: 1);
		if ( ReceiveBuffer->Response[idx] =='\r' || ReceiveBuffer->Response[idx] =='\n')
		{
			ReceiveBuffer->Response[idx] = '\0';
			if (tokenStartidx < idx)
			{
				ReceiveBuffer->TokenList[ReceiveBuffer->TokenListSize++] = & ReceiveBuffer->Response[tokenStartidx];
				for (int i = 0 ; i < ResultListCount ; i++ )
					if (strcmp(ReceiveBuffer->TokenList[ReceiveBuffer->TokenListSize - 1], ResultListText[i]) == 0)
					{
						ReceiveBuffer->Result = ResultList[i];
						ReceiveBuffer->ReceiveStatus = SpecifiedResponseOccure;
						return 0;
					}
			}
			tokenStartidx = idx + 1;
		}
		idx ++;
	/* Check that max buffer size has not been reached */
    if (idx == RESPONSE_BUFFER_SIZE)
    {
      return 1;
    }
  }

  return 0;
}




/**
  * @brief  Wait for Response has coming
  * @param  Response string that received.
  * @param  Timeout the maximum Time to receive.
  * @retval Returns BOARD_OK on success and BOARD_ERROR otherwise.
  */
uint8_t GSM_IO_Wait_for_Response(char * Response, uint32_t Timeout)
{
	char response[100];
	uint32_t tokenStartidx = 0;
	uint32_t idx = 0;
	uint8_t RxChar;
	while (1)
		{
			/* Wait to recieve data */
			if (GSM_IO_Read(&RxChar, 1, Timeout) != 0)
				response[idx] = RxChar;
			else
				return 	1;
			if ( response[idx] =='\r' || response[idx] =='\n')
			{
				response[idx] = '\0';
				if (tokenStartidx < idx)
				{
					strcpy(Response, & response[tokenStartidx]);
					Response = & response[tokenStartidx];
					return 0;
				}
				tokenStartidx = idx + 1;
			}
			idx ++;
		/* Check that max buffer size has not been reached */
	    if (idx == SIM_BUFFER_SIZE)
	    {
	      return 1;
	    }
	  }
}




