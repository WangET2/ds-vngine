#include <nds.h>
#include "display.h"

static int main_textbox_bg, main_scene_bg;
static int sub_textbox_bg, sub_ui_bg, sub_scene_bg;

void display_init(void){
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    
    
    main_scene_bg = bgInitHidden(MAIN_LAYER_SCENE, BgType_Bmp8, BgSize_B8_256x256,
                                MAIN_SCENE_BITMAPBASE, 0);
    main_textbox_bg = bgInitHidden(MAIN_LAYER_TEXTBOX, BgType_Text8bpp, BgSize_T_256x256,
                                    MAIN_TEXTBOX_MAPBASE, MAIN_TEXTBOX_TILEBASE);
    
    //TODO: bginitsubhidden yada yada

    sub_textbox_bg = bgInitHiddenSub(SUB_LAYER_TEXTBOX, BgType_Text8bpp, BgSize_T_256x256,
                                    SUB_TEXTBOX_MAPBASE, SUB_TEXTBOX_TILEBASE);
}

int display_get_main_bg(MainDisplayBgLayer layer){
    switch(layer){
        case MAIN_LAYER_TEXTBOX:
            return main_textbox_bg;
            break;
        case MAIN_LAYER_SCENE:
            return main_scene_bg;
            break;
        default:
            return -1;
    }
}