#ifndef DISPLAY_H
#define DISPLAY_H

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

void display_init(void);
void display_shutdown(void);

int display_get_main_bg(MainDisplayBgLayer layer);
int display_get_sub_bg(SubDisplayBgLayer layer);

#endif