/*
 * CommandTokenizer.h
 *
 *  Created on: Nov 10, 2018
 *      Author: HP
 */

#ifndef GSMMODEM_ATCOMMANDPARSER_COMMANDTOKENIZER_H_
#define GSMMODEM_ATCOMMANDPARSER_COMMANDTOKENIZER_H_



typedef struct {
	char ** Items;
	int ResultIndex;
	int IndexNeedToBeReleased;
} CommandTokensList_TypeDef;

typedef struct {
	BufferStream_TypeDef * bufferStream;
	char * Footer;
	unsigned int FooterCount;
	char * Separator;
	unsigned int SeparatorCount;
} CommandTokenizer_TypeDef;

const char *ATCOMMAND_SEPERATOR = "\r\n";
const char *ATCOMMAND_FOOTER = "\r";

CommandTokenizer_TypeDef * CommandTokenizer_Init(BufferStream_TypeDef *bufferStream,
		char * separator, char * footer);

void CommandTokenizer_DeInit(CommandTokenizer_TypeDef * tokenizer);
CommandTokensList_TypeDef CommandTokenizer_tokenize(
		CommandTokenizer_TypeDef * Tokenizer, unsigned int length);

#endif /* GSMMODEM_ATCOMMANDPARSER_COMMANDTOKENIZER_H_ */
