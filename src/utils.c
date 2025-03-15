#include "../include/utils.h"

// just put this in main?
path_node *get_path()
{
    char *path_raw = getenv("PATH");
    // printf("Raw PATH:\n\n%s\n\n",path_raw);
    if (path_raw != NULL)
    {
        char *path_elem = strtok(path_raw, ":");

        if (path_elem == NULL)
        {
            return NULL;
        }
        path_node *head = (path_node *)calloc(1, sizeof(path_node));
        head->path = strdup(path_elem);

        path_node *cur = head;
        path_elem = strtok(NULL, ":");

        while (path_elem != NULL)
        {
            cur->next = (path_node *)calloc(1, sizeof(path_node));
            cur->next->path = strdup(path_elem);
            cur = cur->next;
            path_elem = strtok(NULL, ":");
        }
        return head;
    }
    return NULL;
}

void free_path(path_node **head)
{
    path_node *cur = *head;
    path_node *tmp = NULL;
    while (cur != NULL)
    {
        tmp = cur;
        cur = cur->next;
        if (tmp->path != NULL)
        {
            free(tmp->path);

        }
        free(tmp);
        
    }

    *head = NULL;
    
}

//if calling this, need to deref path
void print_path_debug(path_node *head)
{
    while (head != NULL && head->path != NULL)
    {
        printf("{%s}\n", head->path);
        head = head->next;
    }
}

void free_all_mallocs(token_node *cmd_head, path_node **path, char **previous_directory, char **current_directory, char** prompt_prefix, FILE* input)
{
    if (cmd_head != NULL)
    {
        free_tokens(cmd_head);
    }
    if (path != NULL && *path != NULL)
    {
        free_path(path);
    }
    if (previous_directory != NULL && *previous_directory != NULL)
    {
        free(*previous_directory);
    }
    if(current_directory != NULL && *current_directory != NULL){
        free(*current_directory);
    }
    if(prompt_prefix != NULL && *prompt_prefix != NULL){
        free(*prompt_prefix);
    }
    if(input != stdin){
        fclose(input);
    }
}


//tmp_current_directory is free'd, current and previous are both still malloc'd
void reassign_current_and_previous_directory(char** current_directory, char** previous_directory, char** tmp_current_directory){
    free(*current_directory);
    *current_directory = getcwd(NULL,0);
    if(*current_directory == NULL){
        perror("Failed to getcwd in cd_cmd current_directory");
    }
    free(*previous_directory);
    *previous_directory = strdup(*tmp_current_directory);
    free(*tmp_current_directory);
}

int get_input(char** cmd_raw,size_t* cmd_len, FILE* input){
    *cmd_raw = NULL;
    int num_read = getline(cmd_raw, cmd_len, input);

    if (num_read == 1)
    { // handles just hitting enter

        if(*cmd_raw != NULL){
             free(*cmd_raw);
            *cmd_raw = NULL;
        }
        return num_read;
    }


    //jesus f'ing christ I want to meat crayon myself over this
    if(num_read == -1){
        if(feof(stdin)){
            clearerr(stdin);
            if(*cmd_raw != NULL){
                free(*cmd_raw);
                *cmd_raw = NULL;
            }
            printf("\n");
            return num_read;
        }
        if(feof(input)){
            clearerr(input);
            if(*cmd_raw!=NULL){
                free(*cmd_raw);
                *cmd_raw = NULL;
            }
            return num_read;
        }
    }
    
    // replaces newline with null terminator
    if ((*cmd_raw)[num_read - 1] == '\n')
    {
        (*cmd_raw)[num_read - 1] = '\0';
        --num_read; //this could be an issue
    }
    return num_read;
}

void print_whole_environment(){
    extern char **__environ;
    char **env = __environ;
    while(*env != NULL){
        printf("%s\n", *env);
        ++env;
    }
}


void handle_signal(int signal){ //doesn't currently do much, idk
    if(signal == SIGINT || signal == SIGTSTP || signal == SIGTERM){
        return;
    }
}

void disable_ctrl_printing(){
    signal(SIGINT, handle_signal);
    signal(SIGTSTP,handle_signal);
    signal(SIGTERM,handle_signal);
}

void reset_terminal_settings(){
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP,SIG_DFL);
    signal(SIGTERM,SIG_DFL);
}


int handle_exit_logic(token_node *cmd_head, path_node **path, char **previous_directory, char **current_directory, char** prompt_prefix, FILE* input, int ret_code){
    printf("Exiting shell with code %d\n", ret_code);
    free_all_mallocs(cmd_head, path, previous_directory, current_directory, prompt_prefix,input);
    exit(ret_code);
    return ret_code;
}

void read_directory(DIR* directory){
    struct dirent* entry;
    while ((entry = readdir(directory)))
    {
        printf("%s\n", entry->d_name);
    }
}

int open_directory_and_read(char* target_dir, bool mult){
    DIR* directory = opendir(target_dir);
    if(directory == NULL){
        perror("Error opening directory");
        return 1;
    }
    if(mult){
        printf("%s:\n",target_dir);
    }
    read_directory(directory);
    closedir(directory);
    return 0;
}

