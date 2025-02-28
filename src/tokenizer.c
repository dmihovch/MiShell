#include "../include/tokenizer.h"

token_node* tokenizer(char* cmd_raw){


    //init the head
    char* tok = strtok(cmd_raw, " ");
    token_node* head = (token_node*) malloc(sizeof(token_node));
    head->token = tok;
    head->next = NULL;
    head->prev = NULL;
    
    token_node* cur = head;
    while(tok!=NULL){
        tok = strtok(NULL, " ");
        //printf("\n\n Heading into the dangerzone\n\n");
        cur -> next = (token_node*) malloc(sizeof(token_node));
        cur -> next -> prev = cur;
        cur -> next -> next = NULL;
        cur -> next -> token = tok;
        cur = cur -> next;
        //printf("\n\n Heading out of the dangerzone\n\n");
    }

    return head;
}

void free_tokens(token_node* head){
    //certified memory good?
    token_node* cur = head;
    token_node* tmp = NULL;
    while(cur!=NULL){
        tmp = cur;
        cur = cur -> next;
        //printf("Address of tmp: %p\nAddress of cur: %p\n",tmp,cur);
        free(tmp);
    }
    
}

void print_tokens_debug(token_node* head){
    while(head!=NULL){
        if(head->token == NULL){
            return;
        }
        printf("{%s}\n", head->token);
        head = head->next;
    }
}
