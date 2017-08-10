/**
 * \file stream.c
 */

#include "stream.h"

size_t sread(void *ptr, size_t tam, size_t nmiemb, stream_t *flujo) {
	int i, j;
	byte_t *dest;
	dest = (byte_t *)ptr;
	for (i=0 ; i<nmiemb ; i++) {
		for (j=0 ; j<tam ; j++) {
			*dest = *(flujo->pos);
			dest++;
			flujo->pos++;
		}
	}
	return nmiemb;
}


size_t swrite(void *ptr, size_t tam, size_t nmiemb, stream_t *flujo) {
	int i, j;
	byte_t *src;
	src = (byte_t *)ptr;
	for (i=0 ; i<nmiemb ; i++) {
		for (j=0 ; j<tam ; j++) {
			*(flujo->pos) = *src;
			src++;
			flujo->pos++;
			flujo->size++;
		}
	}
	return nmiemb;
}


int sseek(stream_t *flujo, long desplto, int origen) {
	byte_t *base;
	switch (origen) {
		case SEEK_SET:
			base = flujo->data;
			break;
		case SEEK_CUR:
			base = flujo->pos;
			break;
		case SEEK_END:
			base = flujo->data + flujo->size;
			break;
		default:
			return -1;
	}
	base += desplto;
	flujo->pos = base;
	return 0;
}

