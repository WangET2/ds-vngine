#include "parser.h"
#include <stdio.h>
#include <string.h>

ParserResult parser_parse_line(const char *line,  ParsedCommand *out){
    if(line == NULL || out == NULL) return PARSER_RESULT_ERROR;
    if (strlen(line) == 0) return PARSER_RESULT_EMPTY;

    memset(out, 0, sizeof(*out));

    char lineCpy[PARSER_MAX_LINE_LEN];
    //strncpy(lineCpy, line, PARSER_MAX_LINE_LEN-1);
    snprintf(lineCpy, sizeof(lineCpy), "%s", line);
    char *p = lineCpy;
    while(*p == ' ' || *p == '\t') ++p;

    if(*p == '\0' || *p == '\n' || *p == '#') return PARSER_RESULT_EMPTY;

    lineCpy[PARSER_MAX_LINE_LEN - 1] = '\0';
    //if(lineCpy[0] == '#') return PARSER_RESULT_EMPTY;
    char *stateptr = NULL;
    char *cmd;
    cmd = strtok_r(lineCpy, " ", &stateptr);
    if(cmd == NULL) return -1;
    if(strcmp(cmd, "BG") == 0){
        out->command = CMD_BG;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
    } else if(strcmp(cmd, "BGSUB") == 0){
        out->command = CMD_BG_SUB;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
    } else if (strcmp(cmd, "SHOW_LEFT") == 0){
        out->command = CMD_SHOW_LEFT;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
        strcpy(out->args[1], strtok_r(NULL, " ", &stateptr));
    } else if (strcmp(cmd, "SHOW_RIGHT") == 0){
        out->command = CMD_SHOW_RIGHT;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
        strcpy(out->args[1], strtok_r(NULL, " ", &stateptr));
    } else if (strcmp(cmd, "SHOW_CENTER") == 0){
        out->command = CMD_SHOW_CENTER;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
        strcpy(out->args[1], strtok_r(NULL, " ", &stateptr));
    } else if (strcmp(cmd, "HIDE_LEFT") == 0){
        out->command = CMD_HIDE_LEFT;
    } else if (strcmp(cmd, "HIDE_RIGHT") == 0){
        out->command = CMD_HIDE_RIGHT;
    } else if (strcmp(cmd, "HIDE_CENTER") == 0){
        out->command = CMD_HIDE_CENTER;
    } else if (strcmp(cmd, "SAY") == 0){
        out->command = CMD_SAY;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
        strcpy(out->args[1], stateptr);
    } else if (strcmp(cmd, "NARRATE") == 0){
        out->command = CMD_NARRATE;
        strcpy(out->args[0], stateptr);
    } else if(strcmp(cmd, "FLAG") == 0){
        out->command = CMD_FLAG;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
    } else if(strcmp(cmd, "UNSET") == 0){
        out->command = CMD_UNSET;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
    } else if(strcmp(cmd, "IF") == 0){
        out->command = CMD_IF;
        strcpy(out->args[0], strtok_r(NULL, " ", &stateptr));
        strcpy(out->args[1], stateptr);
    } else if(strcmp(cmd, "CHOICE") == 0){
        out->command = CMD_CHOICE;
        strcpy(out->args[0], strtok_r(NULL, " {", &stateptr));
        strcpy(out->args[1], strtok_r(NULL, "} ", &stateptr));
        strcpy(out->args[2], strtok_r(NULL, " {", &stateptr));
        strcpy(out->args[3], strtok_r(NULL, "}", &stateptr));
    } else if (strcmp(cmd, "WAIT") == 0){
        out->command = CMD_WAIT;
    } else if (strcmp(cmd, "END") == 0){
        out->command = CMD_END;
    } else if (strcmp(cmd, "PASS") == 0){
        out->command = CMD_PASS;
    } else if(strcmp(cmd, "LOAD") == 0){
        out->command = CMD_LOAD;
        strcpy(out->args[0], stateptr);
    } else return PARSER_RESULT_ERROR;
    return PARSER_RESULT_OK;
}