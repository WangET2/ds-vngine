#include "unity.h"
#include "interpreter.h"
#include "parser.h"
#include "flags.h"
#include "Mockrenderer.h"
#include "Mocktext.h"
#include "Mockdisplay.h"
#include <string.h>
#include <stdbool.h>

void fill_and_test_interpreter(CommandType cmd_type, int num_args, char *args[], InterpreterResult expected_res, bool blocking);

void setUp(){
    Mockrenderer_Init();
    Mockdisplay_Init();
    Mocktext_Init();
    flags_init();
}

void tearDown(){
    Mockrenderer_Verify();
    Mockrenderer_Destroy();
    Mockdisplay_Verify();
    Mockdisplay_Destroy();
    Mocktext_Verify();
    Mocktext_Destroy();
    interpreter_reset();
    flags_reset();
}

void test_interpreter_bg(void){
    renderer_set_background_ExpectAndReturn("testbg", true, 0);
    char *args[1] = {"testbg"};
    fill_and_test_interpreter(CMD_BG, 1, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_bg_error(void){
    renderer_set_background_ExpectAndReturn("testbg", true, -1);
    char *args[1] = {"testbg"};
    fill_and_test_interpreter(CMD_BG, 1, args, INTERPRETER_RESULT_ERROR, false);
}

void test_interpreter_hide_bg(void){
    renderer_hide_background_Expect(true);
    char *args[1];
    fill_and_test_interpreter(CMD_HIDE_BG, 0, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_bgsub(void){
    renderer_set_background_ExpectAndReturn("testbg", false, 0);
    char *args[1] = {"testbg"};
    fill_and_test_interpreter(CMD_BG_SUB, 1, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_bgsub_error(void){
    renderer_set_background_ExpectAndReturn("testbg", false, -1);
    char *args[1] = {"testbg"};
    fill_and_test_interpreter(CMD_BG_SUB, 1, args, INTERPRETER_RESULT_ERROR, false);
}

void test_interpreter_hide_bgsub(void){
    renderer_hide_background_Expect(false);
    char *args[1];
    fill_and_test_interpreter(CMD_HIDE_BG_SUB, 0, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_show_left(void){
    renderer_show_left_ExpectAndReturn("testchar", "testexpr", 0);
    char *args[2] = {"testchar", "testexpr"};
    fill_and_test_interpreter(CMD_SHOW_LEFT, 2, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_show_left_error(void){
    renderer_show_left_ExpectAndReturn("testchar", "testexpr", -1);
    char *args[2] = {"testchar", "testexpr"};
    fill_and_test_interpreter(CMD_SHOW_LEFT, 2, args, INTERPRETER_RESULT_ERROR, false);
}

void test_interpreter_hide_left(void){
    renderer_hide_left_Expect();
    char *args[1];
    fill_and_test_interpreter(CMD_HIDE_LEFT, 0, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_show_right(void){
    renderer_show_right_ExpectAndReturn("testchar", "testexpr", 0);
    char *args[2] = {"testchar", "testexpr"};
    fill_and_test_interpreter(CMD_SHOW_RIGHT, 2, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_show_right_error(void){
    renderer_show_right_ExpectAndReturn("testchar", "testexpr", -1);
    char *args[2] = {"testchar", "testexpr"};
    fill_and_test_interpreter(CMD_SHOW_RIGHT, 2, args, INTERPRETER_RESULT_ERROR, false);
}

void test_interpreter_hide_right(void){
    renderer_hide_right_Expect();
    char *args[1];
    fill_and_test_interpreter(CMD_HIDE_RIGHT, 0, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_show_center(void){
    renderer_show_center_ExpectAndReturn("testchar", "testexpr", 0);
    char *args[2] = {"testchar", "testexpr"};
    fill_and_test_interpreter(CMD_SHOW_CENTER, 2, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_show_center_error(void){
    renderer_show_center_ExpectAndReturn("testchar", "testexpr", -1);
    char *args[2] = {"testchar", "testexpr"};
    fill_and_test_interpreter(CMD_SHOW_CENTER, 2, args, INTERPRETER_RESULT_ERROR, false);
}

void test_interpreter_hide_center(void){
    renderer_hide_center_Expect();
    char *args[1];
    fill_and_test_interpreter(CMD_HIDE_CENTER, 0, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_flag(void){
    char *args[1] = {"testflag"};
    fill_and_test_interpreter(CMD_FLAG, 1, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_TRUE(flags_has("testflag"));
}

void test_interpreter_unset_existing(void){
    flags_set("testflag");
    char *args[1] = {"testflag"};
    fill_and_test_interpreter(CMD_UNSET, 1, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_FALSE(flags_has("testflag"));
}

void test_interpreter_unset_nonexistent(void){
    char *args[1] = {"testflag"};
    fill_and_test_interpreter(CMD_UNSET, 1, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_FALSE(flags_has("testflag"));
}

void test_interpreter_if_existing(void){
    flags_set("testflag");
    char *args[2] = {"testflag", "FLAG testflag2"};
    fill_and_test_interpreter(CMD_IF, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_TRUE(flags_has("testflag2"));
}

void test_interpreter_if_nonexistent(void){
    char *args[2] = {"testflag", "FLAG testflag2"};
    fill_and_test_interpreter(CMD_IF, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_FALSE(flags_has("testflag2"));
}

void test_interpreter_if_nested_existing(void){
    flags_set("testflag");
    flags_set("testflag2");
    char *args[2] = {"testflag", "IF testflag2 FLAG testflag3"};
    fill_and_test_interpreter(CMD_IF, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_TRUE(flags_has("testflag3"));
}

void test_interpreter_if_nested_nonexistent(void){
    flags_set("testflag");
    char *args[2] = {"testflag", "IF testflag2 FLAG testflag3"};
    fill_and_test_interpreter(CMD_IF, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_FALSE(flags_has("testflag3"));
}

void test_interpreter_if_nested_blocking(void){
    flags_set("testflag");
    flags_set("testflag2");
    char *args[2] = {"testflag", "IF testflag2 WAIT"};
    fill_and_test_interpreter(CMD_IF, 2, args, INTERPRETER_RESULT_BLOCKED, true);
}

void test_interpreter_if_nested_dispatch(void){
    flags_set("testflag");
    flags_set("testflag2");
    char *args[2] = {"testflag", "IF testflag2 SAY testchar test dialogue"};
    renderer_load_textbox_ExpectAndReturn("maintextbox", true, 0);
    renderer_show_textbox_Expect(true);
    renderer_hide_textbox_Expect(false);
    text_begin_dialogue_Expect("testchar", "test dialogue");
    fill_and_test_interpreter(CMD_IF, 2, args, INTERPRETER_RESULT_BLOCKED, true);
}

void test_interpreter_pass(void){
    ParsedCommand cmd;
    cmd.command = CMD_PASS;
    cmd.num_args = 0;
    InterpreterResult res = interpreter_execute(&cmd);
    TEST_ASSERT_EQUAL_INT(INTERPRETER_RESULT_OK, res);
    TEST_ASSERT_FALSE(interpreter_is_blocked());
}

void run_interpreter_tests(void){
    RUN_TEST(test_interpreter_bg);
    RUN_TEST(test_interpreter_bg_error);
    RUN_TEST(test_interpreter_hide_bg);
    RUN_TEST(test_interpreter_bgsub);
    RUN_TEST(test_interpreter_bgsub_error);
    RUN_TEST(test_interpreter_hide_bgsub);

    RUN_TEST(test_interpreter_show_left);
    RUN_TEST(test_interpreter_show_left_error);
    RUN_TEST(test_interpreter_hide_left);
    RUN_TEST(test_interpreter_show_right);
    RUN_TEST(test_interpreter_show_right_error);
    RUN_TEST(test_interpreter_hide_right);
    RUN_TEST(test_interpreter_show_center);
    RUN_TEST(test_interpreter_show_center_error);
    RUN_TEST(test_interpreter_hide_center);

    RUN_TEST(test_interpreter_flag);
    RUN_TEST(test_interpreter_unset_existing);
    RUN_TEST(test_interpreter_unset_nonexistent);
    RUN_TEST(test_interpreter_if_existing);
    RUN_TEST(test_interpreter_if_nonexistent);
    RUN_TEST(test_interpreter_if_nested_existing);
    RUN_TEST(test_interpreter_if_nested_nonexistent);
    RUN_TEST(test_interpreter_if_nested_blocking);
    RUN_TEST(test_interpreter_if_nested_dispatch);

    RUN_TEST(test_interpreter_pass);
}

void fill_and_test_interpreter(CommandType cmd_type, int arg_count, char *filled_args[], InterpreterResult expected_res, bool blocking){
    ParsedCommand cmd;
    cmd.command = cmd_type;
    cmd.num_args = arg_count;
    for(int i = 0; i < arg_count; ++i) strncpy(cmd.args[i], filled_args[i], PARSER_MAX_TOKEN_LEN);
    InterpreterResult res = interpreter_execute(&cmd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected_res, res, "Incorrect interpreter result generated.");
    if(blocking) TEST_ASSERT_TRUE(interpreter_is_blocked());
    if(!blocking) TEST_ASSERT_FALSE(interpreter_is_blocked());
}