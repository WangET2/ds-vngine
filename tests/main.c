#include "unity.h"

extern void run_flag_tests(void);
extern void run_parser_tests(void);

int main(void){
    UNITY_BEGIN();
    run_flag_tests();
    run_parser_tests();
    return UNITY_END();
}