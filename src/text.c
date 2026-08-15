#include "text.h"
#include "display.h"
#include "common.h"

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
    SUB_ANCHOR_Y = 4,

    CHOICE_ANCHOR_X = 5,
    CHOICE_2_1_ANCHOR_Y = 8,
    CHOICE_2_2_ANCHOR_Y = 15,
    CHOICE_3_1_ANCHOR_Y = 6,
    CHOICE_3_2_ANCHOR_Y = 11,
    CHOICE_3_3_ANCHOR_Y = 16,
    CHOICE_4_1_ANCHOR_Y = 4,
    CHOICE_4_2_ANCHOR_Y = 9,
    CHOICE_4_3_ANCHOR_Y = 14,
    CHOICE_4_4_ANCHOR_Y = 19,
};

typedef struct {
    char speaker[MAX_SPEAKER_LEN];
    char text[MAX_TEXT_LEN];

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
static PrintConsole debugWindow;
static PrintConsole choice1;
static PrintConsole choice2;
static PrintConsole choice3;
static PrintConsole choice4;

void text_init(void){
    consoleInit(&dialogueBox, MAIN_LAYER_TEXT, BgType_Text4bpp, BgSize_T_256x256,
                MAIN_TEXT_MAPBASE, MAIN_TEXT_TILEBASE, true, true);
    speakerBox = dialogueBox;
    
    consoleInit(&narrationBox, SUB_LAYER_TEXT, BgType_Text4bpp, BgSize_T_256x256,
                SUB_TEXT_MAPBASE, SUB_TEXT_TILEBASE, false, true);

    debugWindow = narrationBox;
    choice1 = narrationBox;
    choice2 = narrationBox;
    choice3 = narrationBox;
    choice4 = narrationBox;


    consoleSetWindow(&dialogueBox, MAIN_ANCHOR_X, MAIN_ANCHOR_Y, 26, 4);
    consoleSetWindow(&speakerBox, SPEAKER_ANCHOR_X, SPEAKER_ANCHOR_Y, 20, 1);
    consoleSetWindow(&narrationBox, SUB_ANCHOR_X, SUB_ANCHOR_Y, 26, 8);
    consoleSetWindow(&debugWindow, 2, 20, 30, 4);

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
    consoleSelect(&choice1);
    consoleClear();
    consoleSelect(&choice2);
    consoleClear();
    consoleSelect(&choice3);
    consoleClear();
    consoleSelect(&choice4);
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

void text_write_choices(char* choices[], int num_choices){
    //TODO: implement!
    //temporary, for testing purposes:
    switch(num_choices){
        case 2:{
            consoleSetWindow(&choice1, CHOICE_ANCHOR_X, CHOICE_2_1_ANCHOR_Y, 20, 2);
            consoleSelect(&choice1);
            printf("%s", choices[0]);
            consoleSetWindow(&choice2, CHOICE_ANCHOR_X, CHOICE_2_2_ANCHOR_Y, 20, 2);
            consoleSelect(&choice2);
            printf("%s", choices[1]);
            break;
        }
        case 3:{
            consoleSetWindow(&choice1, CHOICE_ANCHOR_X, CHOICE_3_1_ANCHOR_Y, 20, 2);
            consoleSelect(&choice1);
            printf("%s", choices[0]);
            consoleSetWindow(&choice2, CHOICE_ANCHOR_X, CHOICE_3_2_ANCHOR_Y, 20, 2);
            consoleSelect(&choice2);
            printf("%s", choices[1]);
            consoleSetWindow(&choice3, CHOICE_ANCHOR_X, CHOICE_3_3_ANCHOR_Y, 20, 2);
            consoleSelect(&choice3);
            printf("%s", choices[2]);
            break;

        }
        case 4:{
            consoleSetWindow(&choice1, CHOICE_ANCHOR_X, CHOICE_4_1_ANCHOR_Y, 20, 2);
            consoleSelect(&choice1);
            printf("%s", choices[0]);
            consoleSetWindow(&choice2, CHOICE_ANCHOR_X, CHOICE_4_2_ANCHOR_Y, 20, 2);
            consoleSelect(&choice2);
            printf("%s", choices[1]);
            consoleSetWindow(&choice3, CHOICE_ANCHOR_X, CHOICE_4_3_ANCHOR_Y, 20, 2);
            consoleSelect(&choice3);
            printf("%s", choices[2]);
            consoleSetWindow(&choice4, CHOICE_ANCHOR_X, CHOICE_4_4_ANCHOR_Y, 20, 2);
            consoleSelect(&choice4);
            printf("%s", choices[3]);
            break;
        }
    }
    return;
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

void text_debug_clear(){
    consoleSelect(&debugWindow);
    consoleClear();
}

void text_debug_set(const char *text){
    text_debug_clear();
    consoleSelect(&debugWindow);
    printf("%s\n", text);
}
