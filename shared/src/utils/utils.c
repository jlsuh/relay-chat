#include "utils/utils.h"

void send_package(int userSocket, void* toSend, t_buffer* buffer) {
    send(userSocket, toSend, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);
}

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

void send_str(int socket, char* str) {
    t_buffer* buffer = buffer_create();
    buffer_pack_string(buffer, str);
    void* toSend = serialize_package(STRING, buffer);
    send_package(socket, toSend, buffer);
    free(buffer->stream);
    free(buffer);
    free(toSend);
}

void* buffer_pack_string(t_buffer* buffer, char* stringToAdd) {
    uint32_t length = strlen(stringToAdd) + 1;
    buffer_pack(buffer, &length, sizeof(length));
    buffer->stream = realloc(buffer->stream, buffer->size + length);
	memcpy(buffer->stream + buffer->size, stringToAdd, length);
	buffer->size += length;
    return buffer;
}

void* buffer_pack_chat_room(t_buffer* buffer, char* roomName, uint32_t roomID) {
    buffer_pack(buffer, &roomID, sizeof(roomID));
    buffer_pack_string(buffer, roomName);
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
        deserialized = buffer_unpack_string(package->buffer);
        break;
    case ROOMINFO:
        deserialized = buffer_unpack_chat_room(package->buffer);
        break;
    default:
        puts("Non valid OpCode");
        break;
    }
    free(package);
    return deserialized;
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
	buffer_unpack(buffer, &length, sizeof(uint32_t));
	str = malloc(length);
	buffer_unpack(buffer, str, length);

    free(buffer->stream);
    free(buffer);

	return str;
}
