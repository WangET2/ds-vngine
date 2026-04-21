#include <nds.h>
#include <stdlib.h>
#include <filesystem.h>
#include "renderer.h"
#include "text.h"
#include "parser.h"
#include "interpreter.h"
#include "script.h"
#include "engine.h"

__attribute__((noreturn)) void wait_forever(void)
{
    while (1)
    {
        swiWaitForVBlank();

        scanKeys();
        if (keysDown() & KEY_START)
            exit(1);
    }
}

int main(int argc, char *argv[])
{
    consoleDemoInit();

    printf("Initializing NitroFS...\n");

    if (!nitroFSInit(NULL))
    {
        perror("nitroFSInit()");
        wait_forever();
    }

    consoleClear();

    engine_init();
    int ret = engine_load_scene("demo");
    if(ret != 0){
        consoleDemoInit();
        printf("Error loading scene!");
        wait_forever();
    }
    else if(ret == 0){
        while (1)
        {
            swiWaitForVBlank();
            scanKeys();

            u16 keys_down = keysDown();
            engine_handle_input(keys_down);
            engine_update();
        }
    }


    return 0;
}


