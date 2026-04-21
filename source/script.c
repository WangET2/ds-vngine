#include "script.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static FILE *fptr = NULL;
static int lineNum;
char currentScene[100];

void script_close(void){
    if(fptr != NULL) fclose(fptr);
    fptr = NULL;
    strcpy(&currentScene[0], "");
    lineNum = 0;
}

int script_open(const char* scene_name){
    if(fptr != NULL) script_close();
    char path[100];
    snprintf(path, sizeof(path), "nitro:/scripts/%s.txt", scene_name);
    fptr = fopen(path, "r");
    if (fptr == NULL) return -1;
    lineNum = 0;
    strcpy(&currentScene[0], scene_name);
    return 0;
}

ScriptResult script_read_line(char *out_line, size_t out_size){
    if(fptr == NULL || out_line == NULL || out_size == 0) return SCRIPT_RESULT_ERROR;
    if(fgets(out_line, out_size, fptr) == NULL){
        if(feof(fptr)) return SCRIPT_RESULT_EOF;
        else return SCRIPT_RESULT_ERROR;
    }
    size_t len = strlen(out_line);
    if(len > 0 && out_line[len - 1] == '\n')
        out_line[strcspn(out_line, "\r\n")] = '\0';
    ++lineNum;
    return SCRIPT_RESULT_OK;
}

bool script_is_open(void) {
    return fptr != NULL;
}

int script_current_line(void) {
    if(script_is_open()) return lineNum;
    return 0;
}

int script_current_script(char *out, size_t out_size){
    if(!script_is_open()) return -1;
    int ret = snprintf(out, out_size, "%s", &currentScene[0]);
    return ret < out_size ? 0 : -1;
}



