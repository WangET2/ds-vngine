#include "unity.h"
#include "interpreter.h"
#include "parser.h"
#include "flags.h"
#include "choice.h"
#include "Mockrenderer.h"
#include "Mocktext.h"
#include "Mockdisplay.h"
#include "Mockscript.h"
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
    fill_and_test_interpreter(CMD_HIDE_BG, 0, NULL, INTERPRETER_RESULT_OK, false);
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
    fill_and_test_interpreter(CMD_HIDE_BG_SUB, 0, NULL, INTERPRETER_RESULT_OK, false);
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
    fill_and_test_interpreter(CMD_HIDE_LEFT, 0, NULL, INTERPRETER_RESULT_OK, false);
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
    fill_and_test_interpreter(CMD_HIDE_RIGHT, 0, NULL, INTERPRETER_RESULT_OK, false);
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
    fill_and_test_interpreter(CMD_HIDE_CENTER, 0, NULL, INTERPRETER_RESULT_OK, false);
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

void test_interpreter_ifn_existing(void){
    flags_set("testflag");
    char *args[2] = {"testflag", "FLAG testflag2"};
    fill_and_test_interpreter(CMD_IFN, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_FALSE(flags_has("testflag2"));
}

void test_interpreter_ifn_nonexistent(void){
    char *args[2] = {"testflag", "FLAG testflag2"};
    fill_and_test_interpreter(CMD_IFN, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_TRUE(flags_has("testflag2"));
}

void test_interpreter_ifn_nested_existing(void){
    flags_set("testflag");
    flags_set("testflag2");
    char *args[2] = {"testflag", "IFN testflag2 FLAG testflag3"};
    fill_and_test_interpreter(CMD_IFN, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_FALSE(flags_has("testflag3"));
}

void test_interpreter_ifn_nested_nonexistent(void){
    char *args[2] = {"testflag", "IFN testflag2 FLAG testflag3"};
    fill_and_test_interpreter(CMD_IFN, 2, args, INTERPRETER_RESULT_OK, false);
    TEST_ASSERT_TRUE(flags_has("testflag3"));
}

void test_interpreter_ifn_nested_blocking(void){
    char *args[2] = {"testflag", "IFN testflag2 WAIT"};
    fill_and_test_interpreter(CMD_IFN, 2, args, INTERPRETER_RESULT_BLOCKED, true);
}

void test_interpreter_ifn_nested_dispatch(void){
    char *args[2] = {"testflag", "IFN testflag2 SAY testchar test dialogue"};
    renderer_load_textbox_ExpectAndReturn("maintextbox", true, 0);
    renderer_show_textbox_Expect(true);
    renderer_hide_textbox_Expect(false);
    text_begin_dialogue_Expect("testchar", "test dialogue");
    fill_and_test_interpreter(CMD_IFN, 2, args, INTERPRETER_RESULT_BLOCKED, true);
}

void test_interpreter_choice_two(void){
    char *args[4] = {"text1", "FLAG flag1", "text2", "FLAG flag2"};
    char buf1[PARSER_MAX_TOKEN_LEN];
    renderer_load_textbox_ExpectAndReturn("choicetwo", false, 0);
    renderer_show_textbox_Expect(false);
    renderer_show_choice_overlay_Ignore();
    text_write_choices_Ignore();
    fill_and_test_interpreter(CMD_CHOICE, 4, args, INTERPRETER_RESULT_BLOCKED, true);
    TEST_ASSERT_EQUAL_INT(2, choice_num_choices());
    choice_get_choice(&buf1[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag1", buf1);
    choice_set_choice(1);
    char buf2[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf2[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag2", buf2);
    choice_reset();
}

void test_interpreter_choice_three(void){
    char *args[6] = {"text1", "FLAG flag1", "text2", "FLAG flag2", "text3", "FLAG flag3"};
    char buf1[PARSER_MAX_TOKEN_LEN];
    renderer_load_textbox_ExpectAndReturn("choicethree", false, 0);
    renderer_show_textbox_Expect(false);
    renderer_show_choice_overlay_Ignore();
    text_write_choices_Ignore();
    fill_and_test_interpreter(CMD_CHOICE, 6, args, INTERPRETER_RESULT_BLOCKED, true);
    TEST_ASSERT_EQUAL_INT(3, choice_num_choices());
    choice_get_choice(&buf1[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag1", buf1);
    choice_set_choice(1);
    char buf2[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf2[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag2", buf2);
    choice_set_choice(2);
    char buf3[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf3[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag3", buf3);
    choice_reset();
}

void test_interpreter_choice_four(void){
    char *args[8] = {"text1", "FLAG flag1", "text2", "FLAG flag2", "text3", "FLAG flag3", "text4", "FLAG flag4"};
    char buf1[PARSER_MAX_TOKEN_LEN];
    renderer_load_textbox_ExpectAndReturn("choicefour", false, 0);
    renderer_show_textbox_Expect(false);
    renderer_show_choice_overlay_Ignore();
    text_write_choices_Ignore();
    fill_and_test_interpreter(CMD_CHOICE, 8, args, INTERPRETER_RESULT_BLOCKED, true);
    TEST_ASSERT_EQUAL_INT(4, choice_num_choices());
    choice_get_choice(&buf1[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag1", buf1);
    choice_set_choice(1);
    char buf2[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf2[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag2", buf2);
    choice_set_choice(2);
    char buf3[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf3[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag3", buf3);
    choice_set_choice(3);
    char buf4[PARSER_MAX_TOKEN_LEN];
    choice_get_choice(&buf4[0], PARSER_MAX_TOKEN_LEN);
    TEST_ASSERT_EQUAL_STRING("FLAG flag4", buf4);
    choice_reset();
}

void test_interpreter_choice_dispatch(void){
    char *args[4] = {"text1", "FLAG flag1", "text2", "FLAG flag2"};
    renderer_load_textbox_ExpectAndReturn("choicetwo", false, 0);
    renderer_show_textbox_Expect(false);
    text_write_choices_Ignore();
    renderer_show_choice_overlay_Ignore();
    fill_and_test_interpreter(CMD_CHOICE, 4, args, INTERPRETER_RESULT_BLOCKED, true);
    choice_set_choice(1);
    text_debug_clear_Expect();
    TEST_ASSERT_EQUAL_INT(INTERPRETER_RESULT_OK, interpreter_advance());
    TEST_ASSERT_FALSE(flags_has("flag1"));
    TEST_ASSERT_TRUE(flags_has("flag2"));
    flags_reset();
    flags_init();
    renderer_load_textbox_ExpectAndReturn("choicetwo", false, 0);
    renderer_show_textbox_Expect(false);
    fill_and_test_interpreter(CMD_CHOICE, 4, args, INTERPRETER_RESULT_BLOCKED, true);
    choice_set_choice(0);
    text_debug_clear_Expect();
    TEST_ASSERT_EQUAL_INT(INTERPRETER_RESULT_OK, interpreter_advance());
    TEST_ASSERT_TRUE(flags_has("flag1"));
    TEST_ASSERT_FALSE(flags_has("flag2"));
}

void test_interpreter_choice_nested_dispatch(void){
    char *args[4] = {"text1", "IF flag1 FLAG flag2", "text2", "PASS"};
    renderer_load_textbox_ExpectAndReturn("choicetwo", false, 0);
    renderer_show_textbox_Expect(false);
    text_write_choices_Ignore();
    renderer_show_choice_overlay_Ignore();
    fill_and_test_interpreter(CMD_CHOICE, 4, args, INTERPRETER_RESULT_BLOCKED, true);
    text_debug_clear_Expect();
    flags_set("flag1");
    TEST_ASSERT_EQUAL_INT(INTERPRETER_RESULT_OK, interpreter_advance());
    TEST_ASSERT_TRUE(flags_has("flag2"));
}

void test_interpreter_nested_multi_choice(void){
    char *args[4] = {"text1", "CHOICE text3 {FLAG flag1} text4 {PASS}", "text2", "PASS"};
    renderer_load_textbox_ExpectAndReturn("choicetwo", false, 0);
    renderer_show_textbox_Expect(false);
    renderer_load_textbox_ExpectAndReturn("choicetwo", false, 0);
    renderer_show_textbox_Expect(false);
    text_write_choices_Ignore();
    renderer_show_choice_overlay_Ignore();
    fill_and_test_interpreter(CMD_CHOICE, 4, args, INTERPRETER_RESULT_BLOCKED, true);
    text_debug_clear_Expect();
    TEST_ASSERT_EQUAL_INT(INTERPRETER_RESULT_BLOCKED, interpreter_advance());
    text_debug_clear_Expect();
    TEST_ASSERT_EQUAL_INT(INTERPRETER_RESULT_OK, interpreter_advance());
    TEST_ASSERT_TRUE(flags_has("flag1"));
}

void test_interpreter_pass(void){
    fill_and_test_interpreter(CMD_PASS, 0, NULL, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_wait(void){
    fill_and_test_interpreter(CMD_WAIT, 0, NULL, INTERPRETER_RESULT_BLOCKED, true);
}

void test_interpreter_load(void){
    char *args[1] = {"testscript"};
    script_close_Expect();
    renderer_reset_Expect();
    text_clear_Expect();
    script_open_ExpectAndReturn("testscript", 0);
    fill_and_test_interpreter(CMD_LOAD, 1, args, INTERPRETER_RESULT_OK, false);
}

void test_interpreter_load_error(void){
    char *args[1] = {"testscript"};
    script_close_Expect();
    script_open_ExpectAndReturn("testscript", -1);
    fill_and_test_interpreter(CMD_LOAD, 1, args, INTERPRETER_RESULT_ERROR, false);
}

void test_interpreter_end(void){
    fill_and_test_interpreter(CMD_END, 0, NULL, INTERPRETER_RESULT_FINISHED, false);
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

    RUN_TEST(test_interpreter_ifn_existing);
    RUN_TEST(test_interpreter_ifn_nonexistent);
    RUN_TEST(test_interpreter_ifn_nested_existing);
    RUN_TEST(test_interpreter_ifn_nested_nonexistent);
    RUN_TEST(test_interpreter_ifn_nested_blocking);
    RUN_TEST(test_interpreter_ifn_nested_dispatch);

    RUN_TEST(test_interpreter_choice_two);
    RUN_TEST(test_interpreter_choice_three);
    RUN_TEST(test_interpreter_choice_four);
    RUN_TEST(test_interpreter_choice_dispatch);
    RUN_TEST(test_interpreter_choice_nested_dispatch);
    RUN_TEST(test_interpreter_nested_multi_choice);

    RUN_TEST(test_interpreter_pass);
    RUN_TEST(test_interpreter_wait);
    RUN_TEST(test_interpreter_load);
    RUN_TEST(test_interpreter_load_error);
    RUN_TEST(test_interpreter_end);
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