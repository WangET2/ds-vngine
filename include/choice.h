#ifndef CHOICE_H
#define CHOICE_H

#include <stdbool.h>

enum {
    CHOICE_BASE_X = 32,
    CHOICE_TWO_BASE_Y = 51,
    CHOICE_TWO_OFFSET_Y = 57,
    CHOICE_THREE_BASE_Y = 39,
    CHOICE_THREE_OFFSET_Y = 41,
    CHOICE_FOUR_BASE_Y = 20,
    CHOICE_FOUR_OFFSET_Y = 40
};

enum {
    CHOICE_OVERLAY_WIDTH = 192,
    CHOICE_OVERLAY_HEIGHT = 32
};

void choice_init(char* choices[], int num_choices);
void choice_reset(void);

int choice_get_choice(char *out, int out_size);
int choice_set_choice(int choice_index);
bool choice_set_choice_touch(int x, int y);
int choice_current_index(void);
int choice_num_choices(void);

#endif