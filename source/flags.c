#include "flags.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define FLAG_BUCKET_COUNT 64

typedef struct FlagNode{
    const char *key;
    struct FlagNode *next;
} FlagNode;

static FlagNode *g_buckets[FLAG_BUCKET_COUNT];

uint32_t hash_fnv1a(const char *str) {
    uint32_t hash = 2166136261u; // offset basis

    while (*str) {
        hash ^= (uint8_t)(*str++);
        hash *= 16777619u; // FNV prime
    }

    return hash;
}

void flags_init(void){
    for(int i = 0; i < FLAG_BUCKET_COUNT; ++i) g_buckets[i] = NULL;
}

void flags_reset(void){
    for(int i = 0; i < FLAG_BUCKET_COUNT; ++i){
        FlagNode *cur = g_buckets[i];
        while(cur != NULL){
            FlagNode *next = cur->next;
            free((char *) cur->key);
            free(cur);
            cur = next;
        }
    }
}

void flags_destroy(void){
    flags_reset();
}

bool flags_has(const char *key){
    uint32_t index = hash_fnv1a(key) % FLAG_BUCKET_COUNT;
    FlagNode *cur = g_buckets[index];
    while(cur != NULL){
        if(strcmp(cur->key, key) == 0) return true;
        cur = cur->next;
    }
    return false;
}

bool flags_set(const char *key){
    if(flags_has(key)) return true;
    uint32_t index = hash_fnv1a(key) % FLAG_BUCKET_COUNT;
    FlagNode *node = malloc(sizeof(FlagNode));
    if (!node) return false;
    node->key = strdup(key);
    if(!node->key){
        free(node);
        return false;
    }
    if(g_buckets[index] == NULL){
        g_buckets[index] = node;
        node->next = NULL;
    } else {
        node->next = g_buckets[index];
        g_buckets[index] = node;
    }
    return true;
}

bool flags_unset(const char *key){
    uint32_t index = hash_fnv1a(key) % FLAG_BUCKET_COUNT;
    FlagNode *cur = g_buckets[index];
    FlagNode *prev = NULL;
    while(cur != NULL){
        if(strcmp(cur->key, key) == 0){
            if(prev != NULL){
                prev->next = cur->next;
            } else{
                g_buckets[index] = cur->next;
            }
            free((char *) cur->key);
            free(cur);
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}