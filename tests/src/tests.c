#include "tests.h"

int main(int argc, char** argv) {
    CU_initialize_registry();
    CU_basic_set_mode(CU_BRM_VERBOSE);

    utils_tests();

    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}

void utils_tests(void) {

    CU_pSuite bufferSuite = CU_add_suite_with_setup_and_teardown(
        "utils/buffer.h Test Suite",
        NULL,
        NULL,
        test_utils_buffer_setup,
        test_utils_buffer_tear_down
    );
    CU_add_test(bufferSuite, "test_buffer_create", test_buffer_create);
    CU_add_test(bufferSuite, "test_buffer_pack_values", test_buffer_pack_values);
    CU_add_test(bufferSuite, "test_buffer_unpack", test_buffer_unpack);
    CU_add_test(bufferSuite, "test_buffer_pack_unpack_string", test_buffer_pack_unpack_string);
    CU_add_test(bufferSuite, "test_buffer_pack_unpack_chat_room", test_buffer_pack_unpack_chat_room);

}
