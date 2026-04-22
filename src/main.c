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
            EngineResult res = engine_update();

            if(res == ENGINE_RESULT_ERROR){
                consoleDemoInit();
                char buf[100];
                engine_current_script(&buf[0], sizeof(buf));
                printf("Error executing scene!\nScript Name:%s\nLine Number:%d", buf, engine_current_line());
                break;
            }

            if(res == ENGINE_RESULT_FINISHED) break;
        }
    }

    while(1){
        swiWaitForVBlank();
        scanKeys();
        u16 keys_down = keysDown();
        if(keys_down & KEY_START) break;
    }

    engine_shutdown();

    return 0;
}


