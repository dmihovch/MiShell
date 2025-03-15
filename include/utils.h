#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <stdbool.h>
#include "../include/tokenizer.h"




typedef struct path_node
{
    char *path;
    struct path_node *next;
} path_node;

path_node *get_path();
void free_path(path_node **);
void print_path_debug(path_node *);
void free_all_mallocs(token_node *, path_node **, char **, char **, char**, FILE*);
void reassign_current_and_previous_directory(char**, char**, char**);
int get_input(char**,size_t*,FILE*);
void print_whole_environment();
void disable_ctrl_printing();
void handle_signal(int);
void reset_terminal_settings();
int handle_exit_logic(token_node *, path_node **, char **, char **, char** , FILE* , int);
void read_directory(DIR*);
int open_directory_and_read(char*, bool);