#include "utils/utils.h"

void send_package(int userSocket, void* toSend, t_buffer* buffer) {
    send(userSocket, toSend, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);
}

void send_str_msg(int socket, char* str) {
    t_buffer* buffer = serialize_string(str);

    void* toSend = serialize_package(STRING, buffer);
    send_package(socket, toSend, buffer);

    free(buffer->stream);
    free(buffer);
    // free(msg.content);
    free(toSend);
}

t_buffer* create_buffer(size_t size, void* stream) {
    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer->size = size;
    buffer->stream = stream;
    return buffer;
}

t_buffer* serialize_string(char* msg) {
    uint32_t length = strlen(msg) + 1;
    size_t size = sizeof(uint32_t) + length;
    void* stream = malloc(size);
    int offset = 0;

    memcpy(stream + offset, &length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, msg, length);

    t_buffer* buffer = create_buffer(size, stream);

    return buffer;
}

t_buffer* serialize_chat_room(char* roomName, uint32_t roomID) {
    uint32_t length = strlen(roomName) + 1;
    size_t size = sizeof(uint32_t) * 2 + length;
    void* stream = malloc(size);
    int offset = 0;

    memcpy(stream + offset, &roomID, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, roomName, length);

    t_buffer* buffer = create_buffer(size, stream);

    return buffer;
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
        deserialized = deserialize_string(package->buffer);
        break;
    case ROOMINFO:
        deserialized = deserialize_chat_room(package->buffer);
        break;
    default:
        puts("Non valid OpCode");
        break;
    }
    free(package);
    return deserialized;
}

t_chat_room* deserialize_chat_room(t_buffer* buffer) {
    t_chat_room* chatRoom = malloc(sizeof(t_chat_room));
    uint32_t len;

    void* stream = buffer->stream;

    memcpy(&(chatRoom->roomID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&len, stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    chatRoom->roomName = malloc(len);
    memcpy(chatRoom->roomName, stream, len);

    free(buffer->stream);
    free(buffer);

    return chatRoom;
}

char* deserialize_string(t_buffer* buffer) {
    char* msg;
    uint32_t length;

    void* stream = buffer->stream;

    memcpy(&length, stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    msg = malloc(length);
    memcpy(msg, stream, length);

    free(buffer->stream);
    free(buffer);

    return msg;
}
