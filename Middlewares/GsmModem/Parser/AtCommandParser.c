/*
 * command.c
 *
 *  Created on: Apr 26, 2018
 *      Author: home
 */
#include <Parser/AtCommandParser.h>
#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "FleetManagementBoard.h"







Response_TypeDef ResponseParse(CommandTokenizer_TypeDef tokenizer,
		unsigned int length) {
	CommandTokensList_TypeDef tokensList = CommandTokenizer_tokenize(tokenizer,
			length);
	Response_TypeDef result;
	result.Tokens = tokensList;
	result.status = ResponseStatusOk;
	if (tokensList.ResultIndex != -1) {
		register char* string = *(tokensList.Items + tokensList.ResultIndex);
		register unsigned int digit;
		while (*string != NULL) {
			digit = *string++ - '0';
			if (digit < 0 || digit > 9) {
				result.resultNumber = -1;
				result.status = ResponseStatusError_BadFormat;
				break;
			} else
				result.resultNumber = result.resultNumber * 10 + digit;
		}
	} else {
		result.resultNumber = -1;
		result.status = ResponseStatusError_ResultIsntThere;
	}
	return result;
}

void CommandExecuter_ResponseRelease(Response_TypeDef response) {
	if (response.Tokens.IndexNeedToBeReleased != -1)
		vPortFree(
				response->Tokens.Items[response->Tokens.IndexNeedToBeReleased]);
}
