#include "utils/buffer.h"

t_buffer* buffer_create() {
    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer->size = 0;
    buffer->stream = NULL;
    return buffer;
}

void buffer_pack(t_buffer* buffer, void* streamToAdd, int size) {
    buffer->stream = realloc(buffer->stream, buffer->size + size);  // Allocate more memory than original
    memcpy(buffer->stream + buffer->size, streamToAdd, size);       // Add new stream from last position
    buffer->size += size;                                           // Update buffer size
}

void* buffer_pack_string(t_buffer* buffer, char* stringToAdd) {
    uint32_t length = strlen(stringToAdd) + 1;
    buffer_pack(buffer, &length, sizeof(length));
    buffer->stream = realloc(buffer->stream, buffer->size + length);
    memcpy(buffer->stream + buffer->size, stringToAdd, length);
    buffer->size += length;
    return buffer;
}

void* buffer_pack_chat_room(t_buffer* buffer, uint32_t roomID, char* roomName) {
    buffer_pack(buffer, &roomID, sizeof(roomID));
    buffer_pack_string(buffer, roomName);
    return buffer;
}

void buffer_unpack(t_buffer* buffer, void* dest, int size) {
    memcpy(dest, buffer->stream, size);
    buffer->size -= size;
    memmove(buffer->stream, buffer->stream + size, buffer->size);
    buffer->stream = realloc(buffer->stream, buffer->size);
}

t_chat_room* buffer_unpack_chat_room(t_buffer* buffer) {
    t_chat_room* chatRoom = malloc(sizeof(t_chat_room));
    buffer_unpack(buffer, &(chatRoom->roomID), sizeof(chatRoom->roomID));
    chatRoom->roomName = buffer_unpack_string(buffer);
    chatRoom->users = NULL;
    return chatRoom;
}

char* buffer_unpack_string(t_buffer* buffer) {
    char* str;
    uint32_t length;
    buffer_unpack(buffer, &length, sizeof(length));
    str = malloc(length);
    buffer_unpack(buffer, str, length);
    return str;
}

void buffer_destroy(t_buffer* buffer) {
    free(buffer->stream);
    free(buffer);
}
