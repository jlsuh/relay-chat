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
void buffer_destroy(t_buffer* buffer);
void buffer_pack(t_buffer* buffer, void* streamToAdd, int size);
void buffer_unpack(t_buffer* buffer, void* dest, int size);
void buffer_pack_string(t_buffer* buffer, char* stringToAdd);
char* buffer_unpack_string(t_buffer* buffer);
void buffer_pack_chat_room(t_buffer* buffer, uint32_t roomID, char* roomName);
t_chat_room* buffer_unpack_chat_room(t_buffer* buffer);

#endif
