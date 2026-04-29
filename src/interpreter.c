#include "interpreter.h"
#include "parser.h"
#include "renderer.h"
#include "text.h"
#include "script.h"
#include "flags.h"
#include "choice.h"
#include <stdbool.h>
#include <string.h>

static bool blocked;
static InterpreterBlockType blockType;

void interpreter_init(void){
    renderer_init();
    text_init();
    flags_init();
    blocked = false;
    blockType = BLOCK_NONE;
}

void interpreter_reset(void){
    blocked = false;
    blockType = BLOCK_NONE;
}

void interpreter_shutdown(void){
    flags_destroy();
    text_clear();
    renderer_shutdown();
}

InterpreterResult interpreter_execute(const ParsedCommand *cmd){
    if(!cmd) return INTERPRETER_RESULT_ERROR;
    int ret;
    blockType = BLOCK_NONE;
    switch(cmd->command) {
        case CMD_BG: {
            blocked = false;
            const char *bg_name = cmd->args[0];
            ret = renderer_set_background(bg_name, true);
            return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
        }
        case CMD_BG_SUB: {
            blocked = false;
            const char *bg_name = cmd->args[0];
            ret = renderer_set_background(bg_name, false);
            return ret == 0 ? INTERPRETER_RESULT_OK : INTERPRETER_RESULT_ERROR;
        }
        case CMD_HIDE_BG: {
            blocked = false;
            renderer_hide_background(true);
            return INTERPRETER_RESULT_OK;
        }
        case CMD_HIDE_BG_SUB: {
            blocked = false;
            renderer_hide_background(false);
            return INTERPRETER_RESULT_OK;
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
            renderer_load_textbox("maintextbox", true);
            renderer_show_textbox(true);
            renderer_hide_textbox(false);
            text_begin_dialogue(speaker, dialogue);
            return INTERPRETER_RESULT_BLOCKED;
        }
        case CMD_NARRATE: {
            blocked = true;
            blockType = BLOCK_TEXT;
            const char *dialogue = cmd->args[0];
            renderer_load_textbox("subtextbox", false);
            renderer_show_textbox(false);
            renderer_hide_textbox(true);
            text_begin_narration(dialogue);
            return INTERPRETER_RESULT_BLOCKED;
        }
        case CMD_HIDE_SAY: {
            blocked = false;
            renderer_hide_textbox(true);
            text_clear();
            return INTERPRETER_RESULT_OK;
        }
        case CMD_HIDE_NARRATE: {
            blocked = false;
            renderer_hide_textbox(false);
            text_clear();
            return INTERPRETER_RESULT_OK;
        }
        case CMD_FLAG: {
            blocked = false;
            const char *flag = cmd->args[0];
            flags_set(flag);
            return INTERPRETER_RESULT_OK;
        }
        case CMD_UNSET: {
            blocked = false;
            const char *flag = cmd->args[0];
            flags_unset(flag);
            return INTERPRETER_RESULT_OK;
        }
        case CMD_IF: {
            blocked = false;
            const char *flag = cmd->args[0];
            if(flags_has(flag)){
                const char *line = cmd->args[1];
                ParsedCommand new_cmd;
                ParserResult inner_res = parser_parse_line(line, &new_cmd);
                if(inner_res == PARSER_RESULT_ERROR|| inner_res == PARSER_RESULT_EMPTY) return INTERPRETER_RESULT_ERROR;
                return interpreter_execute(&new_cmd);
            }
            return INTERPRETER_RESULT_OK;
        }
        case CMD_IFN: {
            blocked = false;
            const char *flag = cmd->args[0];
            if(!flags_has(flag)){
                const char *line = cmd->args[1];
                ParsedCommand new_cmd;
                ParserResult inner_res = parser_parse_line(line, &new_cmd);
                if(inner_res == PARSER_RESULT_ERROR || inner_res == PARSER_RESULT_EMPTY) return INTERPRETER_RESULT_ERROR;
                return interpreter_execute(&new_cmd);
            }
            return INTERPRETER_RESULT_OK;
        }
        case CMD_CHOICE: {
            blocked = true;
            blockType = BLOCK_CHOICE;
            //renderer set ui, text draw choice text, etc.
            int arr_size = cmd->num_args;
            char *instructions[arr_size/2];
            for(int i = 1; i < arr_size; i+=2){
                instructions[i/2] = cmd->args[i];
            }
            choice_init(instructions, arr_size/2);
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
        case CMD_PASS: {
            return INTERPRETER_RESULT_OK;
        }
        case CMD_LOAD: {
            script_close();
            const char *script_name = cmd->args[0];
            ret = script_open(script_name);
            if(ret != 0) return INTERPRETER_RESULT_ERROR;
            renderer_reset();
            text_clear();
            return INTERPRETER_RESULT_OK;
        }
    }
    return INTERPRETER_RESULT_ERROR;
}

InterpreterBlockType interpreter_is_blocked(void){
    return blockType;
}

void interpreter_update(void){
    renderer_update();
    if (!blocked) return;
    if(blockType == BLOCK_TEXT) text_update();
}

InterpreterResult interpreter_advance(void){
    switch(blockType) {
        case BLOCK_NONE: {
            return INTERPRETER_RESULT_OK;
        }
        case BLOCK_TEXT: {
            if(!text_is_finished())
                text_finish_immediately();
            else
                interpreter_reset();
            break;
        }
        case BLOCK_WAIT: {
            interpreter_reset();
            break;
        }
        case BLOCK_CHOICE: {
            char *line[PARSER_MAX_TOKEN_LEN];
            int res = choice_get_choice(&line[0], PARSER_MAX_TOKEN_LEN);
            if(res == -1){
                choice_reset();
                return -1;
            }
            ParsedCommand cmd;
            ParserResult parse_res = parser_parse_line(line, &cmd);
            if(parse_res == PARSER_RESULT_ERROR){
                choice_reset();
                return INTERPRETER_RESULT_ERROR;         
            }
            InterpreterResult int_res = interpreter_execute(&cmd);
            interpreter_reset();
            choice_reset();
            text_debug_clear();
            return int_res;
            break;
        }
    }
    text_debug_clear();
    return INTERPRETER_RESULT_OK;
}