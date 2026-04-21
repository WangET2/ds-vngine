#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

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

    switch(kw->type) {
        case CMD_BG: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            break;
        }
        case CMD_BG_SUB: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            break;
        }
        case CMD_SHOW_LEFT: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            strcpy(out->args[1], strtok_r(NULL, " ", &stateptr));
            break;
        }
        case CMD_SHOW_RIGHT: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            strcpy(out->args[1], strtok_r(NULL, " ", &stateptr));
            break;
        }
        case CMD_SHOW_CENTER: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            strcpy(out->args[1], strtok_r(NULL, " ", &stateptr));
            break;
        }
        case CMD_SAY: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            strcpy(out->args[1], stateptr);
            break;
        } 
        case CMD_NARRATE: {
            strcpy(out->args[0], stateptr);
            break;
        } 
        case CMD_FLAG: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            break;
        } 
        case CMD_UNSET: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            break;
        } 
        case CMD_IF: {
            strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
            strcpy(out->args[1], stateptr);
            break;
        } 
        case CMD_CHOICE: {
            //TODO: Implement variable length CHOICE
            strcpy(out->args[0], strtok_r(NULL, " {", &stateptr));
            strcpy(out->args[1], strtok_r(NULL, "} ", &stateptr));
            strcpy(out->args[2], strtok_r(NULL, " {", &stateptr));
            strcpy(out->args[3], strtok_r(NULL, "}", &stateptr));
            break;
        } 
        case CMD_LOAD: {
            strcpy(out->args[0], stateptr);
            break;
        }
    }
    return PARSER_RESULT_OK;
}