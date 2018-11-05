/*
 * Gsm.c
 *
 *  Created on: Nov 5, 2018
 *      Author: HP
 */

#include "Gsm.h"
#include "command.h"

const CommandType_TypeDef REISSUES_THE_LAST_COMMAND_GIVEN = { "A/", 0, basic };
const CommandType_TypeDef ANSWER_AN_INCOMING_CALL = { "ATA", 0, basic };
const CommandType_TypeDef MOBILE_ORGINATE_CALL_TO_DIAL_A_NUMBER = { "ATA", 0,
		basic };
const CommandType_TypeDef ORGINATE_CALL_TO_PHONE_NUMBER_IN_CURRENT_MEMORY = {
		"ATA", 0, basic };
const CommandType_TypeDef ORGINATE_CALL_TO_PHONE_NUMBER_IN_MEMORY_WHICH_CORRESPONDS =
		{ "ATA", 0, basic };
const CommandType_TypeDef REDIAL_LAST_TELEPHONE_NUMBER_USED =
		{ "ATA", 0, basic };
const CommandType_TypeDef SET_COMMAND_ECHO_MODE = { "ATA", 0, basic };
const CommandType_TypeDef DISCONNECT_EXISTING_CONNECTION = { "ATA", 0, basic };
const CommandType_TypeDef DISPLAY_PRODUCT_IDENTIFICATION_INFORMATION = { "ATA",
		0, basic };
const CommandType_TypeDef SET_MONITOR_SPEAKER_LOUDNESS = { "ATA", 0, basic };
const CommandType_TypeDef SET_MONITOR_SPEAKER_MODE = { "ATA", 0, basic };
const CommandType_TypeDef SWITCH_FROM_DATA_MODE_OR_PPP_ONLINE_MODE_TO_COMMAND_MODE =
		{ "ATA", 0, basic };
const CommandType_TypeDef SWITCH_FROM_COMMAND_MODE_TO_DATA_MODE = { "ATA", 0,
		basic };
const CommandType_TypeDef SELECT_PULSE_DIALING = { "ATA", 0, basic };
const CommandType_TypeDef SET_RESULT_CODE_PRESENTATION_MODE =
		{ "ATA", 0, basic };
const CommandType_TypeDef SET_NUMBER_OF_RINGS_BEFORE_AUTOMATICALY_ANSWERING_THE_CALL =
		{ "ATA", 0, basic };
const CommandType_TypeDef SET_COMMAND_LINE_TERMINATION_CHARACTER = { "ATA", 0,
		basic };
const CommandType_TypeDef SET_RESPONSE_FORMATTINF_CHARACTER =
		{ "ATA", 0, basic };
const CommandType_TypeDef SET_COMMAND_LINE_EDITING_CHARACTER =
		{ "ATA", 0, basic };
const CommandType_TypeDef PAUSE_BEFORE_BLIND_DIALING = { "ATA", 0, basic };
const CommandType_TypeDef SET_NUMBER_OF_SECONDS_TO_WAIT_FOR_CONNECTION_COMPETION =
		{ "ATA", 0, basic };
const CommandType_TypeDef SET_NUMBER_OF_SECONDS_TO_WAIT_FOR_COMMA_DIAL_MODIFIER_ENCOUNTERED =
		{ "ATA", 0, basic };
const CommandType_TypeDef SET_DISCONNECT_DELAY_AFTER_INDICATING_THE_ABSENCE_OF_DATA_CARRIER =
		{ "ATA", 0, basic };
const CommandType_TypeDef SELECT_TONE_DIALING = { "ATA", 0, basic };
const CommandType_TypeDef TA_RESPONSE_FORMAT = { "ATA", 0, basic };
const CommandType_TypeDef SET_CONNECT_RESULT_CODE_FORMAT_AND_MONITOR_CALL_PROGRESS =
		{ "ATA", 0, basic };
const CommandType_TypeDef RESET_DEFAULT_CONFIGURATION = { "ATA", 0, basic };
const CommandType_TypeDef SET_DCD_FUNCTION_MODE = { "ATA", 0, basic };
const CommandType_TypeDef SET_DTR_FUNCTION_MODE = { "ATA", 0, basic };
const CommandType_TypeDef FACTORY_DEFIEND_CONFIGURATION = { "ATA", 0, basic };
const CommandType_TypeDef DISPLAY_CURRENT_CONFIGURATION = { "ATA", 0, basic };
const CommandType_TypeDef STORE_ACTIVE_PROFILE = { "ATA", 0, basic };
const CommandType_TypeDef REQUEST_COMPLETE_TA_CAPABILITIES_LIST = { "ATA", 0,
		basic };
const CommandType_TypeDef REQUEST_MANUFACTURER_IDENTIFICATION = { "ATA", 0,
		basic };
const CommandType_TypeDef REQUEST_TA_MODEL_IDENTIFICATION = { "ATA", 0, basic };
const CommandType_TypeDef REQUEST_TA_REVISION_IDENTIFICATION_OF_SOFTWARE_RELEASE =
		{ "ATA", 0, basic };
const CommandType_TypeDef REQUEST_GLOBAL_OBJECT_IDENTIFICATION = { "ATA", 0,
		basic };
const CommandType_TypeDef REQUEST_TA_SERIAL_NUMBER_IDENTIFICATION = { "ATA", 0,
		basic };
const CommandType_TypeDef SET_TE_TA_CONTROL_CHARACTER_FRAMING = { "ATA", 0,
		basic };
const CommandType_TypeDef SET_TE_TA_LOCAL_DATA_FLOW_CONTROL =
		{ "ATA", 0, basic };
const CommandType_TypeDef SET_TE_TA_FIXED_LOCAL_RATE = { "ATA", 0, basic };
const CommandType_TypeDef DISCONNECT_VOICE_CALL_ONLY = { "ATA", 0, basic };

const CommandType_TypeDef NetworkServiceCommand[9] = { { (const char*) "COPS",
		75000, extended }, { (const char*) "CREG", 300, extended }, {
		(const char*) "CSQ", 300, extended }, { (const char*) "CPOL", 300,
		extended }, { (const char*) "COPN", 300, extended }, {
		(const char*) "QBAND", 30000, extended }, { (const char*) "QENG", 300,
		extended }, { (const char*) "QSCANF", 300, extended }, {
		(const char*) "QLOCKF", 300, extended }, };
