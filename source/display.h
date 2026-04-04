#ifndef DISPLAY_H
#define DISPLAY_H

#include <nds.h>

typedef enum{
    MAIN_DISPLAY_BG_TEXT = 0,
    MAIN_DISPLAY_BG_TEXTBOX = 1,
    MAIN_DISPLAY_BG_SCENE = 2,
    MAIN_DISPLAY_BG_UNUSED = 3
} MainDisplayBgLayer;

typedef enum{
    SUB_DISPLAY_BG_TEXT = 0,
    SUB_DISPLAY_BG_TEXTBOX = 1,
    SUB_DISPLAY_BG_UI = 2,
    SUB_DISPLAY_BG_SCENE = 3
} SubDisplayBgLayer;

#define MAIN_TEXT_MAP_BASE 16
#define MAIN_TEXT_TILE_BASE 0

#define MAIN_TEXTBOX_MAP_BASE 17
#define MAIN_TEXTBOX_TILE_BASE 7

#define MAIN_SCENE_MAP_BASE 3
//No tilebase; scene bg is an 8-bit bitmap

void display_init(void);

int display_get_main_bg(MainDisplayBgLayer layer);
int display_get_sub_bg(SubDisplayBgLayer layer);

#endif