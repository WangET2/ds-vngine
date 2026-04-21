#include "unity.h"
#include "parser.h"

void verify_command_fields(char *input, CommandType expected_res, int expected_num_args, char *expected_args[]);

void setUp(void){
    return;
}

void tearDown(void){
    return;
}

void test_parser_bg(void){
    char *input = "BG testbg";
    char *expected_args[1] = {"testbg"};
    verify_command_fields(input, CMD_BG, 1, expected_args);
}

void test_parser_leading_whitespace(void){
    char *input = "   BG testbg";
    char *expected_args[1] = {"testbg"};
    verify_command_fields(input, CMD_BG, 1, expected_args);
}

void test_parser_leading_tabs(void){
    char *input = "     BG testbg";
    char *expected_args[1] = {"testbg"};
    verify_command_fields(input, CMD_BG, 1, expected_args);
}

void test_parser_bg_sub(void){
    char *input = "BGSUB testbg";
    char *expected_args[1] = {"testbg"};
    verify_command_fields(input, CMD_BG_SUB, 1, expected_args);
}

void test_parser_hide_bg(void){
    char *input = "HIDE_BG";
    char *expected_args[1];
    verify_command_fields(input, CMD_HIDE_BG, 0, expected_args);
}

void test_parser_hide_bg_sub(void){
    char *input = "HIDE_BGSUB";
    char *expected_args[1];
    verify_command_fields(input, CMD_HIDE_BG_SUB, 0, expected_args);
}

void test_parser_show_left(void){
    char *input = "SHOW_LEFT testchar testexpr";
    char *expected_args[2] = {"testchar", "testexpr"};
    verify_command_fields(input, CMD_SHOW_LEFT, 2, expected_args);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_parser_bg);
    RUN_TEST(test_parser_leading_whitespace);
    RUN_TEST(test_parser_leading_tabs);
    RUN_TEST(test_parser_bg_sub);
    RUN_TEST(test_parser_hide_bg);
    RUN_TEST(test_parser_hide_bg_sub);
    RUN_TEST(test_parser_show_left);
    return UNITY_END();
}

void verify_command_fields(char *input, CommandType expected_res, int expected_num_args, char *expected_args[]){
    ParsedCommand cmd;
    ParserResult res = parser_parse_line(input, &cmd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(res, PARSER_RESULT_OK, "Expected PARSER_RESULT_OK.");
    TEST_ASSERT_NOT_NULL(&cmd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(cmd.command, expected_res, "Wrong ParserResult generated.");
    TEST_ASSERT_EQUAL_INT(cmd.num_args, expected_num_args);
    for(int i = 0; i < expected_num_args; ++i)
        TEST_ASSERT_EQUAL_STRING(cmd.args[i], expected_args[i]);
}