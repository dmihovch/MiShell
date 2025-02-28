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
    ssize_t num_read;



    num_read = getline(&cmd_raw, &cmd_len, stdin);
    if(num_read != -1){
        if(cmd_raw[num_read-1]=='\n'){
            cmd_raw[num_read-1] = '\0';
        }
    }

    token_node* head = tokenizer(cmd_raw);
    print_tokens_debug(head);
    free_tokens(head);
    

    

    /*
    
    char* tok = strtok(cmd_raw, " ");
    while(tok!=NULL){
        printf("Token={%s}\n",tok);
        tok = strtok(NULL, " ");
    }



    */


    
   free(cmd_raw);
    return 0;

}
