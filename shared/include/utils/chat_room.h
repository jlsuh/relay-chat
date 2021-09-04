#ifndef SHARED_CHATROOM_H_
#define SHARED_CHATROOM_H_

#include <commons/collections/list.h>
#include <stdint.h>
#include <stdlib.h>

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

void free_chatroom(void*);
void free_user(void*);

#endif
