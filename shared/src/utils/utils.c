#include "utils/utils.h"

void send_package(int userSocket, void* toSend, t_buffer* buffer) {
    send(userSocket, toSend, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);
}

void send_str_msg(int socket, char* str) {
    t_string msg;
    msg.content = strdup(str);
    msg.length = strlen(msg.content) + 1;
    t_buffer* buffer = serialize_string(msg);

    void* toSend = serialize_package(STRING, buffer);
    send_package(socket, toSend, buffer);

    free(buffer->stream);
    free(buffer);
    free(msg.content);
    free(toSend);
}

t_buffer* serialize_string(t_string msg) {
    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer->size = sizeof(uint32_t)
                   + strlen(msg.content) + 1;

    void* stream = malloc(buffer->size);
    int offset = 0;

    memcpy(stream + offset, &msg.length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, msg.content, strlen(msg.content) + 1);

    buffer->stream = stream;

    return buffer;
}

t_buffer* serialize_chat_room(t_string roomName, uint32_t roomID) {
    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer->size = sizeof(uint32_t) * 2
                   + strlen(roomName.content) + 1;
    
    void* stream = malloc(buffer->size);
    int offset = 0;

    memcpy(stream + offset, &roomID, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &roomName.length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, roomName.content, strlen(roomName.content) + 1);

    buffer->stream = stream;

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

t_string* deserialize_string(t_buffer* buffer) {
    t_string* msg = malloc(sizeof(t_string));

    void* stream = buffer->stream;

    memcpy(&(msg->length), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    msg->content = malloc(msg->length);
    memcpy(msg->content, stream, msg->length);

    free(buffer->stream);
    free(buffer);

    return msg;
}

t_config* get_cwd_config(char* configFileName) {
    char* cwd = getcwd(NULL, 0);
    printf("CWD: %s\n", cwd);
    char* fullConfigPath = string_from_format("%s/%s", cwd, configFileName);
    printf("Full path: %s\n", fullConfigPath);
    t_config* config = config_create(fullConfigPath);
    free(cwd);
    free(fullConfigPath);
    return config;
}
