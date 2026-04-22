#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int parser_fill_args(ParsedCommand *out, int num_args, char *stateptr, bool consumes_rest);

ParserResult parser_parse_line(const char *line,  ParsedCommand *out){
    if(line == NULL || out == NULL) return PARSER_RESULT_ERROR;
    if (strlen(line) == 0) return PARSER_RESULT_EMPTY;

    memset(out, 0, sizeof(*out));

    char lineCpy[PARSER_MAX_LINE_LEN];

    snprintf(lineCpy, sizeof(lineCpy), "%s", line);
    char *p = lineCpy;
    while(*p == ' ' || *p == '\t') ++p;

    if(*p == '\0' || *p == '\n' || *p == '#') return PARSER_RESULT_EMPTY;

    lineCpy[PARSER_MAX_LINE_LEN - 1] = '\0';
    
    char *stateptr = NULL;
    char *cmd;
    cmd = strtok_r(p, " ", &stateptr);
    if(cmd == NULL) return PARSER_RESULT_ERROR;

    const struct InstructionKeyword *kw = in_word_set(cmd, strlen(cmd));
    if(!kw) return PARSER_RESULT_ERROR;
    out->command = kw->type;
    if(kw->num_args != -1){
        int ret = parser_fill_args(out, kw->num_args, stateptr, kw->consumes_rest);
        return ret != -1 ? PARSER_RESULT_OK : PARSER_RESULT_ERROR;
    }
    switch(kw->type) {
        case CMD_CHOICE: {
            int num_args = 0;
            while(1){
                if(num_args >= 8) return PARSER_RESULT_ERROR;
                char *text = strtok_r(NULL, "{", &stateptr);
                if(!text || *text == '\0') return PARSER_RESULT_ERROR;
                int n = snprintf(out->args[num_args], PARSER_MAX_TOKEN_LEN, "%s", text);
                if (n >= PARSER_MAX_TOKEN_LEN) return PARSER_RESULT_ERROR;
                //Need to strip trailing whitespace:
                out->args[num_args][strlen(text) - 1] = '\0';
                ++num_args;

                char *instr = strtok_r(NULL, "}", &stateptr);
                if(!instr || *instr == '\0') return PARSER_RESULT_ERROR;
                int m = snprintf(out->args[num_args], PARSER_MAX_TOKEN_LEN, "%s", instr);
                if (m>= PARSER_MAX_TOKEN_LEN) return PARSER_RESULT_ERROR;
                ++num_args;
                if(!stateptr || *stateptr == '\0'){
                    if(num_args < 4) return PARSER_RESULT_ERROR;
                    out->num_args = num_args;
                    return PARSER_RESULT_OK;
                }
                //Advance past trailing whitespace
                stateptr++;
            }
            break;
        } 
    }
    return PARSER_RESULT_OK;
}

int parser_fill_args(ParsedCommand *out, int num_args, char *stateptr, bool consumes_rest){
    int ubound = consumes_rest ? num_args - 1 : num_args;
    for(int i = 0; i < ubound; ++i){
        char *ret = strtok_r(NULL, " ", &stateptr);
        if(!ret || *ret == '\0') return -1;
        int n = snprintf(out->args[i], PARSER_MAX_TOKEN_LEN, "%s", ret);
        if(n >= PARSER_MAX_TOKEN_LEN) return -1;
    }
    if(consumes_rest){
        if(!stateptr || *stateptr == '\0') return -1;
        int n = snprintf(out->args[num_args-1], PARSER_MAX_TOKEN_LEN, "%s", stateptr);
        if(n >= PARSER_MAX_TOKEN_LEN) return -1;
    }
    out->num_args = num_args;
    if(!consumes_rest && stateptr && *stateptr != '\0') return -1;
    return 0;
}