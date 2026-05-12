#include <string.h>
#include <stdbool.h>
#include <maxmod9.h>
#include <stdio.h>
#include "audio.h"
#include "soundbank.h"

static bool g_playing;
static struct MMAudioFile *g_bgm, *g_sfx;


void audio_init(void){
    g_playing = false;
    mmInitDefault("maxmod/soundbank.bin");
    mmSelectMode(MM_MODE_C);
}

int audio_start_bgm(const char *bgm_name){
    if(g_bgm) audio_stop_bgm();
    char buf[260];
    int ret = snprintf(buf, sizeof(buf), "mod_%s", bgm_name);
    if(ret >= sizeof(buf)) return -1;
    g_bgm = get_mmindex(&buf[0], strlen(buf));
    if(!g_bgm) return -1;
    mmLoad(g_bgm->index);
    mmStart(g_bgm->index, MM_PLAY_LOOP);
    return 0;
}

int audio_start_sfx(const char *sfx_name){
    if(g_sfx) audio_stop_sfx();
    char buf[260];
    int ret = snprintf(buf, sizeof(buf), "sfx_%s", sfx_name);
    if(ret >= sizeof(buf)) return -1;
    g_sfx = get_mmindex(&buf[0], strlen(buf));
    if(!g_sfx) return -1;
    mmLoadEffect(g_sfx->index);
    mmEffect(g_sfx->index);
    return 0;
}

void audio_stop_bgm(void){
    if(!g_bgm) return;
    mmUnload(g_bgm->index);
    g_bgm = NULL;
}

void audio_stop_sfx(void){
    if(!g_sfx) return;
    mmEffectCancelAll();
    mmUnloadEffect(g_sfx->index);
    g_sfx = NULL;
}