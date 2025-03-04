#include "../include/builtins.h"

int check_builtin(token_node* cmd_head){
    if(cmd_head!=NULL){
        if(cmd_head->token!=NULL){
            printf("%s\n",cmd_head->token);
        }
        printf("\n\ncmd_head->token == NULL\n\n");
        
    }
    return 0;
}