#include "lists.h"

/**
 * adding word structs to data list or code list
 * @param fileInformation1 struct that holds the input file details
 * @param head the head of the list we want to add to
 * @param word_to_add the word that we want to add to the list
 * @param address the address of the word
 * @param status if is a first word in the code line or not
 */
void add_to_list(fileInformation *fileInformation1,DataNodePointer *head,word word_to_add,int address,FirstOrSecondary status){
    DataNodePointer temp;
    DataNodePointer new_data_word;/*the word that we are going to add */
    new_data_word = (DataNodePointer )malloc(sizeof (struct DataNode));
    /*checking if malloc worked */
    if(new_data_word == NULL){
        printf("Malloc critic error - exit program");
        exit(1);

    }
    /*setting all of the values inside the word that we are going to add */
    new_data_word->memory_address = address;
    new_data_word->word_in_node = word_to_add;
    strcpy(new_data_word->word_in_node.labelName,word_to_add.labelName);
    new_data_word->line = fileInformation1->line_num;
    new_data_word->status = status;
    new_data_word->next = NULL;


    temp = *head;

    if(temp == NULL){
        *head = new_data_word;
        return;
    }


    while(temp->next != NULL)
        temp = temp->next;

    temp->next = new_data_word;
}

word getWord(int address,DataNodePointer head){
    word *word_to_return = NULL ;
    DataNodePointer temp;
    temp = head;

    while(temp != NULL){
        if(temp->memory_address == address)
            return temp->word_in_node;


        temp = temp->next;
    }
    printf("error in getting the word");
    return *word_to_return ;

}

void freeList(DataNodePointer head){
    DataNodePointer prev;
    if(head == NULL)
        return;


    prev = head;


    while(head->next != NULL){
        head = head->next;
        free(prev);
        prev = head->next;

    }

    free(head);
}

void update_are(DataNodePointer head, int address,char ARE) {
    DataNodePointer temp;
    temp = head;
    while(temp != NULL) {
        if(temp->memory_address == address)
            temp->word_in_node.ARE = ARE;
        temp = temp->next;
    }
}
/**
 * updating the values of label operands that has delcared as extern
 * @param head the head of the list we want to update
 * @param address the address we want to update
 * @param s the label name
 */
void update_extern_name(DataNodePointer head,int address,char *s){
    DataNodePointer temp;
    temp = head;
    while(temp != NULL) {
        if(strcmp(temp->word_in_node.labelName, s) == 0) {
            temp->word_in_node.ARE = 'E';
            temp->word_in_node.value = 0;
        }
        temp = temp->next;
    }
}
