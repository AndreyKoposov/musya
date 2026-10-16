#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>

#include "http.h"


void test_add_status(void) {
    char buffer[256] = {0};
    char str_status[] = "HTTP/1.1 200 OK\r\n";

    add_status(buffer, sizeof(buffer), 200);
    CU_ASSERT_STRING_EQUAL_FATAL(buffer, str_status);
}

void test_add_content_type(void) {
    char buffer[256] = {0};
    char str_content_type[] = "Content-Type: text/html; charset=utf-8\r\n";

    add_content_type(buffer, sizeof(buffer));
    CU_ASSERT_STRING_EQUAL_FATAL(buffer, str_content_type);
}

void test_add_content_length(void) {
    char buffer[256] = {0};
    char str_content_type[] = "Content-Length: 22\r\n";

    add_content_length(buffer, sizeof(buffer), 22);
    CU_ASSERT_STRING_EQUAL_FATAL(buffer, str_content_type);
}

void test_add_connection_close(void) {
    char buffer[256] = {0};
    char str_content_close[] = "Connection: close\r\n";

    add_connection_close(buffer, sizeof(buffer));
    CU_ASSERT_STRING_EQUAL_FATAL(buffer, str_content_close);
}

void test_add_body(void) {
    char buffer[256] = {0};
    char str_body[] = "<h1>Hello, World!</h1>";
    char str_expected[] = "\r\n<h1>Hello, World!</h1>";

    add_body(buffer, sizeof(buffer), str_body);
    CU_ASSERT_STRING_EQUAL_FATAL(buffer, str_expected);
}


int main() {
  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("Suite", NULL, NULL);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
  void* success = CU_add_test(pSuite, "test of add_status function", test_add_status);
  success = CU_add_test(pSuite, "test of add_content_type function", test_add_content_type);
  success = CU_add_test(pSuite, "test of add_content_length function", test_add_content_length);
  success = CU_add_test(pSuite, "test of add_connection_close function", test_add_connection_close);
  success = CU_add_test(pSuite, "test of add_body function", test_add_body);
  if (success == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}