#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "tokenizer.h"
#include "utils.h"
#include "exec.h"

#define BUILTIN_COMMANDS_ARR_LENGTH 8

int check_builtin(token_node *, path_node **, char **, char**, char**, FILE*);
void exit_cmd(token_node *, path_node **, char **, char**, char**, FILE*);
int which_cmd(token_node *, path_node *);
int list_cmd(token_node *, path_node *); // needs to support pipes, eventually
int pwd_cmd();                           // needs to support pipes, eventually
int cd_cmd(token_node *, char **, char**);
int prompt_cmd(token_node *, char**);
void pid_cmd();
int printenv_cmd(token_node*);
int setenv_cmd(token_node*, path_node**);