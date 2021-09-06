#include "common_utils.h"

void string_send(int socket, char* str) {
    t_buffer* buffer = buffer_create();
    buffer_pack_string(buffer, str);
    uint32_t bufferSize = buffer->size;
    void* toSend = stream_serialize(STRING, buffer);
    stream_send(socket, toSend, bufferSize);
    buffer_destroy(buffer);
    free(toSend);
}

void stream_send(int userSocket, void* toSend, uint32_t bufferSize) {
    send(userSocket, toSend, sizeof(uint8_t) + sizeof(uint32_t) + bufferSize, 0);
}

void* stream_serialize(uint8_t opCode, t_buffer* buffer) {
    void* toSend = malloc(sizeof(opCode) + sizeof(buffer->size) + buffer->size);
    int offset = 0;

    memcpy(toSend + offset, &opCode, sizeof(opCode));
    offset += sizeof(opCode);
    memcpy(toSend + offset, &(buffer->size), sizeof(buffer->size));
    offset += sizeof(buffer->size);
    memcpy(toSend + offset, buffer->stream, buffer->size);

    return toSend;
}

void* stream_deserialize(int serverSocket, bool deserializeNext) {
    if(!deserializeNext) {
        return NULL;
    }
    t_buffer* buffer = buffer_create();
    uint8_t opCode;

    recv(serverSocket, &opCode, sizeof(opCode), 0);
    recv(serverSocket, &(buffer->size), sizeof(buffer->size), 0);
    buffer->stream = malloc(buffer->size);
    recv(serverSocket, buffer->stream, buffer->size, 0);

    void* deserialized = NULL;
    switch (opCode) {
    case STRING:
        deserialized = buffer_unpack_string(buffer);
        buffer_destroy(buffer);
        break;
    case ROOMINFO:
        deserialized = buffer_unpack_chat_room(buffer);
        buffer_destroy(buffer);
        break;
    default:
        puts("Non valid OpCode");
        break;
    }
    return deserialized;
}
