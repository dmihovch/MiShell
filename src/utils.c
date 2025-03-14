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

void free_path(path_node *head)
{
    path_node *cur = head;
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
}

void print_path_debug(path_node *head)
{
    while (head != NULL && head->path != NULL)
    {
        printf("{%s}\n", head->path);
        head = head->next;
    }
}

void free_all_mallocs(token_node *cmd_head, path_node *path, char **previous_directory, char **current_directory, char** prompt_prefix)
{
    if (cmd_head != NULL)
    {
        free_tokens(cmd_head);
    }
    if (path != NULL)
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

int get_input(char** cmd_raw,size_t* cmd_len){
    int num_read = getline(cmd_raw, cmd_len, stdin);
    if (num_read == 1)
    { // handles just hitting enter
        free(*cmd_raw);
        *cmd_raw = NULL;
        return num_read;
    }
    if (num_read != -1)
    { // replaces newline with null terminator
        if ((*cmd_raw)[num_read - 1] == '\n')
        {
            (*cmd_raw)[num_read - 1] = '\0';
            --num_read;
        }
        return num_read;
    }
    return num_read;
}