#include "text.h"
#include "display.h"

#include <nds.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#define FRAMESPERCHAR 2

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
}

void text_begin_dialogue(const char *speaker, const char *text) {
    text_clear();
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