#include "../include/builtins.h"

int check_builtin(token_node* cmd_head){
    if(cmd_head!=NULL){
        printf("%s\n",cmd_head->token);
    }
    return 0;
}