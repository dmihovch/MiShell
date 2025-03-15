#include "../include/shloop.h"

int main(int argc, char **argv)
{

    
    
    disable_ctrl_printing();



    //FOR CHILD PROCESSES, NEED TO USE signal(SIGINT, SIG_DFL); etc so that ctrl + c/z/etc are no longer ignored
    //call reset_terminal_settings()


    FILE *input_method = stdin;
    

    if(argc > 1){
        input_method = fopen(argv[1], "r");
        if(input_method == NULL){
            perror("Error opening file");
            return 1;
        }
    }

    int last_exit_code = shell_loop(input_method);
    
    if(input_method != stdin){
        fclose(input_method);
    }

    printf("Exiting shell with code %d\n", last_exit_code);
    exit(last_exit_code);
    return last_exit_code;
}
