#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

typedef enum{
    SCRIPT_RESULT_OK = 0,
    SCRIPT_RESULT_EOF,
    SCRIPT_RESULT_ERROR
} ScriptResult;

int script_open(const char *scene_name);

void script_close(void);

ScriptResult script_read_line(char *out_line, size_t out_size);

bool script_is_open(void);

int script_current_line(void);

//int script_seek_line(int line_number);

//int script_get_path(char *out_path, size_t out_size);

#endif