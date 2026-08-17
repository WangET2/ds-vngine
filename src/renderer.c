#include "renderer.h"
#include "display.h"
#include "choice.h"
#include "common.h"

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*TODO: Review this entire module...
    1) Textbox conventions - currently we essentially ASSERT 
       that a textbox image is present with a specific name;
       We likely want textboxes to be user configurable
    2) Take constants out of unnamed enums - why was this a thing...
    3) SlotState updates - why were we passing by pointer?
    4) Inside hide_portrait_.* - update SlotState
    5) Consider SlotState struct; currently handles oam concerns,
       rendering concerns (pixel offset), do we really want to add
       statefulness to this struct?
    6) Reorder to avoid forward declaration - ugly
*/

//Pixel offsets
#define LEFT_ANCHOR_X -16
#define RIGHT_ANCHOR_X 144
#define CENTER_ANCHOR_X 64
#define ANCHOR_Y 74

//OAM offsets
//main screen
#define SPR_LEFT_BASE 0
#define SPR_RIGHT_BASE 4
#define SPR_CENTER_BASE 8
#define SPR_DIALOGUE_PORTRAIT 12
#define NUM_MAIN_SPRITES 13
//sub screen
#define SPR_CHOICE_BASE 0
#define NUM_SUB_SPRITES 3

#define SPR_NUM_COMPONENTS 4

typedef enum {
    SPRITE_POSITION_LEFT,
    SPRITE_POSITION_RIGHT,
    SPRITE_POSITION_CENTER
} SpritePosition;

typedef struct {
    const SpritePosition pos;
    const int oam_base;
    const int pixel_anchor_x;
    const int pixel_anchor_y;
} SpriteInfo_t;

static void *sprite_mem_main[NUM_MAIN_SPRITES];
static void *sprite_mem_sub[NUM_SUB_SPRITES];
static const char* const spriteComponent[4] = {"bleft_png.grf", "bright_png.grf", "tleft_png.grf", "tright_png.grf"};

static char current_main_textbox[32] = "";
static char current_sub_textbox[32] = "";

static RendererState g_renderer_state;

static const SpriteInfo_t sprite_info[] = {
    {SPRITE_POSITION_LEFT, SPR_LEFT_BASE, LEFT_ANCHOR_X, ANCHOR_Y},
    {SPRITE_POSITION_RIGHT, SPR_RIGHT_BASE, RIGHT_ANCHOR_X, ANCHOR_Y},
    {SPRITE_POSITION_CENTER, SPR_CENTER_BASE, CENTER_ANCHOR_X, ANCHOR_Y},
};

// Review this function - low level implementation details should
// probably be handled by display.c
static int renderer_set_sub_backdrop(void){
    int subBg = display_get_sub_bg(SUB_LAYER_UI);
    void *bgData = NULL;
    size_t bgSize = 0;
    void *mapData = NULL;
    size_t mapSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    GRFError err = grfLoadPath("nitro:/grit/ui/subbg_png.grf", NULL, &bgData, &bgSize,
                               &mapData, &mapSize, &palData, &palSize); 
    if(err != GRF_NO_ERROR) return -1;
    memcpy(bgGetGfxPtr(subBg), bgData, bgSize);
    memcpy(bgGetMapPtr(subBg), mapData, mapSize);
    vramSetBankH(VRAM_H_LCD);
    memcpy(VRAM_H_EXT_PALETTE[SUB_LAYER_UI][1], palData, palSize);
    vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
    bgExtPaletteEnableSub();
    free(bgData);
    free(mapData);
    free(palData);
    bgShow(subBg);
    return 0;
}

static void load_normal_sprite(const char *path, void *gfxAlloc){ 
    void *gfxGrf = NULL;
    size_t gfxSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    grfLoadPath(path, NULL, &gfxGrf, &gfxSize,
                NULL, NULL, &palData, &palSize);
    memcpy(gfxAlloc, gfxGrf, gfxSize);
    memcpy(SPRITE_PALETTE_SUB, palData, palSize);
    free(gfxGrf);
    free(palData);
}

void renderer_init(void){
    oamInit(&oamMain, SpriteMapping_Bmp_1D_256, false);
    oamInit(&oamSub, SpriteMapping_1D_256, false);
    for(int i = 0; i < NUM_MAIN_SPRITES; ++i) sprite_mem_main[i] = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_Bmp);
    char *element_paths[3] = {"nitro:/grit/ui/choiceoverlay_left_png.grf", "nitro:/grit/ui/choiceoverlay_center_png.grf",
                            "nitro:/grit/ui/choiceoverlay_right_png.grf"};
    for(int j = 0; j < 3; ++j){
        sprite_mem_sub[j] = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_256Color);
        load_normal_sprite(element_paths[j], sprite_mem_sub[j]);
    }
    int bgMain = display_get_main_bg(MAIN_LAYER_SCENE);
    bgSetPriority(bgMain, 2);
    renderer_set_sub_backdrop();
    memset(&g_renderer_state, 0, sizeof(g_renderer_state));
}

void renderer_update(void)
{
    oamUpdate(&oamMain);
    oamUpdate(&oamSub);
}

void renderer_shutdown(void) {
    for(int i = 0; i < NUM_MAIN_SPRITES; ++i) oamFreeGfx(&oamMain, sprite_mem_main[i]);
    for(int j = 0; j < NUM_SUB_SPRITES; ++j) oamFreeGfx(&oamSub, sprite_mem_sub[j]);
}

void renderer_reset(void){
    renderer_hide_center();
    renderer_hide_left();
    renderer_hide_right();
    setBackdropColor(RGB15(0,0,0));
    setBackdropColorSub(RGB15(0,0,0));
    renderer_hide_background(true);
    renderer_hide_background(false);
    memset(&g_renderer_state, 0, sizeof(g_renderer_state));
}

static int update_slot_state(SlotState *dest, const char *name, const char *expression){
    int ret = snprintf(&dest->sprite_name[0], MAX_SPRITE_FILENAME_LEN, "%s", name);
    if(ret < 0 || ret >= MAX_SPRITE_FILENAME_LEN) return -1;
    ret = snprintf(&dest->sprite_expression[0], MAX_SPRITE_FILENAME_LEN, "%s", expression);
    if(ret < 0 || ret >= MAX_SPRITE_FILENAME_LEN) return -1;
    return 0;
}

static int get_portrait_offset(const char *name, const char *expression){
    char path[100];
    int n = snprintf(path, sizeof(path), "nitro:/offsets/%s/%s/%s", name, expression, "offset.txt");
    if(n >= sizeof(path)) return -1;
    FILE *fptr;
    fptr = fopen(path, "r");
    if(fptr == NULL) return 0;
    char raw_offset[100];
    fgets(raw_offset, sizeof(raw_offset), fptr);
    fclose(fptr);
    return atoi(raw_offset);
}

static inline int sprite_x(int anchor, int i){
    return anchor + 64 * (i % 2);
    //order BLEFT, BRIGHT, TLEFT, TRIGHT. Only right sprite components need + 64
}

static inline int sprite_y(int anchor, int i){
    return anchor - 64 * (i > 1);
    //order BLEFT, BRIGHT, TLEFT, TRIGHT. Only top sprite components need - 64
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


static int renderer_show_portrait(SpritePosition pos, int num_components, 
                                    const char *name, const char *expression) {
    int additional_offset = get_portrait_offset(name, expression);
    int oam_base = sprite_info[pos].oam_base;
    int anchor_x = sprite_info[pos].pixel_anchor_x;
    int anchor_y = sprite_info[pos].pixel_anchor_y;
    for(int i = 0; i < num_components; ++i){
        char path[MAX_SPRITE_FILENAME_LEN];
        int n = snprintf(path, sizeof(path), "nitro:/grit/sprites/%s/%s/%s", name, expression, spriteComponent[i]);
        if(n >= sizeof(path)) return -1;
        int ret = load_bmp_sprite(path, sprite_mem_main[oam_base + i]);
        if (ret!= 0) return ret;
        oamSet(&oamMain, oam_base + i, sprite_x(anchor_x, i) + additional_offset, 
        sprite_y(anchor_y, i), 0, 15,SpriteSize_64x64, SpriteColorFormat_Bmp, sprite_mem_main[oam_base + i],
        -1, false, false, false, false, false);
    }
    return 0;
}

int renderer_show_left(const char *name, const char *expression) {
    renderer_hide_center();
    int ret = renderer_show_portrait(SPRITE_POSITION_LEFT, SPR_NUM_COMPONENTS, name, expression);
    if(ret != 0) return ret;
    int slot_res = update_slot_state(&g_renderer_state.slot_left, name, expression);
    return slot_res == 0 ? 0 : -1;
}

int renderer_show_right(const char *name, const char *expression) {
    renderer_hide_center();
    int ret = renderer_show_portrait(SPRITE_POSITION_RIGHT, SPR_NUM_COMPONENTS, name, expression);
    if(ret != 0) return ret;
    int slot_res = update_slot_state(&g_renderer_state.slot_right, name, expression);
    return slot_res == 0 ? 0 : -1;
}

int renderer_show_center(const char *name, const char *expression) {
    renderer_hide_left();
    renderer_hide_right();
    int ret = renderer_show_portrait(SPRITE_POSITION_CENTER, SPR_NUM_COMPONENTS, name, expression);
    if(ret != 0) return ret;
    int slot_res = update_slot_state(&g_renderer_state.slot_center, name, expression);
    return slot_res == 0 ? 0 : -1;
}

static void renderer_hide_portrait(SpritePosition pos, int num_components){
    int oam_base = sprite_info[pos].oam_base;
    for(int i = 0; i < num_components; ++i)
        oamSet(&oamMain, oam_base + i, 0, 0, 0, 15,SpriteSize_64x64,
                SpriteColorFormat_Bmp, sprite_mem_main[oam_base + i], -1, false, true, false, false, false);
}

void renderer_hide_left(void){
    renderer_hide_portrait(SPRITE_POSITION_LEFT, SPR_NUM_COMPONENTS);
    update_slot_state(&g_renderer_state.slot_left, "", "");
}

void renderer_hide_right(void){
    renderer_hide_portrait(SPRITE_POSITION_RIGHT, SPR_NUM_COMPONENTS);
    update_slot_state(&g_renderer_state.slot_right, "", "");
}

void renderer_hide_center(void){
    renderer_hide_portrait(SPRITE_POSITION_CENTER, SPR_NUM_COMPONENTS);
    update_slot_state(&g_renderer_state.slot_center, "", "");
}

int renderer_set_background(const char *bg_name, bool mainScreen) {
    int bg = mainScreen ? display_get_main_bg(MAIN_LAYER_SCENE) : display_get_sub_bg(SUB_LAYER_SCENE);
    void *bgData = NULL;
    size_t bgSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    char path[MAX_BACKGROUND_FILENAME_LEN];
    int n = snprintf(path, sizeof(path), "nitro:/grit/bg/%s_png.grf", bg_name);
    if(n >= sizeof(path)) return -1;
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
    char* buf = mainScreen ? &(g_renderer_state.background_main[0])
                           : &(g_renderer_state.background_sub[0]);
    snprintf(buf, MAX_BACKGROUND_FILENAME_LEN, "%s", bg_name);
    return 0;
}

void renderer_hide_background(bool mainScreen){
    int bg = mainScreen ? display_get_main_bg(MAIN_LAYER_SCENE) : display_get_sub_bg(SUB_LAYER_SCENE);
    char* buf = mainScreen ? &(g_renderer_state.background_main[0])
                           : &(g_renderer_state.background_sub[0]);
    snprintf(buf, MAX_BACKGROUND_FILENAME_LEN, "%s", "");
    bgHide(bg);
}

void renderer_show_textbox(bool mainScreen){
    int bg = mainScreen ? display_get_main_bg(MAIN_LAYER_TEXTBOX) : display_get_sub_bg(SUB_LAYER_TEXTBOX);
    bgShow(bg);
}

void renderer_hide_textbox(bool mainScreen){
    int bg = mainScreen ? display_get_main_bg(MAIN_LAYER_TEXTBOX) : display_get_sub_bg(SUB_LAYER_TEXTBOX);
    bgHide(bg);
}

int renderer_load_textbox(const char* textbox_name, bool mainScreen){
    if(mainScreen && strcmp(textbox_name, current_main_textbox) == 0) return 0;
    if(!mainScreen && strcmp(textbox_name, current_sub_textbox) == 0) return 0;
    if(mainScreen) strcpy(current_main_textbox, textbox_name);
    else strcpy(current_sub_textbox, textbox_name);
    int textboxBg = mainScreen ? display_get_main_bg(MAIN_LAYER_TEXTBOX) : display_get_sub_bg(SUB_LAYER_TEXTBOX);
    void *bgData = NULL;
    size_t bgSize = 0;
    void *palData = NULL;
    size_t palSize = 0;
    void *mapData = NULL;
    size_t mapSize = 0;
    char path[128];
    int n = snprintf(path, sizeof(path), "nitro:/grit/ui/%s_png.grf", textbox_name);
    if(n >= sizeof(path)) return -1;
    GRFError err = grfLoadPath(path, NULL, &bgData, &bgSize,
                               &mapData, &mapSize, &palData, &palSize); 
    if(err != GRF_NO_ERROR) return -1;
    memcpy(bgGetGfxPtr(textboxBg), bgData, bgSize);
    memcpy(bgGetMapPtr(textboxBg), mapData, mapSize);
    if(mainScreen){
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
        REG_BLDALPHA_SUB = 12 | (4 << 8);
    }
    free(bgData);
    free(mapData);
    free(palData);
    bgSetPriority(textboxBg, 0);
    return 0;
}

void renderer_show_choice_overlay(int choice_index, int num_choices) {
    int dy = num_choices == 2 ? CHOICE_TWO_OFFSET_Y : num_choices == 3 ? CHOICE_THREE_OFFSET_Y : CHOICE_FOUR_OFFSET_Y;
    int y_0 = num_choices == 2 ? CHOICE_TWO_BASE_Y : num_choices == 3 ? CHOICE_THREE_BASE_Y : CHOICE_FOUR_BASE_Y;
    for(int j = 0; j < 3; ++j){
        oamSet(&oamSub, j, 32 + (64 * j), y_0 + (choice_index * dy), 0, 0, SpriteSize_64x32, 
        SpriteColorFormat_256Color,  sprite_mem_sub[j], -1, 
        false, false, false, false, false);
    }
}

void renderer_hide_choice_overlay(void){
    for(int j = 0; j < 3; ++j){
        oamSet(&oamSub, j, 32 + (64 * j), 0, 0, 0, SpriteSize_64x32, 
        SpriteColorFormat_256Color,  sprite_mem_sub[j], -1, 
        false, true, false, false, false);
    }
}

void renderer_get_state(RendererState *out){
    memcpy(out, &g_renderer_state, sizeof(*out));
}
