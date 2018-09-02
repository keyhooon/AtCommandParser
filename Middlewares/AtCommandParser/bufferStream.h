/*
 * bufferStream.h
 *
 *  Created on: Apr 27, 2018
 *      Author: home
 */

#ifndef DATASTRUCTURE_BUFFERSTREAM_H_
#define DATASTRUCTURE_BUFFERSTREAM_H_

#include "cmsis_os.h"

typedef struct
{
	char * buffer;
	unsigned int length;
	unsigned int tail;
	unsigned int head;
} BufferStream_TypeDef;

typedef enum {
	Progress, EndOfFile, JumpToFirst
} ReadStatus_TypeDef;

typedef struct
{
	char *data;
	ReadStatus_TypeDef status;
} ReadCharResult_TypeDef;

BufferStream_TypeDef * BufferStreamInit(uint32_t bufferLenght);


void BufferStreamDeinit(BufferStream_TypeDef * bufferStream);

void BufferStreamRead(BufferStream_TypeDef * bufferStream, char *data,
		unsigned int count);

inline char* BufferStreamReadChar(
		BufferStream_TypeDef * bufferStream);

#endif /* DATASTRUCTURE_BUFFERSTREAM_H_ */
