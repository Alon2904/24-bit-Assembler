

#include "symbolTable.h"
/**
 * adding symbol to the list
 * @param head the first node in the list
 * @param address the IC of the symbol
 * @param type the type data/code/string/int
 */
void add_to_Symbol_list(SymbolNodePointer *head, int address, SYMBOL_TYPE type, char *labelName){
    SymbolNodePointer temp;
    SymbolNodePointer new_symbol;

    new_symbol = (SymbolNodePointer )malloc(sizeof (struct SymbolNode));
    if(new_symbol == NULL){
        printf("Malloc critic error - exit program");
        exit(1);

    }
    new_symbol->next = NULL;

    new_symbol->type = type;
    new_symbol->memory_address = address;
    strcpy(new_symbol->name,labelName);


    temp = *head;
    if(temp == NULL)
        *head = new_symbol;
    else {
        temp = *head;
        while(temp->next !=NULL)
            temp = temp->next;
        temp->next = new_symbol;
    }

}

bool update_entry(SymbolNodePointer head,char * str,fileInformation *fileInformation1){
        SymbolNodePointer temp;
        temp = head;
    while(temp != NULL){
        if(strcmp(temp->name,str )==0){
            if(temp->type == EXTERN_TYPE){
                printf("error ,symbol defined as extern and entry on line %d\n",fileInformation1->line_num);
                fileInformation1->status = TRUE;
                return FALSE;
            }
            temp->type = ENTRY_TYPE;
            return TRUE;

        }
        temp = temp->next;

    }
    return FALSE;
}


/* update the memory address of `memoryInCode` to `newAddress` (for example on labels second pass) */
void update_value(fileInformation *fileInformation1, int newAddress, int memoryInCode) {
    DataNodePointer codeHead;
    codeHead = fileInformation1->code_head;
    while(codeHead != NULL){
        if(codeHead->memory_address == memoryInCode) {
            codeHead->word_in_node.value = newAddress;
            return;
        }
        codeHead = codeHead->next;
    }
}

void update_relative_value(fileInformation *fileInformation1, int newAddress, int memoryInCode) {
    DataNodePointer codeHead;
    codeHead = fileInformation1->code_head;
    while(codeHead != NULL) {
        if(codeHead->memory_address == memoryInCode) {
            codeHead->word_in_node.value = newAddress - (memoryInCode);
        }
        codeHead = codeHead->next;
    }
}

int return_address_by_label(fileInformation *fileInformation1, char * op1){
    SymbolNodePointer temp;
    temp = fileInformation1->symbol_head;

    while(temp != NULL){
        if(strcmp(temp->name,op1) == 0)
            return temp->memory_address;
        temp = temp->next;
    }

    return -1;
}