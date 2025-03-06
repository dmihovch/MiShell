#include "../include/exec.h"

//remember to free whatever variable this is assigned to
char* find_cmd_with_path(path_node* path, char* cmd_name){
    
    
    //make sure to change the value of the cmd string size when I figure out the limting input

    //lmao
    while(path!=NULL && path->path!=NULL){


        printf("%ld\n",strlen(path->path));
        char* cmd_path_buffer = malloc(sizeof(char)*(strlen(path->path)+strlen(cmd_name)));

        sprintf(cmd_path_buffer, "%s/%s", path->path,cmd_name);
        if(access(cmd_path_buffer, X_OK) == 0){
            return cmd_path_buffer;
        }
        free(cmd_path_buffer);
        path = path->next;
        
    }
    return NULL;
}