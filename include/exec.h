#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "utils.h"
#define CD_INSTEAD 99999

char* find_cmd_with_path(path_node*,char*);
int exec_cmd_with_path(token_node*, path_node*,int*);
int fork_and_exec(char*,char**);
int exec_abs_rel_path(token_node*, int*);
int is_directory(char*);