#include "first_pass.h"




/**
 * handeling the whole first part of the proccess
 * @param file_info A pointer to a struct that hold needed data to handle to file
 * @return Returns is proccess has succssfuly completed
 */
void first_pass(fileInformation *file_info) {
    before_and_after *before_and_after_struct; /**this struct hold 2 strings. will help us with the split function */
    file_info->IC = IC_INIT_VALUE; /** initializing IC to 100 */
    file_info->DC = 0;/** initializing DC to 0*/
    before_and_after_struct = (before_and_after*) malloc(sizeof(before_and_after));
    file_info->line_num = 0;

    while(!feof(file_info->file)){
        bool there_is_label;
        data_or_string_or_ext_or_ent status; /*gets what kind of label it is */
        char label_name[MAXIMAL_LABEL_SIZE];
        char currText[MAX_LINE_LENGTH] = {0};
        memset(file_info->line_content, '\0',LINE_LENGTH);
        fgets(file_info->line_content,LINE_LENGTH,file_info->file );
        file_info->line_num++;


        /* comments handeling */
        strcpy(file_info->line_content,trim(file_info->line_content));
        strcpy(currText, file_info->line_content);
        if(*file_info->line_content == '\0'|| *file_info->line_content == ';')
            continue;

       /* stage 3-4 */
       there_is_label = return_if_label_exist_on_the_first_place(file_info->line_content,before_and_after_struct);
        if(strlen(before_and_after_struct->before) > MAXIMAL_LABEL_SIZE){
            printf("Label size is too big on line %d\n",file_info->line_num++);
            file_info->status = TRUE;
            continue;

        }
        /* copy the first part of the line in case its a label */
        strcpy(label_name,trim(before_and_after_struct->before));

        if(there_is_label == FALSE){
            /*status gets what kind of instruction there is if there's any */
            status = return_data_or_string_or_ext_or_ent(before_and_after_struct->before, before_and_after_struct,file_info);
            strcpy(currText, before_and_after_struct->before);
        }
        else { /*the is a label */
            status = return_data_or_string_or_ext_or_ent(before_and_after_struct->after, before_and_after_struct, file_info);
            strcpy(currText, before_and_after_struct->after);
        }
        /*stage 5-6 */

        if(status == DOT_DATA || status == DOT_STRING) {
            if(there_is_label) {
                add_to_Symbol_list(&file_info->symbol_head, file_info->DC, DATA_TYPE, label_name);
            }
            /* dealing with the data operands if there is any */
            enter_data_operands_to_list(before_and_after_struct,file_info,status);
            continue; /* to break the loop */

        } else if(status == DOT_ENTRY){
            /*dealing with entry in the second pass */
           continue;
       } else if(status == DOT_EXTERN) { /* TODO is extern with no value is an error? */
            split_line(file_info->line_content," \t",before_and_after_struct);
            add_to_Symbol_list(&file_info->symbol_head,0,EXTERN_TYPE,trim(before_and_after_struct->after));
        } else{

            /*STAGE 11 */
            if(there_is_label){
                add_to_Symbol_list(&file_info->symbol_head,file_info->IC,CODE_TYPE,label_name);
            }
            analyzeInstruction(file_info, before_and_after_struct);
        }

    }

    updateAddresses(*file_info);
}



/**
 * updating the addresses acording to the synthax that has been given in the book
 * @param fileInformation holds needed data to handle the whole inputfile
 */
void updateAddresses(fileInformation fileInformation){
    /*setting pointers to the heads of the lists */
    DataNodePointer dataPtr = fileInformation.data_head;
    SymbolNodePointer SymbolPtr = fileInformation.symbol_head;

    while(dataPtr != NULL){
        /*increasing the memory address so it will match to the IC */
        dataPtr->memory_address +=fileInformation.IC;
        dataPtr = dataPtr->next;
    }

    while(SymbolPtr != NULL){
        if(SymbolPtr->type == DATA_TYPE){
            SymbolPtr->memory_address += fileInformation.IC;
        }
        SymbolPtr = SymbolPtr->next;
    }

}

