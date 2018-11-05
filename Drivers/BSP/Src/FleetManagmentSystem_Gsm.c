/*
 * FleetManagmentSystem_Gsm.c
 *
 *  Created on: Sep 3, 2017
 *      Author: home
 */

#include "FleetManagementSystem_Gsm.h"
#include "main.h"

void BSP_GSM_ErrorHandler();
extern const CommandType_TypeDef NetworkServiceCommand[9];
extern const CommandType_TypeDef GprsCommand[12];
extern const CommandType_TypeDef GeneralCommand[23];
extern const CommandType_TypeDef SimRelatedCommands[14];
extern const CommandType_TypeDef HttpCommands[6];
extern const CommandType_TypeDef TcpIpCommands[34];


osMessageQId GsmMessageId;
AtCommandExecuter_TypeDef * GsmCommandExecuter;



static inline int Extended_Execute_Command(CommandType_TypeDef commandType,
		Response_TypeDef ** response) {
	Command_TypeDef command = { commandType, Execute, NULL };
	return Run_AtCommand(command, response);
}

static inline int Extended_Write_Command(CommandType_TypeDef commandType,
		Response_TypeDef ** response, void ** Param) {
	Command_TypeDef command = { commandType, Write, Param };
	return Run_AtCommand(command, response);
}

static inline int Extended_Read_Command(CommandType_TypeDef commandType,
		Response_TypeDef ** response) {
	Command_TypeDef command = { commandType, Read, NULL };
	return Run_AtCommand(command, response);
}



void BSP_GSM_Init()
{
	osMessageQDef(Gsm, 1, unsigned int);
	GsmMessageId = osMessageCreate(osMessageQ(Gsm), NULL);
	GSM_IO_Init();
	GsmCommandExecuter = CommandExecuter_Init(GSM_IO_GetBuffer(), GsmMessageId,
			GSM_IO_Write);
}

void GSM_DataReceivedCallback(uint32_t Length) {
	osMessagePut(GsmMessageId, Length, osWaitForever);
}


void GSM_QueryVersionInformation(char * manufactureName, char * moduleName,
		char * firmwareVersion) {
	Response_TypeDef * response;
	if (Extended_Execute_Command(GeneralCommand[0], &response))
	{
		if (manufactureName)
			strcpy(manufactureName, response->Tokens.Items[0].String);
		if (moduleName)
			strcpy(moduleName, response->Tokens.Items[1].String);
		if (firmwareVersion)
			strcpy(firmwareVersion, response->Tokens.Items[2].String);
	}
	CommandExecuter_ResponseRelease(response);
}

void GSM_QueryImei(char * imei) {
	Response_TypeDef * response;
	if (Extended_Execute_Command(GeneralCommand[8], &response))
		strcpy(imei, response->Tokens.Items[0].String);
	CommandExecuter_ResponseRelease(response);
}


void GSM_QueryCcid(char * ccid) {
	Response_TypeDef * response;
	if (Extended_Execute_Command(SimRelatedCommands[7], &response))
		strcpy(ccid, response->Tokens.Items[0].String);
	CommandExecuter_ResponseRelease(response);
}

void GSM_QueryImsi(char * imsi) {
	Response_TypeDef * response;
	if (Extended_Execute_Command(SimRelatedCommands[0], &response))
		strcpy(imsi, response->Tokens.Items[0].String);
	CommandExecuter_ResponseRelease(response);
}

LockMode_TypeDef GSM_QuerySimLock() {
	Response_TypeDef * response;
	char * param[1] = { "2" };
	Extended_Write_Command(SimRelatedCommands[1], &response, param);
	LockMode_TypeDef lockMode = response->Tokens.Items[0].String[6];
	CommandExecuter_ResponseRelease(response);
	return lockMode;
}

LockMode_TypeDef GSM_LockSim(char *password, LockMode_TypeDef LockMode) {
	char * param[2] = { (LockMode == Lock ? "1" : "0"), password };
	Extended_Write_Command(SimRelatedCommands[1], NULL, param);
	return GSM_QuerySimLock();
}


SignalQualityReport GSM_QuerySignalQuality() {
	SignalQualityReport result;
	Response_TypeDef * response;
	if (Extended_Execute_Command(SimRelatedCommands[7], &response)) {
		result.Strength = response->Tokens.Items[0].String[5] - '0';
		if (response->Tokens.Items[0].String[6] == ',')
			result.BitErrorRate = response->Tokens.Items[0].String[7] - '0';
		else {
			result.Strength = result.Strength * 10
					+ response->Tokens.Items[0].String[6] - '0';
			result.BitErrorRate = response->Tokens.Items[0].String[8] - '0';
		}
	}
	CommandExecuter_ResponseRelease(response);
	return result;
}

NetworkRegistrationReport GSM_QueryRegisterationGsm() {
	NetworkRegistrationReport result;
	Response_TypeDef * response;
	if (Extended_Read_Command(NetworkServiceCommand[1], &response))
	{
		result.Registration = response->Tokens.Items[0].String[6] - '0';
		result.State = response->Tokens.Items[0].String[8] - '0';
	}
	CommandExecuter_ResponseRelease(response);
	return result;
}

NetworkRegistrationReport GSM_GsmRegisteration(
		NetworkRegistration_TypeDef registration) {
	Response_TypeDef * response;
	// Convert integer to string
	char * param[1] = { (registration + "0") };
	Extended_Write_Command(SimRelatedCommands[1], NULL, param);
	return GSM_QueryRegisterationGsm();
}

NetworkRegistrationReport GSM_QueryRegisterationGprs() {
	NetworkRegistrationReport result;
	Response_TypeDef * response;
	if (Extended_Read_Command(GprsCommand[9], &response)) {
		result.Registration = response->Tokens.Items[0].String[6] - '0';
		result.State = response->Tokens.Items[0].String[8] - '0';
	}
	CommandExecuter_ResponseRelease(response);
	return result;
}

NetworkRegistrationReport GSM_GprsRegisteration(
		NetworkRegistration_TypeDef registration) {
	Response_TypeDef * response;
	// Convert integer to string
	char * param[1] = { (registration + "0") };
	Extended_Write_Command(GprsCommand[9], NULL, param);
	return GSM_QueryRegisterationGprs();
}


/*

uint32_t BSP_GSM_GPRS_Init(void) {
if (GPRS_Init() == SIM_SUCCESS)
	return BOARD_OK;
return BOARD_ERROR;
}

uint32_t BSP_GSM_HTTP_Get(char *Address, uint8_t * Data, uint32_t *DataLength)
	{
char IP[15];
if (GPRS_Open(IP) == SIM_SUCCESS)
		{
	if (Http_Open(Address, 5) == SIM_SUCCESS) {
		uint32_t StatusCode, RetriesCount = 5;
		do {
			Http_GET(5, 5000, Data, &StatusCode, DataLength);
		} while (StatusCode != 200 && RetriesCount-- == 0);
		Http_Close();
	} else
			{
		GPRS_Close();
		return BOARD_ERROR;
	}
			GPRS_Close();
	return BOARD_OK;
		}
return BOARD_ERROR;
	}
uint32_t BSP_GSM_GPRS_Open() {
char IP[16];
if ((GPRS_Query(IP) == SIM_SUCCESS) || (GPRS_Open(IP) == SIM_SUCCESS))
	return BOARD_OK;
		return BOARD_ERROR;
}
uint32_t BSP_GSM_GPRS_Close() {
if (GPRS_Close() == SIM_SUCCESS)
	return BOARD_OK;
		return BOARD_ERROR;
}

uint32_t BSP_GSM_HTTP_Post(char *Address, uint8_t * RequestData,
	uint32_t RequestDataLength, uint8_t * ResponseData,
	uint8_t * ResponseDataLength, uint32_t * StatusCode) {
if (Http_Open(Address, 5) != SIM_SUCCESS)
	return BOARD_ERROR;
if (Http_Post(RequestData, RequestDataLength, StatusCode, ResponseData,
		ResponseDataLength, 5, 50000) != SIM_SUCCESS)
	return BOARD_ERROR;
Http_Close();
return BOARD_OK;
}
 */

void BSP_GSM_ErrorHandler() {
while (1) {
}

}


