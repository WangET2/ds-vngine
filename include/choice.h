#ifndef CHOICE_H
#define CHOICE_H

void choice_init(char* choices[], int num_choices);
void choice_reset(void);

int choice_get_choice(char* out, int out_size);
int choice_set_choice(int choice_index);
int choice_num_choices(void);

#endif