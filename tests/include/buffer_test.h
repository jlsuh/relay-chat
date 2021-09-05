#ifndef TESTS_UTILS_BUFFER_TEST_H_
#define TESTS_UTILS_BUFFER_TEST_H_

#include <CUnit/Basic.h>
#include "buffer.h"

void test_buffer_setup(void);
void test_buffer_tear_down(void);

void test_buffer_create(void);
void test_buffer_pack_values(void);
void test_buffer_unpack(void);
void test_buffer_pack_unpack_string(void);
void test_buffer_pack_unpack_chat_room(void);

#endif
