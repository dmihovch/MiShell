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

    token_node* head;

    //return codes for functions
    int return_code;


    //collecting prompt
    char* cmd_raw;
    size_t cmd_len;
    ssize_t num_read;



    const path_node* path = get_path();
    print_path_debug(path);


    while(1) {

        return_code = 0;
        cmd_raw = NULL;
        cmd_len = 0;
        num_read = 0;


        //reads in prompt and gets rid of newline
        printf("$ ");
        num_read = getline(&cmd_raw, &cmd_len, stdin);
        if(num_read == 1){ //handles just hitting enter
            free(cmd_raw);
            continue;
        }
        if(num_read != -1){ //replaces newline with null terminator
            if(cmd_raw[num_read-1]=='\n'){
                cmd_raw[num_read-1] = '\0';
            }
        }


        head = tokenizer(cmd_raw);
        free(cmd_raw);
        if(head == NULL){
            continue;
        }
        print_tokens_debug(head);
        return_code = check_builtin(head,path);
        
        
        
        free_tokens(head);
    }
    
    
    

    printf("\n\n\n\n\nTHIS SHOULD NEVER? PRINT!!\n\n\n\n\n");

    

    return 0;

}
