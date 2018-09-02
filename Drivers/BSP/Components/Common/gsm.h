/*
 * gsm.h
 *
 *  Created on: Sep 18, 2017
 *      Author: home
 */

#ifndef BSP_COMPONENTS_COMMON_GSM_H_
#define BSP_COMPONENTS_COMMON_GSM_H_


typedef struct
{
	void      	(*Init)(uint16_t);
	void      	(*DeInit)(void);
	void		(*PowerOn)(void);
	void		(*PowerOff)(void);

	void		(*BtPowerOn)(void);
	void		(*BtPowerOff)(void);
	void		(*BtChangeName)(void);
	void		(*BtSetVisibility)(void);
	void		(*BtReadAddress)(void);
	void		(*BtScan)(void);
	void		(*BtCancelScan)(void);

	void		(*BtPair)(void);
	void		(*BtUnpair)(void);
	void 		(*BtPairingAccept)(void);
	void		(*BTGetPairingStatus)(void);
	void		(*BtConnect)(void);
	void		(*BtDisconnect)(void);
	void		(*BtConnectDirect)(void);
	void 		(*BtConnectionAccept)(void);
	void		(*BTGetConnectionStatus)(void);

	void		(*BtSendData)(void);
	void		(*BtReadData)(void);

}GSM_DrvTypeDef;
#endif /* BSP_COMPONENTS_COMMON_GSM_H_ */
