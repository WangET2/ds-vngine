#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "parser.h"
#include <stddef.h>

struct InstructionKeyword {
    const char *name;
    CommandType type;
};

const struct InstructionKeyword *in_word_set(const char *str, size_t len);

#endif