#ifndef FLAGS_H
#define FLAGS_H

#include <stdbool.h>

void flags_init(void);
void flags_reset(void);
void flags_destroy(void);

bool flags_set(const char* key);
bool flags_has(const char* key);
bool flags_unset(const char* key);

#endif