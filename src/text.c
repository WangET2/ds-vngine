#include "text.h"
#include "display.h"
#include "common.h"

#include <nds.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define FRAMESPERCHAR 2

//main screen textboxes
#define MAIN_ANCHOR_X 2
#define MAIN_ANCHOR_Y 20
#define SPEAKER_ANCHOR_X 2
#define SPEAKER_ANCHOR_Y 18

#define SUB_ANCHOR_X 3
#define SUB_ANCHOR_Y 4

#define CHOICE_ANCHOR_X 5

static const int TWO_CHOICE_ANCHOR_Y[2] = {8, 15};
static const int THREE_CHOICE_ANCHOR_Y[3] = {6, 11, 16};
static const int FOUR_CHOICE_ANCHOR_Y[4] = {4, 9, 14, 19};

/* typedef struct {
    char speaker[MAX_SPEAKER_LEN];
    char text[MAX_TEXT_LEN];

    int visible_chars;
    int total_chars;

    int frames_per_char;
    int frame_counter;

    bool finished;
    bool narration;
} TextState; */

typedef enum {
    SLOT_DIALOGUE,
    SLOT_SPEAKER,
    SLOT_NARRATION,
    SLOT_CHOICE_ONE,
    SLOT_CHOICE_TWO,
    SLOT_CHOICE_THREE,
    SLOT_CHOICE_FOUR,
    NUM_TEXT_SLOTS
} TextSlot_t;

typedef enum {
    TEXT_MODE_NORMAL,
    TEXT_MODE_WRITING
} TextMode_t;

typedef struct {
    char buf[MAX_TEXT_LEN];

    int visible_chars;
    int total_chars;

    int frame_counter;
    bool finished;
} TextSlot;

typdef struct {
    const TextSlot_t slot,
    *PrintConsole console
} TextSlotInfo_t;

static PrintConsole s_dialogueBox;
static PrintConsole s_speakerBox;
static PrintConsole s_narrationBox;
static PrintConsole s_choice1;
static PrintConsole s_choice2;
static PrintConsole s_choice3;
static PrintConsole s_choice4;
static PrintConsole s_debugWindow;

TextSlotInfo_t s_text_slot_info[NUM_TEXT_SLOTS] = {
    {SLOT_DIALOGUE, &s_dialogueBox},
    {SLOT_SPEAKER, &s_speakerBox},
    {SLOT_NARRATION, &s_narrationBox},
    {SLOT_CHOICE_ONE, &s_choice1},
    {SLOT_CHOICE_TWO, &s_choice2},
    {SLOT_CHOICE_THREE, &s_choice3},
    {SLOT_CHOICE_FOUR, &s_choice4}
};


void text_init(void){
    consoleInit(&s_dialogueBox, MAIN_LAYER_TEXT, BgType_Text4bpp, BgSize_T_256x256,
                MAIN_TEXT_MAPBASE, MAIN_TEXT_TILEBASE, true, true);
    speakerBox = dialogueBox;
    
    consoleInit(&s_narrationBox, SUB_LAYER_TEXT, BgType_Text4bpp, BgSize_T_256x256,
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

    s_text.frames_per_char = FRAMESPERCHAR;
}

void text_reset(void){
    strcpy(s_text.speaker, "");
    strcpy(s_text.text, "");
    s_text.visible_chars = 0;
    s_text.total_chars = 0;
    s_text.frame_counter = 0;
    s_text.finished = true;
    for(int i = 0; i < NUM_TEXT_SLOTS; ++i){
        consoleSelect(s_text_slot_info[i].console);
        consoleClear();
    }
}

void text_begin_dialogue(const char *speaker, const char *text) {
    text_clear();
    strcpy(s_text.speaker, speaker);
    consoleSelect(&speakerBox);
    printf("%s", speaker);
    strcpy(s_text.text, text);
    s_text.total_chars = strlen(text);
    s_text.visible_chars = 1;
    s_text.frame_counter = 0;
    s_text.finished = false;
    s_text.narration = false;
}

void text_begin_narration(const char *text) {
    text_clear();
    strcpy(s_text.speaker, "");
    strcpy(s_text.text, text);
    s_text.total_chars = strlen(text);
    s_text.visible_chars = 1;
    s_text.frame_counter = 0;
    s_text.finished = false;
    s_text.narration = true;
}

void text_write_choices(char* choices[], int num_choices){
    //TODO: implement!
    //temporary, for testing purposes:
    switch(num_choices){
        case 2:{
            consoleSetWindow(&choice1, CHOICE_ANCHOR_X, TWO_CHOICE_ANCHOR_Y[0], 20, 2);
            consoleSelect(&choice1);
            printf("%s", choices[0]);
            consoleSetWindow(&choice2, CHOICE_ANCHOR_X, TWO_CHOICE_ANCHOR_Y[1], 20, 2);
            consoleSelect(&choice2);
            printf("%s", choices[1]);
            break;
        }
        case 3:{
            consoleSetWindow(&choice1, CHOICE_ANCHOR_X, THREE_CHOICE_ANCHOR_Y[0], 20, 2);
            consoleSelect(&choice1);
            printf("%s", choices[0]);
            consoleSetWindow(&choice2, CHOICE_ANCHOR_X, THREE_CHOICE_ANCHOR_Y[1], 20, 2);
            consoleSelect(&choice2);
            printf("%s", choices[1]);
            consoleSetWindow(&choice3, CHOICE_ANCHOR_X, THREE_CHOICE_ANCHOR_Y[2], 20, 2);
            consoleSelect(&choice3);
            printf("%s", choices[2]);
            break;

        }
        case 4:{
            consoleSetWindow(&choice1, CHOICE_ANCHOR_X, FOUR_CHOICE_ANCHOR_Y[0], 20, 2);
            consoleSelect(&choice1);
            printf("%s", choices[0]);
            consoleSetWindow(&choice2, CHOICE_ANCHOR_X, FOUR_CHOICE_ANCHOR_Y[1], 20, 2);
            consoleSelect(&choice2);
            printf("%s", choices[1]);
            consoleSetWindow(&choice3, CHOICE_ANCHOR_X, FOUR_CHOICE_ANCHOR_Y[2], 20, 2);
            consoleSelect(&choice3);
            printf("%s", choices[2]);
            consoleSetWindow(&choice4, CHOICE_ANCHOR_X, FOUR_CHOICE_ANCHOR_Y[3], 20, 2);
            consoleSelect(&choice4);
            printf("%s", choices[3]);
            break;
        }
    }
    return;
}

void text_update(void) {
    if(s_text.visible_chars > s_text.total_chars){
        s_text.finished = true;
        return;
    }
    if(s_text.frame_counter == 0){
        PrintConsole textbox = s_text.narration ? narrationBox : dialogueBox;
        consoleSelect(&textbox);
        printf("%.*s", s_text.visible_chars, s_text.text);
        s_text.visible_chars++;
    }
    s_text.frame_counter = (s_text.frame_counter + 1) % s_text.frames_per_char;
}

void text_finish_immediately(void){
    s_text.visible_chars = s_text.total_chars;
    text_update();
}

bool text_is_finished(void){
    return s_text.finished;
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
