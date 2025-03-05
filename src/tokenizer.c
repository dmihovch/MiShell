#include "../include/tokenizer.h"

token_node* tokenizer(char* cmd_raw){

    if(cmd_raw != NULL){
        char* tok = strtok(cmd_raw, " ");
        if(tok == NULL){
            return NULL;
        }


        token_node* head = (token_node*) malloc(sizeof(token_node));
        head->token = strdup(tok);
        head->next = NULL;
        head->prev = NULL;

        //printf("Init head fine \n");
        
        token_node* cur = head;
        tok = strtok(NULL, " ");
        while(tok!=NULL){
            
            //printf("\n\n Heading into the dangerzone\n\n");
            cur -> next = (token_node*) malloc(sizeof(token_node));
            cur -> next -> prev = cur;
            cur -> next -> next = NULL;
            cur -> next -> token = strdup(tok);
            cur = cur -> next;
            tok = strtok(NULL, " ");
            //printf("Not expecting to see this\n");
            //printf("\n\n Heading out of the dangerzone\n\n");
        }
        //printf("Returning tokenizer\n");
        return head;
    }
    return NULL;
}

void free_tokens(token_node* head){
    //certified memory good?
    token_node* cur = head;
    token_node* tmp = NULL;
    while(cur!=NULL){
        tmp = cur;
        cur = cur -> next;
        //printf("Address of tmp: %p\nAddress of cur: %p\n",tmp,cur);
        if(tmp->token != NULL){
            free(tmp->token);
        }
        free(tmp);
    }
    
}

void print_tokens_debug(token_node* head){
    //printf("printing...\n");
    while(head!=NULL){
        if(head->token == NULL){
            return;
        }
        printf("{%s} ::: {%p}\n", head->token, &head->token);
        head = head->next;
    }
    //printf("Printed!\n");
}
