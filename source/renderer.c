#include "renderer.h"
#include "display.h"

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum {
    LEFT_ANCHOR_X = -16,
    RIGHT_ANCHOR_X = 144,
    CENTER_ANCHOR_X = 64,
    ANCHOR_Y = 74
};

enum {
    SPR_LEFT_BASE = 0,

    SPR_RIGHT_BASE = 4,

    SPR_CENTER_BASE = 8,

    SPR_DIALOGUE_PORTRAIT = 12,
    NUM_BITMAP_SPRITES
};

typedef struct {
    int baseIndex;
    int anchorX;
} PortraitSlot;

static const PortraitSlot LEFT_SLOT = {
    .baseIndex = SPR_LEFT_BASE,
    .anchorX = LEFT_ANCHOR_X
};

static const PortraitSlot RIGHT_SLOT = {
    .baseIndex = SPR_RIGHT_BASE,
    .anchorX = RIGHT_ANCHOR_X
};

static const PortraitSlot CENTER_SLOT = {
    .baseIndex = SPR_CENTER_BASE,
    .anchorX = CENTER_ANCHOR_X
};

static void *sprite_mem[NUM_BITMAP_SPRITES];
static const char* const spriteComponent[4] = {"bleft_png.grf", "bright_png.grf", "tleft_png.grf", "tright_png.grf"};
//static int bgMain;

static int load_bmp_sprite(const char *path, void *gfxAlloc);
static int renderer_show_portrait(const PortraitSlot *slot, const char *name, const char *expression);
static void renderer_hide_portrait(const PortraitSlot *slot);
static int get_portrait_offset(const char *name, const char *expression);
static inline int sprite_x(int anchor, int i);
static inline int sprite_y(int i);
int renderer_set_sub_default(void);

void renderer_init(void){
    /*videoSetMode(MODE_3_2D);
    videoSetModeSub(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);*/
    oamInit(&oamMain, SpriteMapping_Bmp_1D_128, false);
    for(int i = 0; i < NUM_BITMAP_SPRITES; ++i) sprite_mem[i] = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_Bmp);
    /*bgMain = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 3, 0);
    bgShow(bgMain);*/
    int bgMain = display_get_main_bg(MAIN_LAYER_SCENE);
    bgSetPriority(bgMain, 2);
    renderer_set_sub_default();
}

void renderer_update(void)
{
    oamUpdate(&oamMain);
}

void renderer_shutdown(void) {
    for(int i = 0; i < NUM_BITMAP_SPRITES; ++i) oamFreeGfx(&oamMain, sprite_mem[i]);
}

void renderer_reset(void){
    renderer_hide_center();
    renderer_hide_left();
    renderer_hide_right();
    int bgMain = display_get_main_bg(MAIN_LAYER_SCENE);
    bgHide(bgMain);
}

int renderer_show_left(const char *name, const char *expression) {
    renderer_hide_center();
    return renderer_show_portrait(&LEFT_SLOT, name, expression);
}

int renderer_show_right(const char *name, const char *expression) {
    renderer_hide_center();
    return renderer_show_portrait(&RIGHT_SLOT, name, expression);
}

int renderer_show_center(const char *name, const char *expression) {
    renderer_hide_left();
    renderer_hide_right();
    return renderer_show_portrait(&CENTER_SLOT, name, expression);
}

void renderer_hide_left(void){
    renderer_hide_portrait(&LEFT_SLOT);
}

void renderer_hide_right(void){
    renderer_hide_portrait(&RIGHT_SLOT);
}

void renderer_hide_center(void){
    renderer_hide_portrait(&CENTER_SLOT);
}

/*int renderer_set_background(const char *bg_name) {
    int bgMain = display_get_main_bg(MAIN_LAYER_SCENE);
    void *bgData = NULL;
    size_t bgSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    char path[100];
    snprintf(path, sizeof(path), "nitro:/grit/bg/%s_png.grf", bg_name);
    GRFError err = grfLoadPath(path, NULL, &bgData, &bgSize,
                               NULL, NULL, &palData, &palSize); 
    if(err != GRF_NO_ERROR) return -1;
    memcpy(bgGetGfxPtr(bgMain), bgData, bgSize);
    memcpy(BG_PALETTE, palData, palSize);
    free(bgData);
    free(palData);
    bgShow(bgMain);
    return 0;
}*/

int renderer_set_background(const char *bg_name, bool mainScreen) {
    int bg = mainScreen ? display_get_main_bg(MAIN_LAYER_SCENE) : display_get_sub_bg(SUB_LAYER_SCENE);
    void *bgData = NULL;
    size_t bgSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    char path[100];
    snprintf(path, sizeof(path), "nitro:/grit/bg/%s_png.grf", bg_name);
    GRFError err = grfLoadPath(path, NULL, &bgData, &bgSize,
                               NULL, NULL, &palData, &palSize); 
    if(err != GRF_NO_ERROR) return -1;
    memcpy(bgGetGfxPtr(bg), bgData, bgSize);
    if(mainScreen)
        memcpy(BG_PALETTE, palData, palSize);
    else
        memcpy(BG_PALETTE_SUB, palData, palSize);
    free(bgData);
    free(palData);
    bgShow(bg);
    return 0;
}

int renderer_set_sub_default(void){
    /*int bgSub = display_get_sub_bg(SUB_LAYER_SCENE);
    void *bgData = NULL;
    size_t bgSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    GRFError err = grfLoadPath("nitro:/grit/bg/subbg_png.grf", NULL, &bgData, &bgSize,
                               NULL, NULL, &palData, &palSize); 
    if(err != GRF_NO_ERROR) return -1;
    memcpy(bgGetGfxPtr(bgSub), bgData, bgSize);
    memcpy(BG_PALETTE_SUB, palData, palSize);
    free(bgData);
    free(palData);
    bgShow(bgSub);
    return 0;*/
    return renderer_set_background("subbg", false);
}

static int load_bmp_sprite(const char *path, void *gfxAlloc) {
    if(gfxAlloc == NULL) return -1;
    
    void *gfxGrf = NULL;
    size_t gfxSize = 0;

    GRFError err = grfLoadPath(path, NULL, &gfxGrf, &gfxSize, NULL, NULL, NULL, NULL);

    if(err != GRF_NO_ERROR) return -2;
    
    memcpy(gfxAlloc, gfxGrf, gfxSize);

    free(gfxGrf);
    return 0;
}

static int renderer_show_portrait(const PortraitSlot *slot, const char *name, const char *expression) {
    int additional_offset = get_portrait_offset(name, expression);
    for(int i = 0; i < 4; ++i){
        char path[100];
        snprintf(path, sizeof(path), "nitro:/grit/sprites/%s/%s/%s", name, expression, spriteComponent[i]);
        int ret = load_bmp_sprite(path, sprite_mem[slot->baseIndex + i]);
        if (ret!= 0) return ret;
        oamSet(&oamMain, slot->baseIndex + i, sprite_x(slot->anchorX, i) + additional_offset, 
        sprite_y(i), 0, 15,SpriteSize_64x64, SpriteColorFormat_Bmp, sprite_mem[slot->baseIndex + i],
        -1, false, false, false, false, false);
    }
    return 0;
}

static inline int sprite_x(int anchor, int i){
    return anchor + 64 * (i % 2);
    //order BLEFT, BRIGHT, TLEFT, TRIGHT. Only right sprite components need + 64
}

static inline int sprite_y(int i){
    return ANCHOR_Y - 64 * (i > 1);
    //order BLEFT, BRIGHT, TLEFT, TRIGHT. Only top sprite components need - 64
}

static void renderer_hide_portrait(const PortraitSlot *slot){
    for(int i = 0; i < 4; ++i)
        oamSet(&oamMain, slot->baseIndex + i, sprite_x(slot->anchorX, i), sprite_y(i), 0, 15,SpriteSize_64x64,
                SpriteColorFormat_Bmp, sprite_mem[slot->baseIndex + i], -1, false, true, false, false, false);
}

static int get_portrait_offset(const char *name, const char *expression){
    char path[100];
    snprintf(path, sizeof(path), "nitro:/offsets/%s/%s/%s", name, expression, "offset.txt");
    FILE *fptr;
    fptr = fopen(path, "r");
    if(fptr == NULL) return 0;
    char raw_offset[100];
    fgets(raw_offset, sizeof(raw_offset), fptr);
    fclose(fptr);
    return atoi(raw_offset);
}

