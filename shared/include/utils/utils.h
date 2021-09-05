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

typedef struct {
    uint8_t opCode;
    t_buffer* buffer;
} t_package;

typedef enum {
    STRING, ROOMINFO,
} op_code;

void package_send(int userSocket, void* toSend, uint32_t bufferSize);
void string_send(int socket, char* str);
t_package* package_create(uint8_t opCode, t_buffer* buffer);
void package_destroy(t_package* package);
void* package_serialize(uint8_t opCode, t_buffer* buffer);
void* package_deserialize(int serverSocket, bool deserializeNext);

#endif
