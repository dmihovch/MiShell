#include "../include/shloop.h"

int shell_loop(FILE *input)
{




    if(input != stdin){

    }







    token_node *head;

    // return codes for functions
    int return_code = 0;

    // collecting prompt
    char *cmd_raw;
    size_t cmd_len;
    ssize_t num_read;

    path_node *path = get_path();

    char* previous_directory = getcwd(NULL, 0);
    char* current_directory = getcwd(NULL,0);
    if (previous_directory == NULL)
    {
        printf("Previous directory in shloop getcwd failed\n");
        return 1;
    }
    if(current_directory == NULL){
        printf("Current directory in shloop getcwd failed\n");
        return 1;
    }

    char* prompt_prefix = NULL;
    char* prompt; //want to change this
    // MUST FIGURE THIS OuT
    int cmd_found_and_exec = 0;
    // print_path_debug(path);
    while (1)
    {

        
        cmd_raw = NULL;
        cmd_len = 0;
        num_read = 0;

        /*

                        todo -
                        figure out how to do scrolling with arrow keys (cmds)
                        add null checks for each malloc and strdup
                        at end see if I can remove the path arg from the builtins


                        PREV DIRECTRY STUFF!

        */

        // reads in prompt and gets rid of newline

        if(input == stdin){
            prompt = malloc(256); // need this to be dynamic

            if(prompt_prefix == NULL){
                sprintf(prompt,"%s $ ",current_directory);
            }
            if(prompt_prefix != NULL){
                sprintf(prompt, "%s@%s $ ",prompt_prefix,current_directory);
            }
            printf("%s",prompt);
            free(prompt);
        }

        

        num_read = get_input(&cmd_raw,&cmd_len, input);
        if(num_read == 1){
            continue;
        }

        if(num_read == -1 && input == stdin){
            continue;
        }
        
        if(num_read == -1 && input != stdin){
            free(previous_directory);
            free(current_directory);
            free_path(&path);
            //free(cmd_raw);
            return return_code;
        }

        head = tokenizer(cmd_raw);
        free(cmd_raw);
        if (head == NULL)
        {
            continue;
        }

        glob_handling(&head);
        
        cmd_found_and_exec = 0;
        return_code = check_builtin(head, &path, &previous_directory, &current_directory, &prompt_prefix, input, &cmd_found_and_exec);
        
        if(cmd_found_and_exec == 0){
            return_code = exec_abs_rel_path(head,&cmd_found_and_exec);
        }
        if(cmd_found_and_exec == 0){
            return_code = exec_cmd_with_path(head,path,&cmd_found_and_exec);
        }

        if(cmd_found_and_exec == 0){
            printf("%s: Command not found.\n",head->token);
        }
        // check for return codes, to see what is next

        free_tokens(head);
    }

    free_all_mallocs(head, &path, &previous_directory, &current_directory, &prompt_prefix, input); // just in case
    return return_code;
}