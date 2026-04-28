#include "choice.h"
#include <string.h>
#include <stdlib.h>

//basically just shared access to an array... should this handle rendering?

int g_num_choices = 0;
int g_current_index = 0;
char* g_choice_arr[4];

void choice_init(char* choices[], int num_choices){
    for(int i = 0; i < num_choices; ++i)
        g_choice_arr[i] = strdup(choices[i]);
    g_num_choices = num_choices;
}

void choice_reset(void){
    for(int i = 0; i < g_num_choices; ++i)
        free(g_choice_arr[i]);
    g_num_choices = 0;
    g_current_index = 0;
}

char* choice_get_choice(void){
    return g_choice_arr[g_current_index];
}

int choice_set_choice(int choice_index){
    if(choice_index < 0 || choice_index >= g_num_choices) return -1;
    g_current_index = choice_index;
    return 0;
}

int choice_num_choices(void){
    return g_num_choices;
}