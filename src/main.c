#include "../include/shloop.h"

int main(int argc, char **argv)
{

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



    return 0;
}
