#include "engine.h"
#include "parser.h"
#include "interpreter.h"
#include "script.h"
#include "display.h"

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
    char currentLine[500];
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
    if(block_type == BLOCK_NONE) return;
    if(block_type == BLOCK_TEXT && (keys_down & KEY_A)) interpreter_advance();
    if(block_type == BLOCK_CHOICE){
        if(keys_down & KEY_DOWN){
            //increment index stuff
        } else if(keys_down & KEY_UP){
            //decrement index stuff
        } else if(keys_down & KEY_A){
            //select stuff
        }
        //touch screen stuff should be grabbed as well... (pass touchRead as arg as well?)
    }

}

int engine_current_line(void){
    return script_current_line();
}

int engine_current_script(char* out, size_t out_size){
    return script_current_script(out, out_size);
}

