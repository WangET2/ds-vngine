#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include <string.h>

struct MMAudioFile {
    const char name[256];
    int index;
};

const struct MMAudioFile *get_mmindex(const char *str, size_t len);

#endif