#include "../include/builtins.h"

int check_builtin(token_node *cmd_head, path_node **path, char **previous_directory, char** current_directory, char** prompt_prefix, FILE* input)
{
    // printf("Entering builtin\n");
    if (cmd_head != NULL && cmd_head->token != NULL)
    {
        int ret_code;
        // printf("%s\n",cmd_head->token);
        char *cmd = cmd_head->token;
        if (strcmp(cmd, "exit") == 0)
        {
            exit_cmd(cmd_head, path, previous_directory, current_directory, prompt_prefix, input);
        }
        if (strcmp(cmd, "which") == 0)
        {
            ret_code = which_cmd(cmd_head, *path);
        }
        if (strcmp(cmd, "list") == 0)
        {
            ret_code = list_cmd(cmd_head, *path);
        }
        if (strcmp(cmd, "pwd") == 0)
        {

            ret_code = pwd_cmd();
        }
        if (strcmp(cmd, "cd") == 0)
        {
            //printf("previous directory in builtin: %s\n", *previous_directory);
            ret_code = cd_cmd(cmd_head, previous_directory, current_directory);
            //printf("previous directory in builtin after cd cmd: %s\n", *previous_directory);
        }
        if((strcmp(cmd, "prompt") == 0) && input == stdin){
            ret_code = prompt_cmd(cmd_head, prompt_prefix);
        }
        if(strcmp(cmd, "pid")== 0){
            pid_cmd();
        }
        if(strcmp(cmd, "printenv") == 0){
            ret_code = printenv_cmd(cmd_head);
        }
        if(strcmp(cmd, "setenv") == 0){
            ret_code = setenv_cmd(cmd_head,path);
        }
    }
    else
    {
        printf("cmd_head || cmd_head->token == NULL\n");
        return 0;
    }

    return 0; // return codes?
}

// should be good
void exit_cmd(token_node *cmd_head, path_node **path, char **previous_directory, char **current_directory, char** prompt_prefix, FILE* input)
{
    token_node *arg_node = cmd_head->next;
    int ret_code;
    if (arg_node != NULL && arg_node->token != NULL)
    {
        ret_code = atoi(arg_node->token);
        printf("Exiting shell with code %d\n", ret_code);
        free_all_mallocs(cmd_head, path, previous_directory, current_directory, prompt_prefix,input);
        exit(ret_code);
    }
    free_all_mallocs(cmd_head, path, previous_directory, current_directory, prompt_prefix,input);
    printf("Exiting shell with code 0, token is NULL\n"); // delete after 0 for sub
    exit(0);
}
int which_cmd(token_node *cmd_head, path_node *path)
{
    const char *builtin_commands[] = {"exit", "which", "list", "pwd", "cd", "pwd", "pid", "prompt"};
    token_node *arg_node = cmd_head->next;
    if (arg_node == NULL)
    {
        return 1;
    }
    bool found_cmd;
    while (arg_node != NULL && arg_node->token != NULL)
    {
        found_cmd = false;

        // finding builtins
        for (int i = 0; i < BUILTIN_COMMANDS_ARR_LENGTH; i++)
        {
            if (strcmp(arg_node->token, builtin_commands[i]) == 0)
            {
                printf("%s: shell built-in command\n", arg_node->token);
                found_cmd = true;
                break;
            }
        }
        if (found_cmd)
        {
            arg_node = arg_node->next;
            continue;
        }

        // finding through absolute paths

        // need to actually do the whole checking of the whether the command
        if (access(arg_node->token, X_OK) == 0)
        {
            printf("%s\n", arg_node->token);
            arg_node = arg_node->next;
            continue;
        }

        // finding through PATH var
        // printf("Addr of arg is which_cmd: {%p}\n",&arg_node->token);
        char *cmd_in_path_var = find_cmd_with_path(path, arg_node->token);
        if (cmd_in_path_var != NULL)
        {
            printf("%s\n", cmd_in_path_var);
            free(cmd_in_path_var);
        }
        else
        {
            printf("%s not found\n", arg_node->token);
        }

        arg_node = arg_node->next;
    }
    return 0;
}

int list_cmd(token_node *cmd_head, path_node *path)
{
    token_node *arg = cmd_head->next;
    struct dirent *entry;
    DIR *directory_path;
    if (arg == NULL)
    {
        directory_path = opendir(".");
        if (directory_path == NULL)
        {
            perror("que?"); // lol, this can't ever fail, right?
            return 1;
        }

        while ((entry = readdir(directory_path)))
        {
            printf("%s\n", entry->d_name);
        }
    }
    char *directory_target;
    bool only_one = true;
    while (arg != NULL && arg->token != NULL)
    {

        directory_target = arg->token;
        directory_path = opendir(directory_target);
        if (directory_path == NULL)
        {
            perror("Error opening file");
            if ((arg = arg->next) != NULL)
            {
                printf("\n");
                continue;
            }
            break;
        }

        entry = readdir(directory_path);

        printf("%s:\n", directory_target);

        while (entry != NULL)
        {
            printf("%s\n", entry->d_name);
            entry = readdir(directory_path);
        }
        if (arg->next != NULL)
        {
            printf("\n");
        }

        arg = arg->next;
    }
    if(directory_path != NULL){
        closedir(directory_path);
    }
    return 0;
}

int pwd_cmd()
{
    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        return 1;
    }

    printf("CWD: %s\n", cwd);
    free(cwd);
    return 0;
}

int cd_cmd(token_node *cmd_head, char **prev_directory, char **current_directory)
{

    char* tmp_current_directory = strdup(*current_directory);

    //printf("Directory was: %s\n", getcwd(NULL, 0));
    int ret_code;

    if (cmd_head->next == NULL)
    {

        char *home = getenv("HOME");
        if (home == NULL)
        {
            printf("HOME is NULL\n");
        }
        ret_code = chdir(home);

        if (ret_code != 0)
        {
            perror("Failed to CD to ~");
            free(tmp_current_directory);
            return 1;
        }


    }

    if (cmd_head->next != NULL && cmd_head->next->token != NULL)
    {
        if (strcmp(cmd_head->next->token, "-") == 0)
        {
            ret_code = chdir(*prev_directory);
            if (ret_code != 0)
            {
                perror("Failed to CD to prev directory");
                free(tmp_current_directory);
                return 1;
            }
        }
        else
        {
            ret_code = chdir(cmd_head->next->token);
            if (ret_code != 0)
            {
                printf("Unable to CD into: %s\n", cmd_head->next->token);
                free(tmp_current_directory);
                return 1;
            }
        }
    }

    //successful cd
    reassign_current_and_previous_directory(current_directory,prev_directory,&tmp_current_directory);
    


    //need to implement prev-directory stuff

    return 0;
}

int prompt_cmd(token_node* cmd_head, char** prompt_prefix){
    if(*prompt_prefix != NULL){
        free(*prompt_prefix);
    }
    if(cmd_head->next == NULL){
        size_t prompt_len = 0;
        printf("Enter new prompt prefix: ");
        ssize_t num_read = get_input(prompt_prefix,&prompt_len,stdin);
        if(num_read == 1 || *prompt_prefix == NULL){
            return 1;
        }
        return 0;
    }
    if(cmd_head->next!=NULL && cmd_head->next->token != NULL){
        *prompt_prefix = strdup(cmd_head->next->token);
        return 0;
    }
    printf("Something went wrong in prompt_cmd\n\n");
    return 1;
}

void pid_cmd(){
    printf("Current Process ID: %d\n", getpid());
}

int printenv_cmd(token_node* cmd_head){
    if(cmd_head->next == NULL){
        print_whole_environment();
        return 0;
    }
    token_node* arg = cmd_head->next;
    while(arg != NULL && arg->token != NULL){
        char* environment_variable = getenv(arg->token);
        if(environment_variable != NULL){
            printf("%s=%s\n",arg->token,environment_variable);
        }
        else{
            printf("No variable: %s\n", arg->token);
        }
        arg=arg->next;
    }
    return 0;
}

int setenv_cmd(token_node* cmd_head, path_node** path){
    if(cmd_head->next == NULL){
        print_whole_environment();
        return 0;
    }
    if(cmd_head->next != NULL && cmd_head->next->token != NULL && cmd_head->next->next == NULL){
        if(strcmp(cmd_head->next->token,"PATH") == 0){
            free_path(path);
        }
        if(setenv(cmd_head->next->token,"",1)==0){
            printf("%s is set to: [%s]\n", cmd_head->next->token,getenv(cmd_head->next->token));
            if(*path == NULL){
                *path = get_path();
            }
            return 0;
        }
        perror("setenv failure");
        return 1;
    }
    token_node* env_var = cmd_head->next;
    if(env_var->token != NULL && env_var->next != NULL && env_var->next->token != NULL && env_var->next->next == NULL){
        if(strcmp(env_var->token,"PATH")==0){
            free_path(path);
        }
        if(setenv(env_var->token,env_var->next->token,1)==0){
            printf("%s is set to: [%s]\n", env_var->token, getenv(env_var->token));
            if(*path == NULL){
                *path = get_path();
            }
            return 0;
        }
        perror("setenv failure");
        return 1;
    }
    fprintf(stderr, "Error: Too many arguments passed to setenv\n");
    return 1;
    
}