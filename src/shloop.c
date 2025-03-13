#include "../include/shloop.h"

int shell_loop()
{

    token_node *head;

    // return codes for functions
    int return_code;

    // collecting prompt
    char *cmd_raw;
    size_t cmd_len;
    ssize_t num_read;

    path_node *path = get_path();

    char *previous_directory = get_cwd();
    // MUST FIGURE THIS OuT

    // print_path_debug(path);

    while (1)
    {

        return_code = 0;
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
        printf("$ ");
        num_read = getline(&cmd_raw, &cmd_len, stdin);
        if (num_read == 1)
        { // handles just hitting enter
            free(cmd_raw);
            continue;
        }
        if (num_read != -1)
        { // replaces newline with null terminator
            if (cmd_raw[num_read - 1] == '\n')
            {
                cmd_raw[num_read - 1] = '\0';
            }
        }

        head = tokenizer(cmd_raw);
        free(cmd_raw);
        if (head == NULL)
        {
            continue;
        }
        // print_tokens_debug(head);
        return_code = check_builtin(head, path, previous_directory);
        // check for return codes, to see what is next

        free_tokens(head);
    }

    free_all_mallocs(head, path, previous_directory); // just in case

    printf("\n\n\n\n\nTHIS SHOULD NEVER? PRINT!!\n\n\n\n\n");
    return 0;
}