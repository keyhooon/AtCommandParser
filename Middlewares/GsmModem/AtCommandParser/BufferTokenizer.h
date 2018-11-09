/*
 * AtTokenizer.h
 *
 *  Created on: Jul 5, 2018
 *      Author: HP
 */

#ifndef TOKENIZER_ATTOKENIZER_H_
#define TOKENIZER_ATTOKENIZER_H_



typedef struct {
	BufferStream_TypeDef * bufferStream;
	char * Footer;
	unsigned int FooterCount;
	char * Separator;
	unsigned int SeparatorCount;
} Tokenizer_TypeDef;


typedef struct {
	char * item;
	int length;
	int needTobeRealesed;
} bufToken_TypeDef;

typedef struct {
	char ** Items;
	int IndexNeedToBeReleased;
} TokensList_TypeDef;

Tokenizer_TypeDef * AtTokenizerInit(char * seperator, char * footer);
void AtTokenizerDeInit(Tokenizer_TypeDef * tokenizer);
TokensList_TypeDef BufferTokenizerTokenizeResponse(
		Tokenizer_TypeDef * Tokenizer, unsigned int length);


#endif /* TOKENIZER_ATTOKENIZER_H_ */
