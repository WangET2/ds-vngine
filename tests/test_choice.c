#include "unity.h"
#include "parser.h"
#include "choice.h"

void test_choice_two(void){
    char* choices[2] = {"TESTINSTR 1", "TESTINSTR 2"};
    choice_init(choices, 2);
    TEST_ASSERT_EQUAL_INT(2, choice_num_choices());
    choice_set_choice(1);
    char buf1[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf1[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 2", buf1);
    choice_set_choice(0);
    char buf2[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf2[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 1", buf2);
    choices[0] = NULL;
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 1", buf2);
    choice_reset();
}

void test_choice_three(void){
    char* choices[3] = {"TESTINSTR 1", "TESTINSTR 2", "TESTINSTR 3"};
    choice_init(choices, 3);
    TEST_ASSERT_EQUAL_INT(3, choice_num_choices());
    choice_set_choice(1);
    char buf1[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf1[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 2", buf1);
    choice_set_choice(0);
    char buf2[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf2[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 1", buf2);
    choice_set_choice(2);
    char buf3[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf3[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 3", buf3);
    choice_reset();
}

void test_choice_four(void){
    char* choices[4] = {"TESTINSTR 1", "TESTINSTR 2", "TESTINSTR 3", "TESTINSTR 4"};
    choice_init(choices, 4);
    TEST_ASSERT_EQUAL_INT(4, choice_num_choices());
    choice_set_choice(1);
    char buf1[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf1[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 2", buf1);
    choice_set_choice(0);
    char buf2[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf2[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 1", buf2);
    choice_set_choice(2);
    char buf3[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf3[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 3", buf3);
    choice_set_choice(3);
    char buf4[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf4[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("TESTINSTR 4", buf4);
    choice_reset();
}

void run_choice_tests(void){
    RUN_TEST(test_choice_two);
    RUN_TEST(test_choice_three);
    RUN_TEST(test_choice_four);
}