#ifndef AUDIO_H
#define AUDIO_H

#include <string.h>
#include <stdbool.h>
#include "common.h"

typedef struct{
    char current_bgm[MAX_AUDIO_TITLE_LEN];
    //char current_sfx[MAX_AUDIO_TITLE_LEN];
} AudioState;

void audio_init(void);
void audio_reset(void);
void audio_shutdown(void);
void audio_update(void);

int audio_start_bgm(const char *bgm_name);
void audio_stop_bgm(void);
int audio_start_sfx(const char *sfx_name);
void audio_stop_sfx(void);

void audio_get_state(AudioState *out);

#endif