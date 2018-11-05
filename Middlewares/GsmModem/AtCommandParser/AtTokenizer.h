/*
 * AtTokenizer.h
 *
 *  Created on: Jul 5, 2018
 *      Author: HP
 */

#ifndef TOKENIZER_ATTOKENIZER_H_
#define TOKENIZER_ATTOKENIZER_H_



typedef struct {
	char * Footer;
	unsigned int FooterCount;
	char * Seperator;
	unsigned int SeperatorCount;
} Tokenizer_TypeDef;

typedef struct {
	char * String;
	unsigned int Length;
} FixedLengthString_TypeDef;

typedef struct {
	FixedLengthString_TypeDef * Items;
	unsigned int Count;
	int IndexNeedToBeReleased;
} TokensList_TypeDef;

Tokenizer_TypeDef * AtTokenizerInit();
void AtTokenizerDeInit(Tokenizer_TypeDef * tokenizer);
TokensList_TypeDef AtTokenizedResponse(
		Tokenizer_TypeDef * Tokenizer, unsigned int length);


#endif /* TOKENIZER_ATTOKENIZER_H_ */
