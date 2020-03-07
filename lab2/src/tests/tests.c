//#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "revert_string.h"

void testRevertString(void) {
printf("%d\n",__LINE__);
  char simple_string[] = "Hello";
  char str_with_spaces[] = "String with spaces";
  char str_with_odd_chars_num[] = "abc";
  char str_with_even_chars_num[] = "abcd";

printf("%d\n",__LINE__);
  RevertString(simple_string);
  //CU_ASSERT_STRING_EQUAL_FATAL(simple_string, "olleH");
  assert(0==strcmp("olleH",simple_string));

printf("%d\n",__LINE__);
  RevertString(str_with_spaces);
  //CU_ASSERT_STRING_EQUAL_FATAL(str_with_spaces, "secaps htiw gnirtS");
  assert(0==strcmp("secaps htiw gnirtS",str_with_spaces));

printf("%d\n",__LINE__);
  RevertString(str_with_odd_chars_num);
  //CU_ASSERT_STRING_EQUAL_FATAL(str_with_odd_chars_num, "cba");
  assert(0==strcmp("cba",str_with_odd_chars_num));

printf("%d\n",__LINE__);
  RevertString(str_with_even_chars_num);
  //CU_ASSERT_STRING_EQUAL_FATAL(str_with_even_chars_num, "dcba");
  assert(0==strcmp("dcba",str_with_even_chars_num));
printf("%d\n",__LINE__);
printf("[OK]\n");
}

int main() {
    testRevertString();
//   CU_pSuite pSuite = NULL;

//   /* initialize the CUnit test registry */
//   if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

//   /* add a suite to the registry */
//   pSuite = CU_add_suite("Suite", NULL, NULL);
//   if (NULL == pSuite) {
//     CU_cleanup_registry();
//     return CU_get_error();
//   }

//   /* add the tests to the suite */
//   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
//   if ((NULL == CU_add_test(pSuite, "test of RevertString function",
//                            testRevertString))) {
//     CU_cleanup_registry();
//     return CU_get_error();

  //}

//   /* Run all tests using the CUnit Basic interface */
//   CU_basic_set_mode(CU_BRM_VERBOSE);
//   CU_basic_run_tests();
//   CU_cleanup_registry();
//   return CU_get_error();
return 0;
}
