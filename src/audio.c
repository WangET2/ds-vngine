#include <string.h>
#include <maxmod9.h>
#include <stdio.h>
#include "audio.h"
#include "soundbank.h"
#include "common.h"

static struct MMAudioFile *g_bgm, *g_sfx;
static AudioState g_audio_state;

void audio_init(void){
    mmInitDefault("maxmod/soundbank.bin");
    mmSelectMode(MM_MODE_C);
    memset(&g_audio_state, 0, sizeof(g_audio_state));
}

void audio_update(void){
    if(g_bgm && !mmActive()) audio_stop_bgm();
}

int audio_start_bgm(const char *bgm_name){
    if(g_bgm) audio_stop_bgm();
    char buf[MAX_AUDIO_TITLE_LEN];
    int ret = snprintf(buf, sizeof(buf), "mod_%s", bgm_name);
    if(ret >= sizeof(buf)) return -1;
    g_bgm = get_mmindex(&buf[0], strlen(buf));
    if(!g_bgm) return -1;
    mmLoad(g_bgm->index);
    mmStart(g_bgm->index, MM_PLAY_LOOP);
    int buf_ret = snprintf(&g_audio_state.current_bgm[0], MAX_AUDIO_TITLE_LEN, "%s", bgm_name);
    if(buf_ret < 0 || buf_ret >= MAX_AUDIO_TITLE_LEN) return -1;
    return 0;
}

int audio_start_sfx(const char *sfx_name){
    if(g_sfx) audio_stop_sfx();
    char buf[MAX_AUDIO_TITLE_LEN];
    int ret = snprintf(buf, sizeof(buf), "sfx_%s", sfx_name);
    if(ret >= sizeof(buf)) return -1;
    g_sfx = get_mmindex(&buf[0], strlen(buf));
    if(!g_sfx) return -1;
    mmLoadEffect(g_sfx->index);
    mmEffect(g_sfx->index);
    //int buf_ret = snprintf(&g_audio_state.current_sfx[0], MAX_AUDIO_TITLE_LEN, "%s", sfx_name);
    //if(buf_ret < 0 || buf_ret >= MAX_AUDIO_TITLE_LEN) return -1;
    return 0;
}

void audio_stop_bgm(void){
    if(!g_bgm) return;
    mmUnload(g_bgm->index);
    g_bgm = NULL;
    snprintf(&g_audio_state.current_bgm[0], MAX_AUDIO_TITLE_LEN, "%s", "");
}

void audio_stop_sfx(void){
    if(!g_sfx) return;
    mmEffectCancelAll();
    mmUnloadEffect(g_sfx->index);
    g_sfx = NULL;
    //snprintf(&g_audio_state.current_sfx[0], MAX_AUDIO_TITLE_LEN, "%s", "");
}

void audio_reset(void){
    audio_stop_bgm();
    audio_stop_sfx();
    memset(&g_audio_state, 0, sizeof(g_audio_state));
}

void audio_get_state(AudioState* out){
    memcpy(out, &g_audio_state, sizeof(*out));
}