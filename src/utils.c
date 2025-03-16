#include "../include/utils.h"
path_node *get_path()
{
    char *path_raw = getenv("PATH");
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

//idk man it prints ^C and ^Z, and if you do kill -1 <shell pid>, there are memory leaks! yay!
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


//I think I am truly a cooked cs student
void glob_handling(token_node** cmd_head){
    if(cmd_head == NULL || *cmd_head== NULL){
        return;
    }
    token_node* arg = *cmd_head;
    glob_t expanded;
    const int flags = GLOB_NOSORT | GLOB_MARK | GLOB_TILDE;

    while(arg!=NULL && arg->token!=NULL){
        if(glob(arg->token,flags,NULL,&expanded)==0){

            token_node* prev_arg = arg->prev;
            token_node* next_arg = arg->next;
            token_node* new_args = glob_tokenizer(expanded);
            if(new_args != NULL){
                token_node* arg_tmp = arg -> next;
                if(prev_arg != NULL){
                    prev_arg ->next = new_args;
                    new_args->prev = prev_arg;
                }
                if(prev_arg == NULL){
                    *cmd_head = new_args;
                }
                if(next_arg != NULL){
                    token_node* last_new_node = get_last_node(new_args);
                    
                    last_new_node -> next = next_arg;
                    next_arg -> prev = last_new_node;
                    
                    
                }
                free(arg->token);
                free(arg);
                arg = arg_tmp;
                globfree(&expanded);
                continue;
            }

            
        }
        else{
            globfree(&expanded);
            arg = arg->next;
            continue;
        }
        globfree(&expanded);
        arg=arg->next;
    }

}

token_node* glob_tokenizer(glob_t expanded){
    if(expanded.gl_pathc == 0){
        return NULL;
    }
    token_node* head;
    token_node* cur;
    for(int i = 0; i<expanded.gl_pathc; i++){
        if(i==0){
            head = (token_node*) calloc(1,sizeof(token_node));
            head->token = strdup(expanded.gl_pathv[i]);
            head->prev = NULL;
            head->next = NULL;
            cur = head;
        }
        else{
            cur -> next = (token_node*) calloc(1,sizeof(token_node));
            cur -> next ->prev = cur;
            cur->next ->next = NULL;
            cur -> next -> token = strdup(expanded.gl_pathv[i]);
            cur = cur -> next;
        }
    }
    return head;
}

token_node* get_last_node(token_node* head){
    token_node* last = head;
    while(last!= NULL && last->next != NULL){
        last=last->next;
    }
    return last;
}

char** create_argv_arr(token_node* arg){
    int argc = 0;
    token_node* counter = arg;
    while(counter != NULL && counter->token != NULL){
        ++argc;
        counter = counter->next;
    }

    char** argv = malloc((argc+1)*sizeof(char*));

    for(int i = 0; arg != NULL && arg->token != NULL; i++){
        argv[i] = strdup(arg->token);
        if(argv[i] == NULL){
            perror("strdup failed");
            for(int j = 0; j < i && argv[j] != NULL; j++){
                free(argv[j]);
            }
            free(argv);
            return NULL;
        }
        arg = arg->next;
    }
    argv[argc] = NULL;
    return argv;
}

void free_argv_arr(char** argv){
    for(int i = 0; argv[i] != NULL; i++){
        free(argv[i]);
    }
    free(argv);
}

int simple_cd(char* path,char **prev_directory, char **current_directory){
    char* tmp_current_directory = strdup(*current_directory);

    int ret_code;

    ret_code = chdir(path);
    if(ret_code!=0){
        free(tmp_current_directory);
        perror("chdir failed");
        return ret_code;
    }

    //successful cd
    reassign_current_and_previous_directory(current_directory,prev_directory,&tmp_current_directory);
    
    return ret_code;

}