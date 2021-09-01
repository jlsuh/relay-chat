#ifndef SHARED_CHATROOM_H_
#define SHARED_CHATROOM_H_

#include <commons/collections/list.h>
#include <inttypes.h>
#include <stdlib.h>

typedef struct {
    uint32_t roomID;
    char* roomName;
    t_list* users;
} t_chat_room;

#endif
