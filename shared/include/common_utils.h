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

void string_send(int socket, char* str);
void stream_send(int userSocket, void* toSend, uint32_t bufferSize);
void* stream_serialize(uint8_t opCode, t_buffer* buffer);
void* stream_deserialize(int serverSocket, bool deserializeNext);

#endif
