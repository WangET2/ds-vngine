#include "text.h"
#include "display.h"

#include <nds.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#define FRAMESPERCHAR 2

void load_textbox_gfx(bool main);

enum {
    MAIN_ANCHOR_X = 2,
    MAIN_ANCHOR_Y = 20,
    SPEAKER_ANCHOR_X = 2,
    SPEAKER_ANCHOR_Y = 18,
    SUB_ANCHOR_X = 3,
    SUB_ANCHOR_Y = 4
};

typedef struct {
    char speaker[32];
    char text[512];

    int visible_chars;
    int total_chars;

    int frames_per_char;
    int frame_counter;

    bool finished;
    bool narration;
} TextState;

static TextState g_text;

static PrintConsole dialogueBox;
static PrintConsole speakerBox;
static PrintConsole narrationBox;

void text_init(void){
    consoleInit(&dialogueBox, MAIN_LAYER_TEXT, BgType_Text4bpp, BgSize_T_256x256,
                MAIN_TEXT_MAPBASE, MAIN_TEXT_TILEBASE, true, true);
    speakerBox = dialogueBox;
    
    consoleInit(&narrationBox, SUB_LAYER_TEXT, BgType_Text4bpp, BgSize_T_256x256,
                SUB_TEXT_MAPBASE, SUB_TEXT_TILEBASE, false, true);

    consoleSetWindow(&dialogueBox, MAIN_ANCHOR_X, MAIN_ANCHOR_Y, 26, 4);
    consoleSetWindow(&speakerBox, SPEAKER_ANCHOR_X, SPEAKER_ANCHOR_Y, 20, 1);
    consoleSetWindow(&narrationBox, SUB_ANCHOR_X, SUB_ANCHOR_Y, 26, 8);
    g_text.frames_per_char = FRAMESPERCHAR;

    load_textbox_gfx(true);
    load_textbox_gfx(false);
}

void text_clear(void){
    strcpy(g_text.speaker, "");
    strcpy(g_text.text, "");
    g_text.visible_chars = 0;
    g_text.total_chars = 0;
    g_text.frame_counter = 0;
    g_text.finished = true;
    consoleSelect(&dialogueBox);
    consoleClear();
    consoleSelect(&speakerBox);
    consoleClear();
    consoleSelect(&narrationBox);
    consoleClear();
    int mainTextBg = display_get_main_bg(MAIN_LAYER_TEXTBOX);
    bgHide(mainTextBg);
    int subTextBg = display_get_sub_bg(SUB_LAYER_TEXTBOX);
    bgHide(subTextBg);
}

void text_begin_dialogue(const char *speaker, const char *text) {
    text_clear();
    int mainTextBg = display_get_main_bg(MAIN_LAYER_TEXTBOX);
    bgShow(mainTextBg);
    strcpy(g_text.speaker, speaker);
    consoleSelect(&speakerBox);
    printf("%s", speaker);
    strcpy(g_text.text, text);
    g_text.total_chars = strlen(text);
    g_text.visible_chars = 1;
    g_text.frame_counter = 0;
    g_text.finished = false;
    g_text.narration = false;
}

void text_begin_narration(const char *text) {
    text_clear();
    int subTextBg = display_get_sub_bg(SUB_LAYER_TEXTBOX);
    bgShow(subTextBg);
    strcpy(g_text.speaker, "");
    strcpy(g_text.text, text);
    g_text.total_chars = strlen(text);
    g_text.visible_chars = 1;
    g_text.frame_counter = 0;
    g_text.finished = false;
    g_text.narration = true;
}

void text_update(void) {
    if(g_text.visible_chars > g_text.total_chars){
        g_text.finished = true;
        return;
    }
    if(g_text.frame_counter == 0){
        PrintConsole textbox = g_text.narration ? narrationBox : dialogueBox;
        consoleSelect(&textbox);
        printf("%.*s", g_text.visible_chars, g_text.text);
        g_text.visible_chars++;
    }
    g_text.frame_counter = (g_text.frame_counter + 1) % g_text.frames_per_char;
}

void text_finish_immediately(void){
    g_text.visible_chars = g_text.total_chars;
    text_update();
}

bool text_is_finished(void){
    return g_text.finished;
}

void load_textbox_gfx(bool main){
    int textboxBg = main ? display_get_main_bg(MAIN_LAYER_TEXTBOX) : display_get_sub_bg(SUB_LAYER_TEXTBOX);
    void *bgData = NULL;
    size_t bgSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    void *mapData = NULL;
    size_t mapSize = 0;
    if(main)
        grfLoadPath("nitro:/grit/bg/maintextbox_png.grf", NULL, &bgData, &bgSize, &mapData, &mapSize, &palData, &palSize);
    else
        grfLoadPath("nitro:/grit/bg/subtextbox_png.grf", NULL, &bgData, &bgSize, &mapData, &mapSize, &palData, &palSize);
    memcpy(bgGetGfxPtr(textboxBg), bgData, bgSize);
    memcpy(bgGetMapPtr(textboxBg), mapData, mapSize);
    if(main){
        vramSetBankE(VRAM_E_LCD);
        memcpy(VRAM_E_EXT_PALETTE[MAIN_LAYER_TEXTBOX][0], palData, palSize);
        vramSetBankE(VRAM_E_BG_EXT_PALETTE);
        bgExtPaletteEnable();
        REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG1 | BLEND_DST_BG2;
        REG_BLDALPHA = 12 | (4 << 8);
    } else{
        vramSetBankH(VRAM_H_LCD);
        memcpy(VRAM_H_EXT_PALETTE[SUB_LAYER_TEXTBOX][0], palData, palSize);
        vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
        bgExtPaletteEnableSub();
        REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG1 | BLEND_DST_BG3;
        REG_BLDALPHA_SUB = 8 | (4 << 8);
    }
    free(bgData);
    free(mapData);
    free(palData);
    bgSetPriority(textboxBg, 0);
}