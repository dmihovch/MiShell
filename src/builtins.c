#include "../include/builtins.h"

int check_builtin(token_node* cmd_head, path_node* path){
    //printf("Entering builtin\n");
    if(cmd_head!=NULL && cmd_head->token != NULL){
        //printf("%s\n",cmd_head->token);
        if(strcmp(cmd_head->token, "exit") == 0){ exit_cmd(cmd_head,path);}
        
    }
    else{
        printf("cmd_head || cmd_head->token == NULL\n");
        return 0;
    }
    
    return 0; //return codes?   
}



//should be good
void exit_cmd(token_node* cmd_head, path_node* path){
    token_node* arg_node = cmd_head->next;
    int ret_code;
    if(arg_node != NULL && arg_node->token != NULL){
        ret_code = atoi(arg_node->token);
        printf("Exiting shell with code %d\n",ret_code);
        free_tokens(cmd_head);
        free_path(path);
        exit(ret_code);
    }
    free_tokens(cmd_head);
    free_path(path);
    printf("Exiting shell with code 0, token is NULL\n"); //delete after 0 for sub
    exit(0);
}

void which_cmd(token_node* cmd_head){
    const char *builtin_commands[] = {"exit","which","list","pwd","cd","pwd","pid","prompt"};
    token_node* arg_node = cmd_head->next;
    if(arg_node == NULL){
        return;
    }
    bool found_cmd;
    while(arg_node!=NULL && arg_node->token != NULL){
        found_cmd = false;
        for(int i = 0; i<BUILTIN_COMMANDS_ARR_LENGTH; i++){
            if(strcmp(arg_node->token,builtin_commands[i])==0){
                printf("%s: shell built-in command\n",arg_node->token);
                found_cmd = true;
                break;
            }
        }
        if(found_cmd){
            arg_node = arg_node -> next;
            continue;
        }

        

            

    }
    
    
    
}