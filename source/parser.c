#include "parser.h"

#include <string.h>

/* int parser_parse_line(const char *line,  ParsedCommand *out){
    if (strlen(line) == 0) return -1;
    char lineCpy[PARSER_MAX_LINE_LEN];
    strncpy(lineCpy, line, PARSER_MAX_LINE_LEN-1);
    if(lineCpy[0] == '#') return -1;
    char *stateptr;
    strcpy(out->command, strtok_r(lineCpy, " ", &stateptr));
    if(strcmp(out->command, "WAIT") == 0 || strcmp(out->command, "HIDE_LEFT") == 0 
    || strcmp(out->command, "HIDE_RIGHT") == 0 
    || strcmp(out->command, "HIDE_CENTER") == 0) out->arg_count = 0;
    else if(strcmp(out->command, "BG") == 0){
        out->arg_count = 1;
        strcpy(out->args[0], strtok_r(lineCpy, " ", &stateptr));
    } else if(strcmp(out->command, "SHOW_LEFT") == 0 || strcmp(out->command, "SHOW_RIGHT") == 0 
    || strcmp(out->command, "SHOW_CENTER") == 0) {
        out->arg_count = 2;
        strcpy(out->args[0], strtok_r(lineCpy, " ", &stateptr));
        strcpy(out->args[1], strtok_r(lineCpy, " ", &stateptr));
    } else if(strcmp(out->command, "SAY") == 0) {
        out->arg_count = 2;
        strcpy(out->args[0], strtok_r(lineCpy, " ", &stateptr));
        strcpy(out->args[1], stateptr);
    } else if(strcmp(out->command, "NARRATE") == 0) {
        out->arg_count = 1;
        strcpy(out->args[0], stateptr);
    } else return -2;
    return 0;

} */

int parser_parse_line(const char *line,  ParsedCommand *out){
    if (strlen(line) == 0) return -1;
    char lineCpy[PARSER_MAX_LINE_LEN];
    strncpy(lineCpy, line, PARSER_MAX_LINE_LEN-1);
    lineCpy[PARSER_MAX_LINE_LEN - 1] = '\0';
    if(lineCpy[0] == '#') return -1;
    char *stateptr = NULL;
    char *cmd;
    cmd = strtok_r(lineCpy, " ", &stateptr);
    if(cmd == NULL) return -1;
    if(strcmp(cmd, "BG") == 0){
        out->command = CMD_BG;
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
    } else if (strcmp(cmd, "WAIT") == 0){
        out->command = CMD_WAIT;
    } else if (strcmp(cmd, "END") == 0){
        out->command = CMD_END;
    } else if(strcmp(cmd, "LOAD") == 0){
        out->command = CMD_LOAD;
        strcpy(out->args[0], stateptr);
    } else return -2;
    return 0;
}