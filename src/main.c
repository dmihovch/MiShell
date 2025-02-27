#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../include/utils.h"
#include "../include/builtins.h"
#include "../include/exec.h"
#include "../include/tokenizer.h"


#define MAX_INPUT 1024

int main(int argc, char **argv){
    
    char* cmd_raw = NULL;
    size_t cmd_len = 0;
    ssize_t read;

    read = getline(&cmd_raw, &cmd_len, stdin);
    cmd_raw[strcspn(cmd_raw, "\n")] = 0;
    printf("Raw Command:{%s}\nCommand Length:{%ld}",cmd_raw, cmd_len);
    
    for(int i = 0; i<cmd_len; i++){
        printf("%c\n", cmd_raw[i]);
    }

    char* tok = strtok(cmd_raw, " ");
    while(tok!=NULL){
        printf("Token={%s}\n",tok);
        tok = strtok(NULL, " ");
    }

   

    free(cmd_raw);

    return 0;

}