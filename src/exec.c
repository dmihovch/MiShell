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

int exec_cmd_with_path(token_node* cmd_head, path_node* path, int* cmd_found_and_exec){
    int ret_code = 0;

    if(cmd_head != NULL && cmd_head->token != NULL){
        char* cmd_path = find_cmd_with_path(path, cmd_head->token);
        if(cmd_path != NULL){
            *cmd_found_and_exec = 1;
            char** argv = create_argv_arr(cmd_head);
            if(argv != NULL){
                ret_code = fork_and_exec(cmd_path,argv);
                free(cmd_path);
                free_argv_arr(argv);
                return ret_code;
            }
            return ret_code;
        }
        return ret_code;
    }
    return ret_code;
}

int fork_and_exec(char* cmd_path, char** argv){
    int status;
    int exit_code;
    int pid = fork();
    if(pid == -1){
        perror("fork failed");
        return 1;
    }
    if(pid == 0){

        reset_terminal_settings();
        //this is the child process

        execve(cmd_path,argv,NULL);
        perror("execve failed");
        return 1;
    }
    if(pid > 0){
        waitpid(pid, &status, 0);
        if(WIFEXITED(status)){
            exit_code = WEXITSTATUS(status);
            if(exit_code != 0){
                printf("Command exited with status: %d\n", exit_code);
            }
        }
    } 
    return exit_code;
}

int exec_abs_rel_path(token_node* cmd, int* cmd_found_and_exec){
    int ret_code = 0;
    if(cmd != NULL && cmd->token != NULL){
        if(access(cmd->token, X_OK) == 0){
            *cmd_found_and_exec = 1;
            char** argv = create_argv_arr(cmd);
            ret_code = fork_and_exec(cmd->token, argv);
            free_argv_arr(argv);
            return ret_code;

        }
    }
}