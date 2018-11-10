/*
 * AtTokenizer.h
 *
 *  Created on: Jul 5, 2018
 *      Author: HP
 */

#ifndef TOKENIZER_ATTOKENIZER_H_
#define TOKENIZER_ATTOKENIZER_H_


typedef struct {
	char * item;
	int length;
	int needTobeRealesed;
} bufToken_TypeDef;


bufToken_TypeDef BufTok(BufferStream_TypeDef * bufferStream, char * delimiter,
		int bufferStreamMaxLength);



#endif /* TOKENIZER_ATTOKENIZER_H_ */
