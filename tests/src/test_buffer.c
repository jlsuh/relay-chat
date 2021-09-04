#include "utils/buffer.h"
#include <cspecs/cspec.h>

context (test_buffer) {

    describe ("buffer packing tests") {

        t_buffer* buffer;

        before {
            buffer = buffer_create();
        } end

        after {
            buffer_destroy(buffer);
        } end

        it ("buffer create test") {
            should_int(buffer->size) be equal to(0);
            should_ptr(buffer->stream) be null;
            NULL;
        } end

        it ("buffer pack values test") {
            uint8_t uint8 = 1;
            uint16_t uint16 = 2;
            uint32_t uint32 = 4;
            uint64_t uint64 = 8;
            buffer_pack(buffer, &uint8, sizeof(uint8));
            buffer_pack(buffer, &uint16, sizeof(uint16));
            buffer_pack(buffer, &uint32, sizeof(uint32));
            buffer_pack(buffer, &uint64, sizeof(uint64));
            should_int(buffer->size) be equal to(15);
        } end

        it ("buffer unpack values test") {
            uint8_t uint8 = 1;
            uint32_t uint32 = 4;
            char* msg = "benjamin bolton";
            buffer_pack(buffer, &uint8, sizeof(uint8));
            buffer_pack(buffer, &uint32, sizeof(uint32));
            buffer_pack_string(buffer, msg);

            uint8_t uint8_;
            uint32_t uint32_;
            buffer_unpack(buffer, &uint8_, sizeof(uint8_));
            buffer_unpack(buffer, &uint32_, sizeof(uint32_));
            char* msg_ = buffer_unpack_string(buffer);

            should_int(uint8_) be equal to(uint8);
            should_int(uint32_) be equal to(uint32);
            should_string(msg_) be equal to(msg);
        } end

        it ("buffer pack-unpack string test") {
            buffer_pack_string(buffer, "Test string :)");
            char* result = buffer_unpack_string(buffer);
            should_string(result) be equal to("Test string :)");
        } end

        it ("buffer pack-unpack a chat room test") {
            buffer_pack_chat_room(buffer, 0, "th3g3ntl3man");
            t_chat_room* chatRoom = (t_chat_room*) buffer_unpack_chat_room(buffer);
            should_int(chatRoom->roomID) be equal to(0);
            should_string(chatRoom->roomName) be equal to("th3g3ntl3man");
        } end

    } end

}
