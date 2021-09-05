#include "utils/package_test.h"

static t_package* package;
static t_buffer* buffer;

void test_package_setup(void) {
    buffer = buffer_create();
    package = package_create(STRING, buffer);
}

void test_package_tear_down(void) {
    package_destroy(package);
    buffer_destroy(buffer);
}

void test_package_create_from_empty_buffer(void) {
    CU_ASSERT_EQUAL(package->opCode, STRING);
    CU_ASSERT_EQUAL(package->buffer->size, 0);
}
