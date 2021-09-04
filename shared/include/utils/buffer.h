#ifndef SHARED_BUFFER_H_
#define SHARED_BUFFER_H_

#include <stdio.h>
#include <string.h>
#include "chat_room.h"

typedef struct {
    uint32_t size;
    void* stream;
} t_buffer;

t_buffer* buffer_create();
void buffer_pack(t_buffer*, void*, int);
void buffer_pack_string(t_buffer*, char*);
void buffer_pack_chat_room(t_buffer*, uint32_t, char*);
void buffer_unpack(t_buffer*, void*, int);
t_chat_room* buffer_unpack_chat_room(t_buffer*);
char* buffer_unpack_string(t_buffer*);
void buffer_destroy(t_buffer*);

#endif
