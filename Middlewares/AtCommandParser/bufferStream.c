/*
 * bufferStream.c
 *
 *  Created on: Apr 27, 2018
 *      Author: home
 */
#include "bufferStream.h"
#include "string.h"
#define DEFAULT_BUFFER_LENGTH		1024
#define DEFAULT_QUEUE_LENGTH		5


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
	uint32_t c1 = bufferStream->length - bufferStream->tail;
	if (c1 < count) {
		memcpy(data, bufferStream->buffer + bufferStream->tail, c1);
		count -= c1;
	}
	memcpy(data, bufferStream->buffer + bufferStream->tail, count);
}

inline char * BufferStreamReadChar(
		BufferStream_TypeDef * bufferStream)
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




