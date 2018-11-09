/*
 * bufferStream.c
 *
 *  Created on: Apr 27, 2018
 *      Author: home
 */

#include "bufferStream.h"

BufferStream_TypeDef * BufferStreamInit(uint32_t bufferLenght)
{
	BufferStream_TypeDef * bufferStream = pvPortMalloc(
			sizeof(BufferStream_TypeDef));
	bufferStream->buffer = pvPortMalloc(bufferLenght);
	bufferStream->length = bufferLenght;
	bufferStream->tail = 0;
	bufferStream->head = 0;
	return bufferStream;
}


void BufferStreamDeinit(BufferStream_TypeDef * bufferStream)
{
	vPortFree(bufferStream->buffer);
	vPortFree(bufferStream);
}

void BufferStreamRead(BufferStream_TypeDef * bufferStream, char *data,
		unsigned int count) {
	int32_t c1 = bufferStream->length - bufferStream->tail;
	if (c1 < count) {
		memcpy(data, bufferStream->buffer + bufferStream->tail, c1);
		count -= c1;
		bufferStream->tail = 0;
	}
	memcpy(data, bufferStream->buffer + bufferStream->tail, count);
	bufferStream->tail += count;
}

inline char * BufferStreamReadChar(BufferStream_TypeDef * bufferStream)
 {
	char * result;
	if (bufferStream->tail == bufferStream->head)
		result = 0;
	else
		result = bufferStream->buffer + bufferStream->tail++;
	if (bufferStream->tail >= bufferStream->length)
		bufferStream->tail = 0;

	return result;
}
inline int BufferStreamCheckEquality(BufferStream_TypeDef * bufferStream, char * data)
{
	int i = 0;
	while(*data != 0)
	{
		if (BufferStreamCheckEndOfFile(bufferStream))
			return 0;
		if (*(bufferStream->buffer + bufferStream->tail + i++) != *data++)
			return 0;
		if (bufferStream->tail + i >= bufferStream->length)
			i = i - bufferStream->length;
	}
	return 1;
}
inline int BufferStreamCheckEndOfFile(BufferStream_TypeDef * bufferStream){
	return bufferStream->head == bufferStream->tail;
}

inline void BufferStreamReadBefore(BufferStream_TypeDef * bufferStream,
		char *data, unsigned int count) {
	int32_t c1 = count - bufferStream->tail;
	if (c1 > 0) {
		memcpy(data, bufferStream->buffer + bufferStream->length - c1, c1);
		count -= c1;
	}
	memcpy(data, bufferStream->buffer + bufferStream->tail - count, count);
}





