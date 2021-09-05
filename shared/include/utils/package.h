#ifndef SHARED_PACKAGE_H_
#define SHARED_PACKAGE_H_

#include "utils/buffer.h"
#include <netdb.h>

typedef struct {
    uint8_t opCode;
    t_buffer* buffer;
} t_package;

typedef enum {
    STRING, ROOMINFO,
} op_code;

void package_send(int userSocket, void* toSend, uint32_t bufferSize);
t_package* package_create(uint8_t opCode, t_buffer* buffer);
void package_destroy(t_package* package);
void* package_serialize(uint8_t opCode, t_buffer* buffer);
void* package_deserialize(int serverSocket, bool deserializeNext);

#endif
