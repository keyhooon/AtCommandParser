/*
 * Tokenizer.c
 *
 *  Created on: Jul 5, 2018
 *      Author: HP
 */

#include <BufferTokenizer.h>
#include "string.h"
#include "FleetManagementBoard.h"

const char *ATCOMMAND_SEPERATOR = "\r\n";
const char *ATCOMMAND_FOOTER = "\r";
Tokenizer_TypeDef * BufferTokenizerInit(BufferStream_TypeDef *bufferStream, char * separator, char * footer) {
	Tokenizer_TypeDef * tokenizer = pvPortMalloc(
			sizeof(Tokenizer_TypeDef));
	tokenizer->bufferStream = bufferStream;
	tokenizer->Footer = footer;
	tokenizer->Separator = separator;
	return tokenizer;
}

void BufferTokenizerDeInit(Tokenizer_TypeDef * tokenizer) {
	vPortFree(tokenizer);
}

bufToken_TypeDef BufTok(BufferStream_TypeDef * bufferStream, char * delimiter, int bufferStreamMaxLength){
	bufToken_TypeDef tok = { 0, 0, 0 };
	int currentIndex = bufferStream->tail;

	int delimiterFoundCount = 0;
	int delimiterLength = strlen(delimiter);
	for (int i = 0; i < bufferStreamMaxLength; i++) {
		if (*(bufferStream->buffer + currentIndex) == *(delimiter + delimiterFoundCount))
			delimiterFoundCount++;
		if (delimiterLength == delimiterFoundCount) {
			*(bufferStream->buffer + currentIndex - delimiterLength + 1) = 0;
			int length = currentIndex - bufferStream->tail - delimiterLength + 1;
			if (length < 0) {
				length += bufferStream->length;
				tok.needTobeRealesed =  1;
				tok.item = pvPortMalloc(length);
				int e = bufferStream->length - bufferStream->tail;
				memcpy(tok.item, bufferStream->buffer + bufferStream->tail, e);
				memcpy(tok.item + e, bufferStream->buffer, length - e);
			} else
				tok.item = bufferStream->buffer + bufferStream->tail;
			bufferStream->tail = currentIndex + 1;
			tok.length = length;
			break;
		}
		if (currentIndex ++==bufferStream->length)
			currentIndex = 0;
	}
	return tok;
}
TokensList_TypeDef BufferTokenizerTokenizeResponse(
		Tokenizer_TypeDef * Tokenizer, unsigned int responseLength) {
	TokensList_TypeDef result;
	char * TokenizedItems[10];
	bufToken_TypeDef currentBuffToken;
	int tokenCount = 0;

	currentBuffToken = BufTok(Tokenizer->bufferStream,Tokenizer->Separator, responseLength);
	while (currentBuffToken.item != 0){
		if (currentBuffToken.needTobeRealesed)
			result.IndexNeedToBeReleased = tokenCount;
		TokenizedItems[tokenCount++] = currentBuffToken.item;
		responseLength -= currentBuffToken.length + strlen(Tokenizer->Separator);
		currentBuffToken = BufTok(Tokenizer->bufferStream,Tokenizer->Separator, responseLength);
	}

	currentBuffToken = BufTok(Tokenizer->bufferStream,Tokenizer->Footer, responseLength);
	if (currentBuffToken.item != 0)
	{
		if (currentBuffToken.needTobeRealesed)
			result.IndexNeedToBeReleased = tokenCount;
		TokenizedItems[tokenCount++] = currentBuffToken.item;
		responseLength -= currentBuffToken.length + strlen(Tokenizer->Separator);
	}
	result.Items = pvPortMalloc(sizeof(char *) * tokenCount);
	memcpy(result.Items, TokenizedItems, sizeof(char *) * tokenCount);
	return result;
}
