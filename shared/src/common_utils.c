#include "common_utils.h"

void string_send(int socket, char* str) {
    t_buffer* buffer = buffer_create();
    buffer_pack_string(buffer, str);
    uint32_t bufferSize = buffer->size;
    void* toSend = package_serialize(STRING, buffer);
    package_send(socket, toSend, bufferSize);
    buffer_destroy(buffer);
    free(toSend);
}
