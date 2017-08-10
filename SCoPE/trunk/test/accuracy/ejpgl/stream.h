/**
 * \file stream.h
 */
#ifndef STREAM_H
#define STREAM_H

typedef unsigned int size_t;
typedef unsigned char byte_t;

typedef struct {
	byte_t *data;
	byte_t *pos;
	size_t size;
} stream_t;

#ifndef SEEK_SET
	#define SEEK_SET 0
	#define SEEK_CUR 1
	#define SEEK_END 2
#endif

size_t sread(void *ptr, size_t tam, size_t nmiemb, stream_t *flujo);
size_t swrite(void *ptr, size_t tam, size_t nmiemb, stream_t *flujo);
int sseek(stream_t *flujo, long desplto, int origen);

#endif /* STREAM_H */

