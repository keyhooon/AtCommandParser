/*
 * Tokenizer.c
 *
 *  Created on: Jul 5, 2018
 *      Author: HP
 */

#include "AtTokenizer.h"
#include "string.h"

const char *SEPERATOR = "\r\n";
const char *FOOTER = "\r";
Tokenizer_TypeDef * AtTokenizerInit(BufferStream_TypeDef * inputBuffer) {
	Tokenizer_TypeDef * tokenizer = pvPortMalloc(
			sizeof(Tokenizer_TypeDef));

	tokenizer->Buffer = inputBuffer;
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
	int find;
	int type;
	tokenStart = BufferStreamReadChar(Tokenizer->Buffer);
	char * current = tokenStart;
	result.IndexNeedToBeReleased = -1;
	for (; length > 0; length--) {
		type = 0;

		// search Seperator
		for (find = 0;
				find < Tokenizer->SeperatorCount
						&& *current == *(Tokenizer->Seperator + find); find++)
			;
		if (find == Tokenizer->SeperatorCount)
			type = 1;

		// search Footer
		for (find = 0;
				find < Tokenizer->FooterCount
						&& *current == *(Tokenizer->Footer + find); find++)
			;
		if (find == Tokenizer->FooterCount)
			type = 2;

		// found seperator or footer
		if (type != 0) {
			*current = 0;
			int length = current - tokenStart;
			if (length < 0) {
				length += Tokenizer->Buffer->length;
				TokenizedItems[result.Count].String =
						pvPortMalloc(
						length);
				int firstPartLength = length
						- (current - Tokenizer->Buffer->buffer);
				memcpy(
						TokenizedItems[result.Count].String,
						tokenStart,
						firstPartLength);
				memcpy(
						TokenizedItems[result.Count].String
								+ firstPartLength,
						Tokenizer->Buffer->buffer,
						length - firstPartLength);
			} else
				TokenizedItems[result.Count].String =
						tokenStart;
			TokenizedItems[result.Count].Length =
					length;

			tokenStart = BufferStreamReadChar(Tokenizer->Buffer);
			current = tokenStart;
			if (type == 1)
				continue;
			if (type == 2)
				break;
		}
		current = BufferStreamReadChar(Tokenizer->Buffer);
	}

	return result;
}
