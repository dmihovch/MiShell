#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct token_node {
    char* token;
    struct token_node *next;
    struct token_node *prev;
} token_node;
token_node* tokenizer(char*);
void free_tokens(token_node*);
void print_tokens_debug(token_node*);
