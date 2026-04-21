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

void test_parser_show_right(void){
    char *input = "SHOW_RIGHT testchar testexpr";
    char *expected_args[2] = {"testchar", "testexpr"};
    verify_command_fields(input, CMD_SHOW_RIGHT, 2, expected_args);
}

void test_parser_show_center(void){
    char *input = "SHOW_CENTER testchar testexpr";
    char *expected_args[2] = {"testchar", "testexpr"};
    verify_command_fields(input, CMD_SHOW_CENTER, 2, expected_args);
}

void test_parser_hide_left(void){
    char *input = "HIDE_LEFT";
    char *expected_args[1];
    verify_command_fields(input, CMD_HIDE_LEFT, 0, expected_args);
}

void test_parser_hide_right(void){
    char *input = "HIDE_RIGHT";
    char *expected_args[1];
    verify_command_fields(input, CMD_HIDE_RIGHT, 0, expected_args);
}

void test_parser_hide_center(void){
    char *input = "HIDE_CENTER";
    char *expected_args[1];
    verify_command_fields(input, CMD_HIDE_CENTER, 0, expected_args);
}

void test_parser_flag(void){
    char *input = "FLAG testflag";
    char *expected_args[1] = {"testflag"};
    verify_command_fields(input, CMD_FLAG, 1, expected_args);
}

void test_parser_unset(void){
    char *input = "UNSET testflag";
    char *expected_args[1] = {"testflag"};
    verify_command_fields(input, CMD_UNSET, 1, expected_args);
}

void test_parser_if(void){
    char *input = "IF testflag NESTEDINSTRUCTION instructionargs";
    char *expected_args[2] = {"testflag", "NESTEDINSTRUCTION instructionargs"};
    verify_command_fields(input, CMD_IF, 2, expected_args);
}

//TODO: Write CHOICE tests for variable num_args.
void test_parser_choice(void){
    return;
}

void test_parser_say(void){
    char *input = "SAY testcharacter This is a line of test dialogue.";
    char *expected_args[2] = {"testcharacter", "This is a line of test dialogue."};
    verify_command_fields(input, CMD_SAY, 2, expected_args);
}

void test_parser_narrate(void){
    char *input = "NARRATE This is a line of test narration.";
    char *expected_args[1] = {"This is a line of test narration."};
    verify_command_fields(input, CMD_NARRATE, 1, expected_args);
}

void test_parser_hide_say(void){
    char *input = "HIDE_SAY";
    char *expected_args[1];
    verify_command_fields(input, CMD_HIDE_SAY, 0, expected_args);
}

void test_parser_hide_narrate(void){
    char *input = "HIDE_NARRATE";
    char *expected_args[1];
    verify_command_fields(input, CMD_HIDE_NARRATE, 0, expected_args);
}

void test_parser_bgm(void){
    char *input = "BGM testbgm";
    char *expected_args[1] = {"testbgm"};
    verify_command_fields(input, CMD_BGM, 1, expected_args);
}

void test_parser_sfx(void){
    char *input = "SFX testsfx";
    char *expected_args[1] = {"testsfx"};
    verify_command_fields(input, CMD_SFX, 1, expected_args);
}

void test_parser_endbgm(void){
    char *input = "ENDBGM";
    char *expected_args[1];
    verify_command_fields(input, CMD_ENDBGM, 0, expected_args);
}

void test_parser_wait(void){
    char *input = "WAIT";
    char *expected_args[1];
    verify_command_fields(input, CMD_WAIT, 0, expected_args); 
}

void test_parser_load(void){
    char *input = "LOAD testscript";
    char *expected_args[1] = {"testscript"};
    verify_command_fields(input, CMD_LOAD, 1, expected_args);
}

void test_parser_pass(void){
    char *input = "PASS";
    char *expected_args[1];
    verify_command_fields(input, CMD_PASS, 0, expected_args);
}

void test_parser_end(void){
    char *input = "END";
    char *expected_args[1];
    verify_command_fields(input, CMD_END, 0, expected_args);
}

void test_parser_missing_arguments(void){
    char *input1 = "SAY testcharacter";
    ParsedCommand cmd1;
    ParserResult parse_res1 = parser_parse_line(input1, &cmd1);
    TEST_ASSERT_EQUAL_INT(parse_res1, PARSER_RESULT_ERROR);

    char *input2 = "BG";
    ParsedCommand cmd2;
    ParserResult parse_res2 = parser_parse_line(input2, &cmd2);
    TEST_ASSERT_EQUAL_INT(parse_res2, PARSER_RESULT_ERROR);
}

void test_parser_empty(void){
    char *input1 = "";
    ParsedCommand cmd1;
    ParserResult parse_res1 = parser_parse_line(input1, &cmd1);
    TEST_ASSERT_EQUAL_INT(parse_res1, PARSER_RESULT_EMPTY);

    char *input2 = "  ";
    ParsedCommand cmd2;
    ParserResult parse_res2 = parser_parse_line(input2, &cmd2);
    TEST_ASSERT_EQUAL_INT(parse_res2, PARSER_RESULT_EMPTY);

    char *input3 = "    ";
    ParsedCommand cmd3;
    ParserResult parse_res3 = parser_parse_line(input3, &cmd3);
    TEST_ASSERT_EQUAL_INT(parse_res3, PARSER_RESULT_EMPTY);
}

void test_parser_comment(void){
    char *input = "#This is an example comment.";
    ParsedCommand cmd;
    ParserResult parse_res = parser_parse_line(input, &cmd);
    TEST_ASSERT_EQUAL_INT(parse_res, PARSER_RESULT_EMPTY);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_parser_bg);
    RUN_TEST(test_parser_bg_sub);
    RUN_TEST(test_parser_hide_bg);
    RUN_TEST(test_parser_hide_bg_sub);

    RUN_TEST(test_parser_show_left);
    RUN_TEST(test_parser_show_right);
    RUN_TEST(test_parser_show_center);
    RUN_TEST(test_parser_hide_left);
    RUN_TEST(test_parser_hide_right);
    RUN_TEST(test_parser_hide_center);

    RUN_TEST(test_parser_flag);
    RUN_TEST(test_parser_unset);
    RUN_TEST(test_parser_if);

    RUN_TEST(test_parser_say);
    RUN_TEST(test_parser_narrate);
    RUN_TEST(test_parser_hide_say);
    RUN_TEST(test_parser_hide_narrate);

    RUN_TEST(test_parser_bgm);
    RUN_TEST(test_parser_sfx);
    RUN_TEST(test_parser_endbgm);

    RUN_TEST(test_parser_wait);
    RUN_TEST(test_parser_load);
    RUN_TEST(test_parser_pass);
    RUN_TEST(test_parser_end);

    RUN_TEST(test_parser_leading_whitespace);
    RUN_TEST(test_parser_leading_tabs);
    RUN_TEST(test_parser_choice);

    RUN_TEST(test_parser_missing_arguments);
    RUN_TEST(test_parser_empty);
    RUN_TEST(test_parser_comment);
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