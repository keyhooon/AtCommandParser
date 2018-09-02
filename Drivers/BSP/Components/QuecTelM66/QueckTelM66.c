/*
 * QueckTelM66.c
 *
 *  Created on: Apr 17, 2018
 *      Author: HP
 */

#include "../../../../Middlewares/AtCommandParser/command.h"

#define NETWORK_SERVICE_COMMANDS_SUPPORT
#define GPRS_COMMANDS_SUPPORT


const CommandType_TypeDef NetworkServiceCommand[9] = { { (const char*) "COPS",
		75000, extended }, { (const char*) "CREG", 300, extended }, {
		(const char*) "CSQ", 300, extended }, { (const char*) "CPOL", 300,
		extended }, { (const char*) "COPN", 300, extended }, {
		(const char*) "QBAND", 30000, extended }, { (const char*) "QENG", 300,
		extended }, { (const char*) "QSCANF", 300, extended }, {
		(const char*) "QLOCKF", 300, extended },
		};

const CommandType_TypeDef GprsCommand[12] = { { (const char*) "CGATT", 75000,
		extended }, { (const char*) "CGDCONT", 300, extended }, {
		(const char*) "CGQREQ", 300, extended }, { (const char*) "CGQMIN", 300,
		extended }, { (const char*) "CGACT", 150000, extended }, {
		(const char*) "CGDATA", 300, extended }, { (const char*) "CGPADDR", 300,
		extended }, { (const char*) "CGCLASS", 30000, extended }, {
		(const char*) "CGEREP", 300, extended }, { (const char*) "CGREG", 300,
		extended }, { (const char*) "CGSMS", 300, extended }, {
		(const char*) "QGPCLASS", 300, extended },
};
const CommandType_TypeDef GeneralCommand[23] = {
		{ (const char*) "I", 300, basic },
		{ (const char*) "GMI", 300, extended }, { (const char*) "GMM", 300,
				extended }, { (const char*) "GMR", 300, extended }, {
				(const char*) "GOI", 300, basic }, { (const char*) "CGMI", 300,
				extended }, { (const char*) "CGMM", 300, extended }, {
				(const char*) "CGMR", 300, extended }, { (const char*) "GSN",
				300, basic }, { (const char*) "CGSN", 300, extended }, {
				(const char*) "QGSN", 300, extended }, { (const char*) "&F",
				300, basic }, { (const char*) "&V", 300, basic }, {
				(const char*) "&W", 300, basic }, { (const char*) "Q", 300,
				basic }, { (const char*) "V", 300, basic }, { (const char*) "X",
				300, basic }, { (const char*) "Z", 300, basic }, {
				(const char*) "CFUN", 15000, extended }, {
				(const char*) "QPOWD", 300, extended }, { (const char*) "CMEE",
				300, extended }, { (const char*) "CSCS", 300, extended }, {
				(const char*) "GCAP", 300, extended },
};

const CommandType_TypeDef SimRelatedCommands[14] = { { (const char*) "CIMI",
		300, extended }, { (const char*) "CLCK", 5000, extended }, {
		(const char*) "CPIN", 5000, extended }, { (const char*) "CPWD", 300,
		extended }, { (const char*) "CRSM", 300, extended }, {
		(const char*) "CSIM", 300, extended }, { (const char*) "QCSPWD", 300,
		extended }, { (const char*) "CCID", 300, extended }, {
		(const char*) "QCCID", 300, extended }, { (const char*) "QGID", 300,
		extended }, { (const char*) "QSIMVOL", 300, extended }, {
		(const char*) "QSPN", 300, extended }, { (const char*) "QTRPIN", 300,
		extended }, { (const char*) "QDSIM", 300, extended }, };

const CommandType_TypeDef HttpCommands[6] = { { (const char*) "QHTTPURL", 12000,
		extended }, { (const char*) "QHTTPGET", 12000, extended }, {
		(const char*) "QHTTPPOST", 12000, extended }, {
		(const char*) "QHTTPREAD", 12000, extended }, { (const char*) "QHTTPDL",
		12000, extended }, { (const char*) "QHTTPCFG", 12000, extended }, };
const CommandType_TypeDef TcpIpCommands[34] = { { (const char*) "QIOPEN", 75000,
		extended }, { (const char*) "QISEND", 300, extended }, {
		(const char*) "QICLOSE", 300, extended }, { (const char*) "QIDEACT",
		40000, extended }, { (const char*) "QILPORT", 300, extended }, {
		(const char*) "QIREGAPP", 300, extended }, { (const char*) "QIACT",
		150000, extended }, { (const char*) "QILOCIP", 300, extended }, {
		(const char*) "QISTAT", 300, extended }, { (const char*) "QISTATE", 300,
		extended }, { (const char*) "QISSTAT", 300, extended }, {
		(const char*) "QIDNSCFG", 300, extended }, { (const char*) "QIDNSGIP",
		14000, extended }, { (const char*) "QIDNSIP", 300, extended }, {
		(const char*) "QIHEAD", 300, extended }, { (const char*) "QIAUTOS", 300,
		extended }, { (const char*) "QIPROMPT", 300, extended }, {
		(const char*) "QISERVER", 150000, extended }, { (const char*) "QICSGP",
		300, extended }, { (const char*) "QISRVC", 300, extended }, {
		(const char*) "QISHOWRA", 300, extended }, { (const char*) "QISCON",
		300, extended }, { (const char*) "QIMODE", 300, extended }, {
		(const char*) "QITCFG", 300, extended }, { (const char*) "QISHOWPT",
		300, extended }, { (const char*) "QIMUX", 300, extended }, {
		(const char*) "QISHOWLA", 300, extended }, { (const char*) "QIFGCNT",
		300, extended }, { (const char*) "QISACK", 300, extended }, {
		(const char*) "QINDI", 300, extended }, { (const char*) "QIRD", 300,
		extended }, { (const char*) "QISDE", 300, extended }, {
		(const char*) "QPING", 300, extended }, { (const char*) "QNTP", 120000,
		extended }, };




