
#include "second_pass.h"
static int IC = 100;

/**
 * handeling the second pass of the assembler program
 * @param file_info struct to hold all the needed data to handle the input file
 */
void second_pass(fileInformation *file_info){
    file_info->line_num = 0;
    while(!feof(file_info->file)){
        bool there_is_label;
        data_or_string_or_ext_or_ent status; /*gets what kind of label it is */
        before_and_after *before_and_after_struct;/*struct to be used with the splitLine function */
        memset(file_info->line_content, '\0',LINE_LENGTH);
        fgets(file_info->line_content,LINE_LENGTH,file_info->file );
        before_and_after_struct = malloc(sizeof(before_and_after));
        file_info->line_num+=1;/*after validating line size were increasing the line counter */

        strcpy(file_info->line_content,trim(file_info->line_content));
        if(*file_info->line_content == '\0'|| *file_info->line_content == ';')
            continue;
        /*value will hold if there is label */
        there_is_label = return_if_label_exist_on_the_first_place(file_info->line_content,before_and_after_struct);

        if(there_is_label){
        strcpy(file_info->line_content,trim(before_and_after_struct->after));

        }

        strcpy(file_info->line_content,trim(file_info->line_content));
        /*checking if there is instruction command */
        status = return_data_or_string_or_ext_or_ent(file_info->line_content, before_and_after_struct, file_info);
        /*getting rid of white spaces */
        strcpy(before_and_after_struct->before,trim(before_and_after_struct->before));
        strcpy(before_and_after_struct->after,trim(before_and_after_struct->after));

        /*data and string has been dealt in the first pass */
        if(status == DOT_DATA || status == DOT_STRING)
            continue;
    /*if were dealing with extern we will update the value in all the places that the label has been used */
        else if(status == DOT_EXTERN) {
            int address;
            address = return_address_by_label(file_info,before_and_after_struct->after);
            update_extern_name(file_info->code_head,address,before_and_after_struct->after);
        }

        else if(status == DOT_ENTRY){
            if(update_entry(file_info->symbol_head,before_and_after_struct->after,file_info)== FALSE){
                printf("Symbol is not defined on line %d\n",file_info->line_num);
                file_info->status = TRUE;
                continue;
            }

            if(update_entry(file_info->symbol_head,before_and_after_struct->after, file_info) == FALSE) {
                file_info->status = TRUE;
                continue;
            }
        }
        else
            handle_inst_second_pass(file_info, before_and_after_struct);
    }
}

void handle_inst_second_pass(fileInformation *fileInformation1, before_and_after  * before_and_after_struct){
    char operation[MAX_LINE_LENGTH];
    char op1[MAX_LINE_LENGTH];
    char op2[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    int addressingType1;
    int addressingType2;
    int newAddress;
    word code_word;
    int L;
    strcpy(line,fileInformation1->line_content);
    L = 0;

    split_line(line,"\n\t ",before_and_after_struct);
    strcpy(operation,trim(before_and_after_struct->before));
    get_opcode_func(operation,&code_word);

    split_line(line," \t",before_and_after_struct);
    strcpy(line,trim(before_and_after_struct->after));
    IC++;

    if(code_word.WORD_OP_CODE < 5) {
        L = 2;
        split_line(line, ",", before_and_after_struct);
        strcpy(op1, trim(before_and_after_struct->before));
        strcpy(op2, trim(before_and_after_struct->after));
    }
    else if (code_word.WORD_OP_CODE<=13){
        L = 1;
        strcpy(op2, trim(before_and_after_struct->after));
    } else {
        return;
    }

    addressingType1 = get_addressing_type(op1,fileInformation1);
    addressingType2 = get_addressing_type(op2,fileInformation1);

    if(addressingType1 == 1){
        newAddress = return_address_by_label(fileInformation1, op1);

        if(newAddress != -1 ) {
            update_value(fileInformation1, newAddress, IC);
        } else {
            printf("error, symbol in use as first operand is not defined on line %d\n", fileInformation1->line_num);
            fileInformation1->status = TRUE;
            return;
        }
    }


    else if(addressingType1 == 2){
        strcpy(op2,trim(op2+1));
        newAddress = return_address_by_label(fileInformation1, op2);
        if(return_data_or_string_or_ext_or_ent(op2, before_and_after_struct, fileInformation1) == DOT_EXTERN){
            printf("error - symbol is defined as extern, cannot branch to extern on line %d\n",fileInformation1->line_num);
            fileInformation1->status = TRUE;

    }
        update_relative_value(fileInformation1, newAddress, IC);
    }
    if(L == 2)
        IC++;

    if(addressingType2 == 1){
        newAddress = return_address_by_label(fileInformation1, op2);

        if(newAddress != -1) {
            update_value(fileInformation1, newAddress,IC);
        }
        else{
            printf("error, symbol in use as first operand is not on the list on line %d\n ", fileInformation1->line_num);
            fileInformation1->status = TRUE;
        }

    }

    else if (addressingType2 == 2){
        if(op2[0]=='%')
            memmove(op2,op2+1,strlen(op2));
        newAddress = return_address_by_label(fileInformation1, op2);
        if(return_data_or_string_or_ext_or_ent(op1, before_and_after_struct, fileInformation1) == DOT_EXTERN){
            printf("error - symbol is defined as extern on line %d\n",fileInformation1->line_num);
            fileInformation1->status = TRUE;
        }
        update_relative_value(fileInformation1, newAddress, IC);

    }
    IC++;
}