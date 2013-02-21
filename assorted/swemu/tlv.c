#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <strarg.h>

#include <arpa/inet.h>

#include "tlv.h"

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

struct tlv_object *make_tlv(int length)
{
	struct tlv_object *obj;

	obj = calloc(sizeof(struct tlv_object), 1);
	if (!tlv) return NULL;

	obj->buffer = calloc(sizeof(char), 1);
	if (!obj->buffer) {
		free(tlv);
		return NULL;
	}

	obj->length = length;

	return tlv;
}

static inline int encode_u32(u8 *buf, u32 type)
{
	u8 *p = (u8 *)&type;

	type = htonl(type);
	*buf++ = *p++;
	*buf++ = *p++;
	*buf++ = *p++;
	*buf = *p;

	return 4;
}

static inline int encode_u8(u8 *buf, u8 type)
{
	*buf = type;

	return 1;
}

static inline int encode_u16(u8 *buf, u16 type)
{
	u16 *p = (u16 *)&type;

	type = htons(type);
	*buf++ = *p++;
	*buf = *p;

	return 2;
}

static inline int encode_u64(u8 *buf, u64 type)
{
	u32 t1 = type & 0xffffffff, t2 = (type >> 32) & 0xffffffff;

	encode_u32(buf, t1);
	encode_u32(buf+4, t2);

	return 8;
}

static int resize_tlv(struct tlv_object *obj, int len)
{
	if (obj->len <= obj->index+len+1 &&
	    (!realloc(obj->buffer, obj->index+len+1-obj->len+len)))
			return -1;
	return 0;
}

int push_tlv_object(struct tlv_object *obj, ...)
{
	va_list arg;
	char *buf;
	int index, i;
	int tlv_type, tlv_dyn_size;
	u8 tlv_u8, *tlv_dyn;
	u16 tlv_u16;
	u32 tlv_u32;
	u64 tlv_u64;

	if (!obj) return -1;

	buf = obj->buffer;
	index = obj->index;

	va_start(arg, obj);
	
	do {
		type = va_arg(arg, int);
		if (type == TLV_TTYPE_NONE)
			break;

		tlv_type = va_arg(arg, int);
		if (!resize_tlv(obj, sizeof(tlv_type)))
			return -1;
		index += encode_u32(&buf[index], tlv_type);

		if (type == TLV_TTYPE_VALUE) {
			if (!resize_tlv(obj, sizeof(u64)))
				return -1;

			switch (tlv_type) {
			case TLV_TVALUE_U8:
				tlv_u8 = va_arg(arg, char);
				index += encode_u8(&buf[index], tlv_u8);
				break;
			case TLV_TVALUE_U16:
				tlv_u16 = va_arg(arg, short int);
				index += encode_u16(&buf[index], tlv_u16);
				break;
			case TLV_TVALUE_U32:
				tlv_u32 = va_arg(arg, int);
				index += encode_u32(&buf[index], tlv_u32);
				break;
			case TLV_TVALUE_U64:
				tlv_u64 = va_arg(arg, long long int);
				index += encode_u64(&buf[index], tlv_u64);
				break;
			}
		} else if (type == TLV_TTYPE_DYNAMIC) {
			tlv_dyn_size = va_arg(arg, int);
			if (!resize_tlv(obj, tlv_dyn_size+4))
				return -1;

			index += encode_u32(&buf[index], tlv_dyn_size);
			tlv_dyn = va_arg(arg, u8 *);
			for (i = 0; i < tlv_dyn_size; i++, index++)
				buf[index] = tlv_dyn[i];
		}
		obj->index = index;
	} while (1);

	va_end(arg);

	return 0;
}

int trim_tlv_object(struct tlv_object *obj)
{
	if (!obj) return -1;

	if (obj->index < obj->len-1) {
		realloc(obj->buffer, obj->index+1);
		obj->len = obj->index+1;
	}
	return 0;
}

char *get_tlv_object(struct tlv_object *obj, int *len)
{
	if (!obj) return NULL;

	trim_tlv_object(obj);
	*len = obj->len;

	return obj->buffer;
}

void delete_tlv_object(struct tlv_object *obj)
{
	if (!obj) return;

	free(obj->buffer);
	free(obj);
}

struct tlv_object *make_wrapper_tlv(char *tlv, int len)
{
	struct tlv_object *obj;

	obj = calloc(1, sizeof(struct tlv_object));
	if (!obj) return NULL;

	obj->len = len;
	obj->index = 0;
	obj->buffer = tlv;

	return obj;
}

int pop_tlv_object(struct tlv_object *obj, int *type, int *len, void *data)
{
	char *buf;
	int index, tlv_type, tlv_dyn_size;
	u8 tlv_u8, *tlv_dyn;
	u16 tlv_u16;
	u32 tlv_u32;
	u64 tlv_u64;

	buf = obj->buffer;
	index = obj->index;

	if (!type) {
		tlv_type = ntohl(*(u32 *)buf[index]);
		obj->index = index + 4;
		return tlv_type;
	}
	switch (type) {
	case TLV_TVALUE_U8:
		tlv_u8 = buf[index];
		index++;
		break;
	case TLV_TVALUE_U16:
		tlv_u16 = htons(*(u16 *)&buf[index]);
	}
}

