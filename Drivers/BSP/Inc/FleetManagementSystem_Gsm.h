/*
 * FleetManagementSystem_Gsm.h
 *
 *  Created on: Sep 23, 2017
 *      Author: home
 */

#ifndef BSP_INC_FLEETMANAGEMENTSYSTEM_GSM_H_
#define BSP_INC_FLEETMANAGEMENTSYSTEM_GSM_H_

#include "FleetManagementBoard.h"
#include "QuecTelM66.h"

#define HTTP_RESPONSE_MAXIMUM_LENGTH		64

void BSP_GSM_Init(void);
uint32_t BSP_GSM_GPRS_Init(void);
uint32_t BSP_GSM_GPRS_Open(void);
uint32_t BSP_GSM_GPRS_Close(void);
uint32_t BSP_GSM_HTTP_Get(char *Address, uint8_t * Data, uint32_t *DataLength);
uint32_t BSP_GSM_HTTP_Post(char *Address, uint8_t * RequestData, uint32_t RequestDataLength, uint8_t * ResponseData, uint8_t * ResponseDataLength, uint32_t * StatusCode);

typedef enum {
	Unlock, Lock,
} LockMode_TypeDef;

typedef enum {
	Disable, Enable, EnableWithLocation
} NetworkRegistration_TypeDef;

typedef enum {
	NotRegisteredNotSearching,
	RegisterHome,
	NotRegisterButSearching,
	Denied,
	Unknown,
	RegisterRoaming
} NetworkRegistrationState_TypeDef;

typedef struct {
	char Strength;
	char BitErrorRate;
} SignalQualityReport;

typedef struct {
	NetworkRegistration_TypeDef Registration;
	NetworkRegistrationState_TypeDef State;
} NetworkRegistrationReport;

#endif /* BSP_INC_FLEETMANAGEMENTSYSTEM_GSM_H_ */
