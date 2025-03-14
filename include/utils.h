#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tokenizer.h"

typedef struct path_node
{
    char *path;
    struct path_node *next;
} path_node;

path_node *get_path();
void free_path(path_node **);
void print_path_debug(path_node *);
void free_all_mallocs(token_node *, path_node **, char **, char **, char**);
void reassign_current_and_previous_directory(char**, char**, char**);
int get_input(char**,size_t*);
void print_whole_environment();
