#include "unity.h"
#include "choice.h"

void test_choice_two(void){
    char* choices[2] = {"TESTINSTR 1", "TESTINSTR 2"};
    choice_init(choices, 2);
    TEST_ASSERT_EQUAL_INT(2, choice_num_choices());
    choice_set_choice(1);
    TEST_ASSERT_EQUAL_STRING(choice_get_choice(), "TESTINSTR 2");
    choice_set_choice(0);
    TEST_ASSERT_EQUAL_STRING(choice_get_choice(), "TESTINSTR 1");
    choices[0] = NULL;
    TEST_ASSERT_EQUAL_STRING(choice_get_choice(), "TESTINSTR 1");
}

void run_choice_tests(void){
    RUN_TEST(test_choice_two);
    choice_reset();
}