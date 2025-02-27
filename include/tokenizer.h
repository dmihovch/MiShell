#include <stdio.h>
#include <string.h>

typedef struct TokenNode {
    char* token;
    struct TokenNode *next;
    struct TokenNode *prev;
} TokenNode;
TokenNode** tokenizer(char*);