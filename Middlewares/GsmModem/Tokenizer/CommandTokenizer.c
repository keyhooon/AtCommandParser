/*
 * CommandTokenizer.c
 *
 *  Created on: Nov 10, 2018
 *      Author: HP
 */


CommandTokenizer_TypeDef * CommandTokenizer_Init(BufferStream_TypeDef *bufferStream,
		char * separator, char * footer) {
	CommandTokenizer_TypeDef * tokenizer = pvPortMalloc(sizeof(CommandTokenizer_TypeDef));
	tokenizer->bufferStream = bufferStream;
	tokenizer->Footer = footer;
	tokenizer->Separator = separator;
	return tokenizer;
}

void CommandTokenizer_DeInit(CommandTokenizer_TypeDef * tokenizer) {
	vPortFree(tokenizer);
}

CommandTokensList_TypeDef CommandTokenizer_tokenize(
		CommandTokenizer_TypeDef * tokenizer, unsigned int commandLength) {
	CommandTokensList_TypeDef result = { 0, -1, -1 };
	char * TokenizedItems[10];
	bufToken_TypeDef currentBuffToken;
	int tokenCount = 0;

	currentBuffToken = BufTok(tokenizer->bufferStream, tokenizer->Separator,
			commandLength);
	while (currentBuffToken.item != 0) {
		if (currentBuffToken.needTobeRealesed)
			result.IndexNeedToBeReleased = tokenCount;
		TokenizedItems[tokenCount++] = currentBuffToken.item;
		commandLength -= currentBuffToken.length
				+ strlen(tokenizer->Separator);
		currentBuffToken = BufTok(tokenizer->bufferStream, tokenizer->Separator,
				commandLength);
	}

	currentBuffToken = BufTok(tokenizer->bufferStream, tokenizer->Footer,
			commandLength);
	if (currentBuffToken.item != 0) {
		if (currentBuffToken.needTobeRealesed)
			result.IndexNeedToBeReleased = tokenCount;
		result.ResultIndex = tokenCount;
		TokenizedItems[tokenCount++] = currentBuffToken.item;
		commandLength -= currentBuffToken.length
				+ strlen(tokenizer->Separator);
	}
	TokenizedItems[tokenCount++] = 0;
	result.Items = pvPortMalloc(sizeof(char *) * tokenCount);
	memcpy(result.Items, TokenizedItems, sizeof(char *) * tokenCount);
	return result;
}
void CommandTokenizer_FreeTokenList(CommandTokensList_TypeDef commandTokenList) {
	if (commandTokenList.IndexNeedToBeReleased != -1)
		vPortFree(
				commandTokenList.Items[commandTokenList.IndexNeedToBeReleased]);
	vPortFree(commandTokenList.Items);
}
