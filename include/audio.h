#ifndef AUDIO_H
#define AUDIO_H

#include <string.h>
#include <stdbool.h>

void audio_init(void);
void audio_reset(void);
void audio_shutdown(void);

int audio_start_bgm(const char *bgm_name);
void audio_stop_bgm(void);
int audio_start_sfx(const char *sfx_name);
void audio_stop_sfx(void);

bool audio_is_playing(void);

#endif