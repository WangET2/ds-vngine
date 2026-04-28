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

typedef enum {
    BLOCK_NONE = 0,
    BLOCK_WAIT,
    BLOCK_TEXT,
    BLOCK_CHOICE
} InterpreterBlockType;

void interpreter_init(void);

void interpreter_reset(void);

void interpreter_shutdown(void);

InterpreterResult interpreter_execute(const ParsedCommand *cmd);

void interpreter_update(void);

InterpreterBlockType interpreter_is_blocked(void);

InterpreterResult interpreter_advance(void);

#endif