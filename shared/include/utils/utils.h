#ifndef SHARED_UTILS_H_
#define SHARED_UTILS_H_

#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <netdb.h>

typedef struct {
    uint32_t size;
    void* stream;
} t_buffer;

typedef struct {
    uint8_t opCode;
    t_buffer* buffer;
} t_package;

typedef struct {
    uint32_t roomID;
    char* roomName;
    t_list* users;
} t_chat_room;

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
t_buffer* buffer_create();
void buffer_pack(t_buffer*, void*, int);
void send_str(int, char*);
void* buffer_pack_string(t_buffer*, char*);
void* buffer_pack_chat_room(t_buffer*, char*, uint32_t);
void* serialize_package(uint8_t, t_buffer*);
void* deserialize_package(int, bool);
void buffer_unpack(t_buffer*, void*, int);
t_chat_room* buffer_unpack_chat_room(t_buffer*);
char* buffer_unpack_string(t_buffer*);

#endif
