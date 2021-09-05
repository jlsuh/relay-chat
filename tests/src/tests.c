#include "tests.h"

int main(int argc, char* argv[]) {
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
        test_buffer_setup,
        test_buffer_tear_down
    );
    t_test_case bufferTestCases[] = {
        FUNCTION_TEST(test_buffer_create),
        FUNCTION_TEST(test_buffer_pack_values),
        FUNCTION_TEST(test_buffer_unpack),
        FUNCTION_TEST(test_buffer_pack_unpack_string),
        FUNCTION_TEST(test_buffer_pack_unpack_chat_room),
    };
    ADD_TEST_CASES_TO_SUITE(bufferSuite, bufferTestCases);

    CU_pSuite packageSuite = CU_add_suite_with_setup_and_teardown(
        "utils/package.h Test Suite",
        NULL,
        NULL,
        test_package_setup,
        test_package_tear_down
    );
    t_test_case packageTestCases[] = {
        FUNCTION_TEST(test_package_create_from_empty_buffer),
    };
    ADD_TEST_CASES_TO_SUITE(packageSuite, packageTestCases);

}
