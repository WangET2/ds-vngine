#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

int parser_fill_args(ParsedCommand *out, int num_args, char *stateptr);

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
        int ret = parser_fill_args(out, kw->num_args, stateptr);
        return ret != -1 ? PARSER_RESULT_OK : PARSER_RESULT_ERROR;
    }
    switch(kw->type) {
        case CMD_CHOICE: {
            //TODO: Implement variable length CHOICE
            strcpy(out->args[0], strtok_r(NULL, " {", &stateptr));
            strcpy(out->args[1], strtok_r(NULL, "} ", &stateptr));
            strcpy(out->args[2], strtok_r(NULL, " {", &stateptr));
            strcpy(out->args[3], strtok_r(NULL, "}", &stateptr));
            break;
        } 
    }
    return PARSER_RESULT_OK;
}

int parser_fill_args(ParsedCommand *out, int num_args, char *stateptr){
    for(int i = 0; i < num_args - 1; ++i){
        char *ret = strtok_r(NULL, " ", &stateptr);
        if(!ret || *ret == '\0') return -1;
        int n = snprintf(out->args[i], PARSER_MAX_TOKEN_LEN, "%s", ret);
        if(n >= PARSER_MAX_TOKEN_LEN) return -1;
    }
    if((!stateptr || *stateptr == '\0') && num_args != 0) return -1;
    int n = snprintf(out->args[num_args-1], PARSER_MAX_TOKEN_LEN, "%s", stateptr);
    if(n >= PARSER_MAX_TOKEN_LEN) return -1;
    out->num_args = num_args;
    return 0;
}