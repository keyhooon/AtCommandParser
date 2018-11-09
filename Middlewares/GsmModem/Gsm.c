/*
 * Gsm.c
 *
 *  Created on: Nov 5, 2018
 *      Author: HP
 */

#include "Gsm.h"
#include "command.h"

const CommandType_TypeDef REISSUES_THE_LAST_COMMAND_GIVEN = { "A/", 0, basic };
const CommandType_TypeDef ANSWER_AN_INCOMING_CALL = { "A", 20000, basic };
const CommandType_TypeDef MOBILE_ORGINATE_CALL_TO_DIAL_A_NUMBER = { "D", 20000,
		basic };
const CommandType_TypeDef ORGINATE_CALL_TO_PHONE_NUMBER_IN_CURRENT_MEMORY = {
		"D", 0, basic };
const CommandType_TypeDef ORGINATE_CALL_TO_PHONE_NUMBER_IN_MEMORY_WHICH_CORRESPONDS =
		{ "D", 0, basic };
const CommandType_TypeDef REDIAL_LAST_TELEPHONE_NUMBER_USED =
		{ "DL", 0, basic };
const CommandType_TypeDef SET_COMMAND_ECHO_MODE = { "E", 0, basic };
const CommandType_TypeDef DISCONNECT_EXISTING_CONNECTION = { "H", 20000, basic };
const CommandType_TypeDef DISPLAY_PRODUCT_IDENTIFICATION_INFORMATION = { "I",
		0, basic };
const CommandType_TypeDef SET_MONITOR_SPEAKER_LOUDNESS = { "L", 0, basic };
const CommandType_TypeDef SET_MONITOR_SPEAKER_MODE = { "M", 0, basic };
const CommandType_TypeDef SWITCH_FROM_DATA_MODE_OR_PPP_ONLINE_MODE_TO_COMMAND_MODE =
		{ "+++", 0, basic };
const CommandType_TypeDef SWITCH_FROM_COMMAND_MODE_TO_DATA_MODE = { "O", 0,
		basic };
const CommandType_TypeDef SELECT_PULSE_DIALING = { "P", 0, basic };
const CommandType_TypeDef SET_RESULT_CODE_PRESENTATION_MODE =
		{ "Q", 0, basic };
//const CommandType_TypeDef SET_NUMBER_OF_RINGS_BEFORE_AUTOMATICALY_ANSWERING_THE_CALL =
//		{ "ATA", 0, basic };
//const CommandType_TypeDef SET_COMMAND_LINE_TERMINATION_CHARACTER = { "ATA", 0,
//		basic };
//const CommandType_TypeDef SET_RESPONSE_FORMATTINF_CHARACTER =
//		{ "ATA", 0, basic };
//const CommandType_TypeDef SET_COMMAND_LINE_EDITING_CHARACTER =
//		{ "ATA", 0, basic };
//const CommandType_TypeDef PAUSE_BEFORE_BLIND_DIALING = { "ATA", 0, basic };
//const CommandType_TypeDef SET_NUMBER_OF_SECONDS_TO_WAIT_FOR_CONNECTION_COMPETION =
//		{ "ATA", 0, basic };
//const CommandType_TypeDef SET_NUMBER_OF_SECONDS_TO_WAIT_FOR_COMMA_DIAL_MODIFIER_ENCOUNTERED =
//		{ "ATA", 0, basic };
//const CommandType_TypeDef SET_DISCONNECT_DELAY_AFTER_INDICATING_THE_ABSENCE_OF_DATA_CARRIER =
//		{ "ATA", 0, basic };
const CommandType_TypeDef SELECT_TONE_DIALING = { "T", 0, basic };
const CommandType_TypeDef TA_RESPONSE_FORMAT = { "V", 0, basic };
const CommandType_TypeDef SET_CONNECT_RESULT_CODE_FORMAT_AND_MONITOR_CALL_PROGRESS =
		{ "X", 0, basic };
const CommandType_TypeDef RESET_DEFAULT_CONFIGURATION = { "Z", 0, basic };
const CommandType_TypeDef SET_DCD_FUNCTION_MODE = { "&C", 0, basic };
const CommandType_TypeDef SET_DTR_FUNCTION_MODE = { "&D", 0, basic };
const CommandType_TypeDef FACTORY_DEFIEND_CONFIGURATION = { "&F", 0, basic };
const CommandType_TypeDef DISPLAY_CURRENT_CONFIGURATION = { "&V", 0, basic };
const CommandType_TypeDef STORE_ACTIVE_PROFILE = { "&W", 0, basic };
const CommandType_TypeDef REQUEST_COMPLETE_TA_CAPABILITIES_LIST = { "GCAP", 0,
		extended };
const CommandType_TypeDef REQUEST_MANUFACTURER_IDENTIFICATION = { "GMI", 0,
		extended };
const CommandType_TypeDef REQUEST_TA_MODEL_IDENTIFICATION = { "GMM", 0, extended };
const CommandType_TypeDef REQUEST_TA_REVISION_IDENTIFICATION_OF_SOFTWARE_RELEASE =
		{ "GMR", 0, extended };
const CommandType_TypeDef REQUEST_GLOBAL_OBJECT_IDENTIFICATION = { "GOI", 0,
		extended };
const CommandType_TypeDef REQUEST_TA_SERIAL_NUMBER_IDENTIFICATION = { "GSN", 0,
		extended };
const CommandType_TypeDef SET_TE_TA_CONTROL_CHARACTER_FRAMING = { "ICF", 0,
		extended };
const CommandType_TypeDef SET_TE_TA_LOCAL_DATA_FLOW_CONTROL =
		{ "IFC", 0, extended };
const CommandType_TypeDef SET_TE_TA_FIXED_LOCAL_RATE = { "IPR", 0, extended };
const CommandType_TypeDef DISCONNECT_VOICE_CALL_ONLY = { "HVOIC", 0, extended };


AtCommandExecuter_TypeDef * GsmCommandExecuter;

void Gsm_Init()
{
	osMessageQId GsmMessageId;
	osMessageQDef(Gsm, 1, unsigned int);
	GsmMessageId = osMessageCreate(osMessageQ(Gsm), NULL);
	BSP_GSM_Init();
	GsmCommandExecuter = CommandExecuter_Init(GSM_IO_GetBuffer(), GsmMessageId,
			GSM_IO_Write);
}

void GSM_Proc()
{
	while(1)
	{
	osEvent event = osMessageGet(GsmCommandExecuter->messageId,
			osWaitForever);
		Response_TypeDef * result = 0;

		if (event.status == osEventMessage)
			result = ResponseReceived(GsmCommandExecuter, event.value.v);
		else {
			result = pvPortMalloc(sizeof(Response_TypeDef));
			result->status = ResponseStatusError_Timeout;
		}
	}
}

void GSM_DataReceivedCallback(uint32_t Length) {
	osMessagePut(GsmCommandExecuter->messageId, Length, osWaitForever);
}
