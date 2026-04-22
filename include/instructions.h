#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "parser.h"
#include <stddef.h>
#include <stdbool.h>

struct InstructionKeyword {
    const char *name;
    CommandType type;
    int num_args;
    bool consumes_rest;
};

const struct InstructionKeyword *in_word_set(const char *str, size_t len);

#endif