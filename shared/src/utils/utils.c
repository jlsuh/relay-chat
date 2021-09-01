#include "utils/utils.h"

void send_package(int userSocket, void* toSend, t_buffer* buffer) {
    send(userSocket, toSend, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);
}

void send_str(int socket, char* str) {
    t_buffer* buffer = buffer_create();
    buffer_pack_string(buffer, str);
    void* toSend = serialize_package(STRING, buffer);
    send_package(socket, toSend, buffer);
    buffer_destroy(buffer);
    free(toSend);
}

void* serialize_package(uint8_t anOpCode, t_buffer* buffer) {
    t_package* package = malloc(sizeof(t_package));
    package->opCode = anOpCode;
    package->buffer = buffer;
    void* toSend = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    int offset = 0;
    memcpy(toSend + offset, &(package->opCode), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(toSend + offset, &(package->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(toSend + offset, package->buffer->stream, package->buffer->size);
    free(package);
    return toSend;
}

void* deserialize_package(int serverSocket, bool deserializeNext) {
    if(!deserializeNext) {
        return NULL;
    }
    t_package* package = malloc(sizeof(t_package));
    package->buffer = malloc(sizeof(t_buffer));

    recv(serverSocket, &(package->opCode), sizeof(uint8_t), 0);
    recv(serverSocket, &(package->buffer->size), sizeof(uint32_t), 0);
    package->buffer->stream = malloc(package->buffer->size);
    recv(serverSocket, package->buffer->stream, package->buffer->size, 0);

    void* deserialized = NULL;
    switch (package->opCode) {
    case STRING:
        deserialized = buffer_unpack_string(package->buffer);
        buffer_destroy(package->buffer);
        break;
    case ROOMINFO:
        deserialized = buffer_unpack_chat_room(package->buffer);
        buffer_destroy(package->buffer);
        break;
    default:
        puts("Non valid OpCode");
        break;
    }
    free(package);
    return deserialized;
}
