#include "unity.h"
#include "flags.h"

void setUp(void){
    flags_init();
}

void tearDown(void){
    flags_reset();
}

void test_flags_set_basic(void){
    flags_set("testflag");
    TEST_ASSERT_TRUE(flags_has("testflag"));
}

void test_flags_unset_basic(void){
    flags_set("testflag");
    flags_unset("testflag");
    TEST_ASSERT_FALSE(flags_has("testflag"));
}

void test_flags_unset_nonexistent(void){
    TEST_ASSERT_FALSE(flags_unset("testflag"));
}

void test_flags_set_duplicate(void){
    TEST_ASSERT_TRUE(flags_set("testflag"));
    TEST_ASSERT_TRUE(flags_set("testflag"));
    TEST_ASSERT_TRUE(flags_set("testflag"));

    TEST_ASSERT_TRUE(flags_has("testflag"));

    flags_unset("testflag");
    TEST_ASSERT_FALSE(flags_has("testflag"));
}

void test_flags_unset_duplicate(void){
    flags_set("testflag");
    TEST_ASSERT_TRUE(flags_unset("testflag"));
    TEST_ASSERT_FALSE(flags_unset("testflag"));
    TEST_ASSERT_FALSE(flags_has("testflag"));
}

void test_flags_has_false(void){
    TEST_ASSERT_FALSE(flags_has("testflag"));
}

void test_flags_large_load_factor(void){
    for(int i = 0; i < 128; ++i){
        char str[4];
        snprintf(str, sizeof(str), "%d", i);
        TEST_ASSERT_TRUE(flags_set(str));
    }
    for(int i = 0; i < 128; ++i){
        char str[4];
        snprintf(str, sizeof(str), "%d", i);
        TEST_ASSERT_TRUE(flags_has(str));
    }
    for(int i = 0; i < 128; ++i){
        char str[4];
        snprintf(str, sizeof(str), "%d", i);
        TEST_ASSERT_TRUE(flags_unset(str));
    }
    for(int i = 0; i < 128; ++i){
        char str[4];
        snprintf(str, sizeof(str), "%d", i);
        TEST_ASSERT_FALSE(flags_has(str));
    }
}

void run_flag_tests(void){
    //UNITY_BEGIN();
    RUN_TEST(test_flags_set_basic);
    RUN_TEST(test_flags_unset_basic);
    RUN_TEST(test_flags_unset_nonexistent);
    RUN_TEST(test_flags_set_duplicate);
    RUN_TEST(test_flags_unset_duplicate);
    RUN_TEST(test_flags_has_false);
    RUN_TEST(test_flags_large_load_factor);
    //return UNITY_END();
}