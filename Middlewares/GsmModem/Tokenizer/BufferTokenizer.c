/*
 * Tokenizer.c
 *
 *  Created on: Jul 5, 2018
 *      Author: HP
 */

#include <BufferTokenizer.h>
#include "string.h"
#include "FleetManagementBoard.h"


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

