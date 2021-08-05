
#ifndef _TEXTANALYZE_H
#define _TEXTANALYZE_H
#include "base.h"
#include "ctype.h"
#include "lists.h"
#include "symbolTable.h"
char *trim(char *str);
bool is_reserved_word(char * str);
void handle_inst_second_pass(fileInformation *fileInformation1, before_and_after  * before_and_after_struct);
void analyzeInstruction(fileInformation *file_info, before_and_after *before_and_after_struct);
bool is_label_valid(char * str);
bool is_alpha_or_number(char * str);
int register_name_to_bin(char * str);
bool operands_addressing_valid(word *code_word);
addressing_type get_addressing_type(char *operand, fileInformation *fileInformation1);
void get_opcode_func(char *command, word *word1);
bool isNumber(char * s, fileInformation *fileInformation1);
bool isLastCharQuote (char * str);
void enter_data_operands_to_list(before_and_after *before_and_after_struct, fileInformation *fileInformation1, data_or_string_or_ext_or_ent status);
bool split_line(char* line_content,char * delimeter, before_and_after *before_after_struct) ;


bool return_if_label_exist_on_the_first_place(char * line_content,before_and_after *before_and_after_struct);

data_or_string_or_ext_or_ent
return_data_or_string_or_ext_or_ent(char *line_content, before_and_after *before_and_after_struct,
                                    fileInformation *file_info);


#endif
