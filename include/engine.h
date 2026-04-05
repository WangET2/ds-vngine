#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <nds.h>

typedef enum{
    ENGINE_RESULT_OK = 0,
    ENGINE_RESULT_FINISHED,
    ENGINE_RESULT_ERROR
} EngineResult;

void engine_init(void);

void engine_reset(void);

int engine_load_scene(const char *scene_name);

EngineResult engine_update(void);

void engine_handle_input(u16 keys_down);

//bool engine_is_blocked(void);

#endif