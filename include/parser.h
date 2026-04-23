#ifndef PARSER_H
#define PARSER_H

#define PARSER_MAX_ARGS 8
#define PARSER_MAX_TOKEN_LEN 256
#define PARSER_MAX_LINE_LEN 512

typedef enum{
    CMD_BG = 0,
    CMD_BG_SUB,
    CMD_HIDE_BG,
    CMD_HIDE_BG_SUB,

    CMD_SHOW_LEFT,
    CMD_SHOW_RIGHT,
    CMD_SHOW_CENTER,
    CMD_HIDE_LEFT,
    CMD_HIDE_RIGHT,
    CMD_HIDE_CENTER,

    CMD_FLAG,
    CMD_UNSET,
    CMD_IF,
    CMD_IFN,
    CMD_CHOICE,

    CMD_SAY,
    CMD_NARRATE,
    CMD_HIDE_SAY,
    CMD_HIDE_NARRATE,

    CMD_BGM,
    CMD_SFX,
    CMD_ENDBGM,
    
    CMD_WAIT,
    CMD_LOAD,
    CMD_PASS,
    CMD_END
} CommandType;

typedef enum{
    PARSER_RESULT_OK = 0,
    PARSER_RESULT_EMPTY,
    PARSER_RESULT_ERROR
} ParserResult;

typedef struct{
    CommandType command;
    char args[PARSER_MAX_ARGS][PARSER_MAX_TOKEN_LEN];
    int num_args;
} ParsedCommand;

ParserResult parser_parse_line(const char *line,  ParsedCommand *out);

#endif