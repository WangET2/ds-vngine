#include "interpreter.h"
#include "parser.h"
#include "renderer.h"
#include "text.h"
#include "script.h"
#include <stdbool.h>
#include <string.h>

typedef enum {
    BLOCK_NONE = 0,
    BLOCK_WAIT,
    BLOCK_TEXT
} InterpreterBlockType;

static bool blocked;
static InterpreterBlockType blockType;

void interpreter_init(void){
    renderer_init();
    text_init();
    blocked = false;
    blockType = BLOCK_NONE;
}

void interpreter_reset(void){
    blocked = false;
    blockType = BLOCK_NONE;
}

/* InterpreterResult interpreter_execute(const ParsedCommand *cmd){
    if(strcmp(cmd->command, "SAY") == 0){
        blocked = true;
        text_begin_dialogue(&cmd->args[0], &cmd->args[1]);
        return INTERPRETER_RESULT_BLOCKED;
    } else if(strcmp(cmd->command, "NARRATE") == 0){
        blocked = true;
        text_begin_narration(&cmd->args[0]);
        return INTERPRETER_RESULT_BLOCKED;
    } else if(strcmp(cmd->command, "WAIT") == 0){
        blocked = true;
        return INTERPRETER_RESULT_BLOCKED;
    } else if(strcmp(cmd->command, "HIDE_LEFT") == 0){
        blocked = false;
        renderer_hide_left();
        return INTERPRETER_RESULT_OK;
    } else if(strcmp(cmd->command, "HIDE_RIGHT") == 0){
        blocked = false;
        renderer_hide_right();
        return INTERPRETER_RESULT_OK;
    } else if(strcmp(cmd->command, "HIDE_CENTER") == 0){
        blocked = false;
        renderer_hide_center();
        return INTERPRETER_RESULT_OK;
    } else if(strcmp(cmd->command, "SHOW_LEFT") == 0){
        blocked = false;
        int ret = renderer_show_left(&cmd->arg[0], &cmd->arg[1]);
        return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
    } else if(strcmp(cmd->command, "SHOW_RIGHT") == 0){
        blocked = false;
        int ret = renderer_show_right(&cmd->arg[0], &cmd->arg[1]);
        return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
    } else if(strcmp(cmd->command, "SHOW_CENTER") == 0){
        blocked = false;
        int ret = renderer_show_center(&cmd->arg[0], &cmd->arg[1]);
        return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
    } else if(strcmp(cmd->command, "BG") == 0){
        blocked = false;
        int ret = renderer_set_background(&cmd->arg[0]);
        return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
    }
    return INTERPRETER_RESULT_ERROR;
}
*/

InterpreterResult interpreter_execute(const ParsedCommand *cmd){
    int ret;
    switch(cmd->command) {
        case CMD_BG: {
            blocked = false;
            const char *bg_name = cmd->args[0];
            ret = renderer_set_background(bg_name);
            return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
        }
        case CMD_SHOW_LEFT: {
            blocked = false;
            const char *character = cmd->args[0];
            const char *expression = cmd->args[1];
            ret = renderer_show_left(character, expression);
            return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
        }
        case CMD_SHOW_RIGHT: {
            blocked = false;
            const char *character = cmd->args[0];
            const char *expression = cmd->args[1];
            ret = renderer_show_right(character, expression);
            return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
        }
        case CMD_SHOW_CENTER: {
            blocked = false;
            const char *character = cmd->args[0];
            const char *expression = cmd->args[1];
            ret = renderer_show_center(character, expression);
            return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
        }
        case CMD_HIDE_LEFT: {
            blocked = false;
            renderer_hide_left();
            return INTERPRETER_RESULT_OK;
        }
        case CMD_HIDE_RIGHT: {
            blocked = false;
            renderer_hide_right();
            return INTERPRETER_RESULT_OK;
        }
        case CMD_HIDE_CENTER: {
            blocked = false;
            renderer_hide_center();
            return INTERPRETER_RESULT_OK;
        }
        case CMD_SAY: {
            blocked = true;
            blockType = BLOCK_TEXT;
            const char *speaker = cmd->args[0];
            const char *dialogue = cmd->args[1];
            text_begin_dialogue(speaker, dialogue);
            return INTERPRETER_RESULT_BLOCKED;
        }
        case CMD_NARRATE: {
            blocked = true;
            blockType = BLOCK_TEXT;
            const char *dialogue = cmd->args[0];
            text_begin_narration(dialogue);
            return INTERPRETER_RESULT_BLOCKED;
        }
        case CMD_WAIT: {
            blocked = true;
            blockType = BLOCK_WAIT;
            return INTERPRETER_RESULT_BLOCKED;
        }
        case CMD_END: {
            return INTERPRETER_RESULT_FINISHED;
        }
        case CMD_LOAD: {
            script_close();
            renderer_reset();
            text_clear();
            const char *script_name = cmd->args[0];
            script_open(script_name);
            return INTERPRETER_RESULT_FINISHED;
        }
    }
    return INTERPRETER_RESULT_ERROR;
}

bool interpreter_is_blocked(void){
    return blocked;
}

void interpreter_update(void){
    renderer_update();
    if (!blocked) return;
    if(blockType == BLOCK_TEXT) text_update();
}

void interpreter_advance(void){
    if(!blocked) return;
    if(blockType == BLOCK_TEXT){
        if(!text_is_finished())
            text_finish_immediately();
        else
            interpreter_reset();
    }
    if(blockType == BLOCK_WAIT) interpreter_reset();
}