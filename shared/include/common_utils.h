#ifndef SHARED_UTILS_H_
#define SHARED_UTILS_H_

#include "buffer.h"
#include <commons/config.h>
#include <commons/string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

typedef enum {
    STRING, ROOMINFO,
} op_code;

void string_send(int, char*);
void stream_send(int, void*, uint32_t);
void* stream_serialize(uint8_t, t_buffer*);
void* stream_deserialize(int, bool);

#endif
