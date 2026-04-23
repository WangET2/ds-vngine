#include "unity.h"

extern int run_flag_tests(void);
extern int run_parser_tests(void);

int main(void){
    run_flag_tests();
    run_parser_tests();
}