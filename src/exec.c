#include "../include/exec.h"

//remember to free whatever variable this is assigned to
char* find_cmd_with_path(path_node* path, char* cmd_name){

    
    

    char* cmd_path_buffer;
    size_t buf_size;

    while(path!=NULL && path->path!=NULL){


        //printf("%ld\n",strlen(path->path));
        buf_size = (strlen(path->path)+strlen(cmd_name)+2);
        cmd_path_buffer = calloc(buf_size, sizeof(char));
        

        snprintf(cmd_path_buffer, buf_size, "%s/%s", path->path,cmd_name);
        if(access(cmd_path_buffer, X_OK) == 0){
            return cmd_path_buffer;
        }
        free(cmd_path_buffer);
        path = path->next;
        
    }
    return NULL;
}