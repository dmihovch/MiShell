#include "../include/exec.h"

//remember to free whatever variable this is assigned to
char* find_cmd_with_path(path_node* path, char* cmd_name){
    

    //make sure to change the value of the cmd string size when I figure out the limting input

    char* cmd_path_buffer = (char*)malloc(sizeof(char)*256);

    //lmao
    while(path!=NULL && path->path!=NULL){
        sprintf(cmd_path_buffer, "%s/%s", path->path,cmd_name);
        if(access(cmd_path_buffer, X_OK) == 0){
            return cmd_path_buffer;
        }
        path = path->next;
    }
    free(cmd_path_buffer);
    return NULL;
}