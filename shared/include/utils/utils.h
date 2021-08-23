#ifndef SHARED_UTILS_H_
#define SHARED_UTILS_H_

#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>

typedef struct {
    char* content;
    uint32_t length;
} t_string;

typedef struct {
    uint32_t size;
    void* stream;
} t_buffer;

typedef struct {
    uint8_t op_code;
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

void send_package(int userSocket, void* toSend, t_buffer* buffer);
void send_str_msg(int socket, char* str);
t_buffer* serialize_string(t_string msg);
t_buffer* serialize_chat_room(t_string roomName, uint32_t roomID);
void* serialize_package(uint8_t op_code, t_buffer* buffer);
void* deserialize_package(int serverSocket, bool deserializeNext);
t_chat_room* deserialize_chat_room(t_buffer* buffer);
t_string* deserialize_string(t_buffer* buffer);
t_config* get_cwd_config(char* configFileName);

#endif
