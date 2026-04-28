#ifndef CHOICE_H
#define CHOICE_H

void choice_init(char* choices[], int num_choices);
void choice_reset(void);

char* choice_get_choice(void);
int choice_set_choice(int choice_index);
int choice_num_choices(void);

#endif