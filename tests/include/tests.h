#ifndef TESTS_UTILS_TESTS_H_
#define TESTS_UTILS_TESTS_H_

#include "utils/buffer_test.h"

#define FUNCTION_TEST(f) { "\033[93m"#f"\033[0m", f }
#define ARRAY_LENGTH(array)	(sizeof((array)) / sizeof(*(array)))
#define ADD_TEST_CASES_TO_TEST_SUITE(test_suite, test_functions)\
	for(unsigned long i = 0; i < ARRAY_LENGTH(test_functions); i++)\
		CU_add_test(test_suite, test_functions[i].testName, test_functions[i].test);


typedef struct {
    const char* testName;
	void(*test)(void);
} t_test_case;

void utils_tests(void);

#endif
