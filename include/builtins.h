#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "utils.h"
#include "exec.h"


#define BUILTIN_COMMANDS_ARR_LENGTH 8


int check_builtin(token_node*, path_node*);
void exit_cmd(token_node*, path_node*);
void which_cmd(token_node*, path_node*);