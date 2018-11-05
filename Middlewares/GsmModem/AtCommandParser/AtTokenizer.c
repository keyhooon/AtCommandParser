/*
 * Tokenizer.c
 *
 *  Created on: Jul 5, 2018
 *      Author: HP
 */

#include "AtTokenizer.h"
#include "string.h"
#include "FleetManagementBoard.h"

const char *SEPERATOR = "\r\n";
const char *FOOTER = "\r";
Tokenizer_TypeDef * AtTokenizerInit() {
	Tokenizer_TypeDef * tokenizer = pvPortMalloc(
			sizeof(Tokenizer_TypeDef));
	tokenizer->Footer = FOOTER;
	tokenizer->FooterCount = strlen(FOOTER);
	tokenizer->Seperator = SEPERATOR;
	tokenizer->SeperatorCount = strlen(SEPERATOR);
	return tokenizer;
}

void AtTokenizerDeInit(Tokenizer_TypeDef * tokenizer) {
	vPortFree(tokenizer);
}

TokensList_TypeDef AtTokenizedResponse(
		Tokenizer_TypeDef * Tokenizer, unsigned int length) {
	TokensList_TypeDef result;
	FixedLengthString_TypeDef TokenizedItems[10];
	char * tokenStart;
	char * current;
	int seperatorFind = 0;
	int footerFind = 0;
	current = GSM_IO_Read_Char();
	length--;
	tokenStart = current;
	result.IndexNeedToBeReleased = -1;
	for (; length > 0; length--) {


		// search Seperator
		if (*current == *(Tokenizer->Seperator + seperatorFind)) {
			seperatorFind++;
		} else
			seperatorFind = 0;

		// search Footer
		if (*current == *(Tokenizer->Footer + footerFind)) {
			footerFind++;
		} else
			footerFind = 0;

		// found seperator or footer
		if (footerFind == Tokenizer->FooterCount
				|| seperatorFind == Tokenizer->SeperatorCount) {


			*current = 0;
			int length = current - tokenStart;
			if (length < 0) {
				length += DEFAULT_BUFFER_LENGTH;
				TokenizedItems[result.Count].String = pvPortMalloc(length);
				GSM_IO_ReadBefore(TokenizedItems[result.Count].String, length);
			} else
				TokenizedItems[result.Count].String = tokenStart;
			current = GSM_IO_Read_Char();
			tokenStart = current;
			if (footerFind == Tokenizer->FooterCount)
			{
				TokenizedItems[result.Count].Length = length - footerFind;

				footerFind = 0;
				break;
			} else if (seperatorFind == Tokenizer->SeperatorCount) {
				TokenizedItems[result.Count].Length = length - seperatorFind;
				seperatorFind = 0;
			}

		} else
			current = GSM_IO_Read_Char();
	}

	return result;
}
