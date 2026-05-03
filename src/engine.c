#include "engine.h"
#include "parser.h"
#include "interpreter.h"
#include "script.h"
#include "display.h"
#include "choice.h"
#include "renderer.h"
#include <stdbool.h>
#include <string.h>

void engine_init(void){
    display_init();
    interpreter_init();
}

void engine_reset(void){
    interpreter_reset();
}

void engine_shutdown(void){
    interpreter_shutdown();
    display_shutdown();
}

int engine_load_scene(const char *scene_name){
    return script_open(scene_name);
}

EngineResult engine_update(void){
    if(!script_is_open()) return ENGINE_RESULT_ERROR;
    char currentLine[PARSER_MAX_LINE_LEN];
    while(!interpreter_is_blocked()){
        ScriptResult script_res = script_read_line(&currentLine[0], PARSER_MAX_LINE_LEN);
        if(script_res == SCRIPT_RESULT_ERROR) return ENGINE_RESULT_ERROR;
        else if(script_res == SCRIPT_RESULT_EOF) return ENGINE_RESULT_FINISHED;
        else{
            ParsedCommand cmd;
            ParserResult parse_res = parser_parse_line(currentLine, &cmd);
            if(parse_res == PARSER_RESULT_ERROR) return ENGINE_RESULT_ERROR;
            else if(parse_res == PARSER_RESULT_OK){
                InterpreterResult int_res = interpreter_execute(&cmd);
                if(int_res == INTERPRETER_RESULT_BLOCKED) break;
                else if(int_res == INTERPRETER_RESULT_ERROR) return ENGINE_RESULT_ERROR;
                else if(int_res == INTERPRETER_RESULT_FINISHED) return ENGINE_RESULT_FINISHED;
            }
        }
        //interpreter_update();
    }
    interpreter_update();
    return ENGINE_RESULT_OK;
}

void engine_handle_input(u16 keys_down){
    InterpreterBlockType block_type = interpreter_is_blocked();
    switch(block_type) {
        case BLOCK_NONE: {
            return;
        }
        case BLOCK_TEXT: {
            if(keys_down & KEY_A) interpreter_advance();
            if(keys_down & KEY_TOUCH) interpreter_advance();
            break;
        }
        case BLOCK_WAIT: {
            if(keys_down & KEY_A) interpreter_advance();
            if(keys_down & KEY_TOUCH) interpreter_advance();
            break;
        }
        case BLOCK_CHOICE: {
            int current_index = choice_current_index();
            if(keys_down & KEY_DOWN){
                if(current_index < choice_num_choices() - 1) choice_set_choice(current_index + 1);
            } else if(keys_down & KEY_UP){
                if(current_index > 0) choice_set_choice(current_index - 1);
            } else if(keys_down & KEY_A){
                renderer_hide_choice_overlay();
                interpreter_advance();
                return;
            } else if(keys_down & KEY_TOUCH){
                touchPosition touch_pos;
                touchRead(&touch_pos);
                bool advance = choice_set_choice_touch(touch_pos.px, touch_pos.py);
                if(advance){
                    renderer_hide_choice_overlay();
                    interpreter_advance();
                    return;
                }
            }
            renderer_show_choice_overlay(choice_current_index(), choice_num_choices());
            break;
        }
    }
    return;
}

int engine_current_line(void){
    return script_current_line();
}

int engine_current_script(char* out, size_t out_size){
    return script_current_script(out, out_size);
}

