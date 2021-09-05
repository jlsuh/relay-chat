#ifndef TESTS_UTILS_PACKAGE_TEST_H_
#define TESTS_UTILS_PACKAGE_TEST_H_

#include <CUnit/Basic.h>
#include "package.h"

void test_package_setup(void);
void test_package_tear_down(void);

void test_package_create_from_empty_buffer(void);

#endif