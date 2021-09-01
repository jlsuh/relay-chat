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

typedef struct {
    uint32_t userID;
    char* userName;
    uint32_t socket;
    t_chat_room* currentChatRoom;
} t_user;

typedef enum {
    STRING, ROOMINFO,
} op_code;

void send_package(int, void*, t_buffer*);
void send_str(int, char*);
void* serialize_package(uint8_t, t_buffer*);
void* deserialize_package(int, bool);

#endif
