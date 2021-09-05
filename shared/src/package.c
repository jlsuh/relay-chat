#include "package.h"

void package_send(int userSocket, void* toSend, uint32_t bufferSize) {
    send(userSocket, toSend, sizeof(uint8_t) + sizeof(uint32_t) + bufferSize, 0);
}

t_package* package_create(uint8_t opCode, t_buffer* buffer) {
    t_package* package = malloc(sizeof(t_package));
    package->opCode = opCode;
    package->buffer = buffer_create();
    memcpy(package->buffer, buffer, sizeof(t_buffer));
    package->buffer->stream = malloc(buffer->size);
    memcpy(package->buffer->stream, buffer->stream, buffer->size);
    return package;
}

void package_destroy(t_package* package) {
    buffer_destroy(package->buffer);
    free(package);
}

void* package_serialize(uint8_t opCode, t_buffer* buffer) {
    t_package* package = package_create(opCode, buffer);

    void* toSend = malloc(sizeof(package->opCode) + sizeof(package->buffer->size) + package->buffer->size);
    int offset = 0;

    memcpy(toSend + offset, &(package->opCode), sizeof(package->opCode));
    offset += sizeof(package->opCode);
    memcpy(toSend + offset, &(package->buffer->size), sizeof(package->buffer->size));
    offset += sizeof(package->buffer->size);
    memcpy(toSend + offset, package->buffer->stream, package->buffer->size);

    package_destroy(package);

    return toSend;
}

void* package_deserialize(int serverSocket, bool deserializeNext) {
    if(!deserializeNext) {
        return NULL;
    }
    t_package* package = malloc(sizeof(t_package));
    package->buffer = malloc(sizeof(t_buffer)); // TODO

    recv(serverSocket, &(package->opCode), sizeof(package->opCode), 0);
    recv(serverSocket, &(package->buffer->size), sizeof(package->buffer->size), 0);
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
