#include "utils/buffer_test.h"

static t_buffer* buffer;

void test_utils_buffer_setup(void) {
    buffer = buffer_create();
}

void test_utils_buffer_tear_down(void) {
    buffer_destroy(buffer);
}

void test_buffer_create(void) {
    CU_ASSERT_EQUAL(buffer->size, 0);
    CU_ASSERT_PTR_NULL(buffer->stream);
}

void test_buffer_pack_values(void) {
    uint8_t uint8 = 1;
    uint16_t uint16 = 2;
    uint32_t uint32 = 4;
    uint64_t uint64 = 8;
    buffer_pack(buffer, &uint8, sizeof(uint8));
    buffer_pack(buffer, &uint16, sizeof(uint16));
    buffer_pack(buffer, &uint32, sizeof(uint32));
    buffer_pack(buffer, &uint64, sizeof(uint64));
    CU_ASSERT_EQUAL(buffer->size, 15);
}

void test_buffer_unpack(void) {
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

    CU_ASSERT_EQUAL(uint8_, uint8);
    CU_ASSERT_EQUAL(uint32_, uint32);
    CU_ASSERT_STRING_EQUAL(msg_, msg);
}

void test_buffer_pack_unpack_string(void) {
    buffer_pack_string(buffer, "samsepi0l");
    char* actual = buffer_unpack_string(buffer);
    CU_ASSERT_STRING_EQUAL(actual, "samsepi0l");
}

void test_buffer_pack_unpack_chat_room(void) {
    buffer_pack_chat_room(buffer, 0, "th3g3ntl3man");
    t_chat_room* chatRoom = buffer_unpack_chat_room(buffer);
    CU_ASSERT_EQUAL(chatRoom->roomID, 0);
    CU_ASSERT_STRING_EQUAL(chatRoom->roomName, "th3g3ntl3man");
}
