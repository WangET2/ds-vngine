#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>

void text_init(void);
void text_clear(void);

void text_begin_dialogue(const char *speaker, const char *text);
void text_begin_narration(const char *text);

void text_update(void);
void text_finish_immediately(void);

bool text_is_finished(void);

#endif