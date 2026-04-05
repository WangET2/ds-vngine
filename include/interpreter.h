#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <stdbool.h>
#include "parser.h"

typedef enum {
    INTERPRETER_RESULT_OK = 0,
    INTERPRETER_RESULT_BLOCKED,
    INTERPRETER_RESULT_FINISHED,
    INTERPRETER_RESULT_ERROR
} InterpreterResult;

void interpreter_init(void);

void interpreter_reset(void);

InterpreterResult interpreter_execute(const ParsedCommand *cmd);

void interpreter_update(void);

bool interpreter_is_blocked(void);

void interpreter_advance(void);

#endif