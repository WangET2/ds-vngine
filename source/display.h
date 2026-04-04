#ifndef DISPLAY_H
#define DISPLAY_H

#include <nds.h>

typedef enum{
    MAIN_LAYER_TEXT = 0,
    MAIN_LAYER_TEXTBOX = 1,
    MAIN_LAYER_SCENE = 2,
    MAIN_LAYER_UNUSED = 3
} MainDisplayBgLayer;

typedef enum{
    SUB_LAYER_TEXT = 0,
    SUB_LAYER_TEXTBOX = 1,
    SUB_LAYER_UI = 2,
    SUB_LAYER_SCENE = 3
} SubDisplayBgLayer;

typedef enum{
    MAIN_TEXT_TILEBASE = 1,
    MAIN_TEXTBOX_TILEBASE
}MainTileBase;

typedef enum{
    MAIN_TEXT_MAPBASE = 0,
    MAIN_TEXTBOX_MAPBASE,
    MAIN_SCENE_BITMAPBASE = 3
}MainMapBase;

//if anything blows up this is probably why... https://mtheall.com/vram.html#
typedef enum{
    SUB_TEXT_TILEBASE = 1,
    SUB_TEXTBOX_TILEBASE,
    SUB_UI_TILEBASE
}SubTileBase;

typedef enum{
    SUB_TEXT_MAPBASE = 0,
    SUB_TEXTBOX_MAPBASE,
    SUB_UI_MAPBASE,
    SUB_SCENE_BITMAPBASE = 4
}SubMapBase;

//temp please don't break please don't break
#define SUB_TEXT_MAP_BASE 0
#define SUB_TEXT_TILE_BASE 1

#define SUB_TEXTBOX_MAP_BASE 1
#define SUB_TEXTBOX_TILE_BASE 2

#define SUB_UI_MAP_BASE 2


void display_init(void);

int display_get_main_bg(MainDisplayBgLayer layer);
int display_get_sub_bg(SubDisplayBgLayer layer);

#endif