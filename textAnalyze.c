
#include "textAnalyze.h"
/**
 * cutting any white spaces from both start and end of a string
 * @param str the input string
 * @return the trimmed string
 */
char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return ""; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( frontp != str && endp == frontp )
        *str = '\0';
    else if( str + len - 1 != endp )
        *(endp + 1) = '\0';

    /* shift the string so that it starts at str so that if its dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
        while( *frontp ) { *endp++ = *frontp++; }
        *endp = '\0';
    }

    return str;
}
/**
 * the function gets a delimeter as a parameter if it finds the delimeter it returns
 * true and putting the string before the delimeter in string called before
 * and the string after the parameter in a string called after
 * @param line_content input string
 * @param delimeter the char that we are looking for
 * @param before_after_struct a struct the contain 2 char arrays
 * the first to hold the string before delimeter and second the string after delimeter
 *
 * if delimeter hasnt been found, all string is copied to "before" char array
 * @return true if delimeter has been found or false if it hasnt
 */
bool split_line(char* line_content,char * delimeter, before_and_after *before_after_struct) {
    char *token;
    char temp[LINE_LENGTH];

    strcpy(temp, line_content);
    /*check if char array is empty */
    if (strcmp(temp, "") == 0)
        return FALSE;
/*cuts the char array according to the delimeter */
    token = strtok(temp, delimeter);
    memset(before_after_struct,'\0',sizeof(before_and_after));
    if (token != NULL && strcmp(line_content, token) == 0) {
        /* if delimeter is not found we will copy all of the string to "before" */
        strcpy(before_after_struct->after, "");
        strcpy(before_after_struct->before, temp);
        return FALSE;
    }
    /* else we will copy the strings as explained before */
    token = strtok(NULL,delimeter);
    strcpy(before_after_struct->before,temp);
    strcpy(before_after_struct->after,line_content + strlen(temp)+1);

    return TRUE;

}
/**
 * Checks if there is a label at the beggining of the line
 * @param line_content The line to analyze
 * @param before_and_after_struct The struct that needed to "splitLine" function
 * @return TRUE is label is first on the line or FALSE if its not
 */
bool return_if_label_exist_on_the_first_place(char * line_content,before_and_after *before_and_after_struct) {
    bool found; /* represent if label has been found */
    int i;
    strcpy(line_content,trim(line_content));
    /*if ':' has been found we will copy all the string that is before ':' */
    found = split_line(line_content,":",before_and_after_struct);
    for( i = 0;i < strlen(before_and_after_struct->before);i++){
        /*validate that it is not a string */
        if(before_and_after_struct->before[i]== '"')
            return FALSE;

    }
    if(found)
        return TRUE;
    return FALSE;
}

/**
 * Checks wether any kind of instruction command is on line
 * @param line_content The line to be analyzed
 * @param before_and_after_struct Needed struct for splitLine function
 * @param file_info Pointer to a struct that hold all of the file information
 * @return Enum that specify what kind of instruction in written in the line
 */
data_or_string_or_ext_or_ent
return_data_or_string_or_ext_or_ent(char *line_content, before_and_after *before_and_after_struct,
                                    fileInformation *file_info) {
    char * temp;
    data_or_string_or_ext_or_ent toRet = NONE; /* setting variable to NONE to avoid mistakes or grabge values */
    temp = malloc(LINE_LENGTH);
    /* getting rid of any uneccesery white spaces */
    strcpy(temp,trim(line_content));

    /* we know that if there is any kind of instruction in the line it should be at the beggining of line content there for
     * we will use split_line and set space as a delimeter */
    split_line(temp," \t\n",before_and_after_struct);

    /*we will compare "toRet to any kind of instruction to find the right one */
    if(strcmp(before_and_after_struct->before,".data") == 0)
        toRet = DOT_DATA;

    if(strcmp(before_and_after_struct->before,".string") == 0)
        toRet = DOT_STRING;

    if(strcmp(before_and_after_struct->before,".extern") == 0) {
        file_info->hasExt = TRUE;
        toRet = DOT_EXTERN;
    }

    if(strcmp(before_and_after_struct->before,".entry") == 0) {
        file_info->hasEnt = TRUE;
        toRet = DOT_ENTRY;
    }

    free(temp);
    return toRet;
}

/**
 * analyzing a command with data operands after .string or .data call and adding them to the data list
 * @param before_and_after_struct struct for split_line function. the struct already hold in "after" the
 * @param fileInformation1 pointer to a struct that holds all the information to handle the file
 * @param status enum represent what kind of data were handeling
 */
void enter_data_operands_to_list(before_and_after *before_and_after_struct, fileInformation *fileInformation1, data_or_string_or_ext_or_ent status){
    bool b; /* will hold value if were handeling one number or more */
    char *temp;
    int i; /* will be used as a index for a loop*/
    int x;
    word data_word_to_add;
    temp = (char*) malloc(sizeof(before_and_after_struct->after));
    strcpy(before_and_after_struct->after,trim(before_and_after_struct->after));
    strcpy(temp,before_and_after_struct->after);
    if(status == DOT_DATA){
        /*if were handling numbers we will check wether there are few of them by searching for ' , ' */
        b = split_line(temp,",",before_and_after_struct);

    if(b == FALSE){ /*covers the case that there is one operand without comas */

        /*checking if input is valid*/
        strcpy(before_and_after_struct->before,trim(before_and_after_struct->before));
        for(i=0;i<strlen(before_and_after_struct->before);i++) {
            if (isspace(before_and_after_struct->before[i])) {
                printf("input invalid on line %d\n", fileInformation1->line_num);
                fileInformation1->status = TRUE;
                return;
            }
        }

         /*creating a word struct to hold values and add to the list */
        if(!isNumber(before_and_after_struct->before,fileInformation1))
            return;
        x = atoi(before_and_after_struct->before); /*converting to int */
        /*checks if number is too big for 12 bits*/
        if(x>MAXIMAL_INT_VALUE||x<MINIAL_INT_VALUE){
            printf("number is out of boundries on line %d\n",fileInformation1->line_num);
            fileInformation1->status = TRUE;
            return;
        }

        data_word_to_add.value = x;
        data_word_to_add.WORD_T = DATA_WORD;
        data_word_to_add.ARE = 'A';
        add_to_list(fileInformation1,&fileInformation1->data_head,data_word_to_add,fileInformation1->DC++,SECONDARY_WORD);

        return;
        /*in case there is ' , ' and no operand after */
    } else if(strcmp(trim(before_and_after_struct->after),"") == 0) {
        printf("Operand missing line %d\n", fileInformation1->line_num);
        fileInformation1->status = TRUE;
    }

    while(b){ /*if there is more than 1 data object to add */
        int x;
        word data_word_to_add;
       if(!isNumber(before_and_after_struct->before,fileInformation1)){
           printf("invalid input on line %d\n",fileInformation1->line_num);
           fileInformation1->status = TRUE;
           return;}
       x = atoi(before_and_after_struct->before);

       data_word_to_add.value = x;
       data_word_to_add.WORD_T = DATA_WORD;
       data_word_to_add.ARE = 'A';
       add_to_list(fileInformation1,&fileInformation1->data_head,data_word_to_add,fileInformation1->DC++,SECONDARY_WORD);

       strcpy(temp,before_and_after_struct->after);
       b = split_line(temp,",",before_and_after_struct);
    }

        if(strcmp(before_and_after_struct->before,"")==0){
            printf("Missing operand line %d",fileInformation1->line_num);
            fileInformation1->status = TRUE;
        } else {
            int x;
            word data_word_to_add;
            if(!isNumber(before_and_after_struct->before,fileInformation1))
                return;
            x = atoi(before_and_after_struct->before);
            data_word_to_add.value = x;
            data_word_to_add.WORD_T = DATA_WORD;
            data_word_to_add.ARE = 'A';
            add_to_list(fileInformation1,&fileInformation1->data_head,data_word_to_add,fileInformation1->DC++,SECONDARY_WORD);
        }

    }


    else { /*the data is a string */
        int i;
        word data_word_to_add;
        strcpy(temp,trim(temp));

        /*validation if the last char is quote */
        if (!isLastCharQuote(temp)) {
            printf("missing upper quote on line %d\n", fileInformation1->line_num);
            fileInformation1->status = TRUE;
            return;
        }

        /* getting rid of the left quote */
        if(*temp != '"'){
            printf("invalid string - text before quotes in line %d",fileInformation1->line_num);
            fileInformation1->status = TRUE;
            return;
        }
        strcpy(temp,temp+1);


        /* getting rid of the right quote */
        temp[strlen(temp)-1] = '\0';

        strcpy(temp,trim(temp));
         /* string is valid therefore we will add it to data image */
            for(i = 0;i < strlen(temp);i++) {
                word temp_data_word;
                temp_data_word.ARE = 'A';
                temp_data_word.value = (int)temp[i];
                add_to_list(fileInformation1,&fileInformation1->data_head,temp_data_word,fileInformation1->DC++,SECONDARY_WORD);
            }
        data_word_to_add.value = (int) '\0';
            data_word_to_add.ARE = 'A';
        add_to_list(fileInformation1,&fileInformation1->data_head,data_word_to_add,fileInformation1->DC++,SECONDARY_WORD);
    }

    free(temp);
}
/**
 * checking if the last char of a string is a quote
 * @param str input string
 * @return  TRUE if last char is a quote or FALSE if not
 */
bool isLastCharQuote (char * str){
   int len;
   len = strlen(str);

   if(str[len-1] == '"')
       return TRUE;
   else
       return FALSE;
}

/**
 * validating the number input is correct
 * @param s string to be checked
 * @param fileInformation1 struct that holds input file data
 * @return TRUE or FALSE
 */
bool isNumber(char * s, fileInformation *fileInformation1)
{
    int i = 0;
    if(*s == '-' || *s == '+') {
        if(strlen(s) == 1){
            printf("INVALID NUMBER line %d", fileInformation1->line_num);
            fileInformation1->status= TRUE;
        return FALSE;}
        i++;
    }
    for (; i < strlen(s); i++)
        if (isdigit(s[i]) == FALSE)
            return FALSE;

    return TRUE;
}

/**
 * gets a line with operating code and analyze opcode,funct ,addressing types or any code word that needed to be printed later
 * and add the words to code list and increasing IC
 * @param file_info struct that hold all information needed to handle the input file. the struct should already hold in "before" the operation and in "after" the operands
 * @param before_and_after_struct struct filled already with previous information needed to split_line function
 */
void analyzeInstruction(fileInformation *file_info, before_and_after *before_and_after_struct) {
    char operation[MAX_LINE_LENGTH];/* will hold the operation string */
    char  operands[MAX_LINE_LENGTH];/*will hold the operands */
    word code_word; /*the first word to add */
    word code_word_2;/* word 2 and 3 is optionaly and values to these words will be added only if there are operands */
    word code_word_3;
    int L = 0; /*represent if there are 1 or 2 additionaly words to the first one*/

    /*cutting any white spaces from strings */
    strcpy(operation,trim(before_and_after_struct->before));
    strcpy(operands,trim(before_and_after_struct->after));

        /*getting opcode and funct and saving it into code_word*/
    get_opcode_func(operation,&code_word);


    if(code_word.WORD_OP_CODE == INVALID_OP_CODE) {
        printf("error, invalid operation on line %d\n", file_info->line_num);
        file_info->status = TRUE;
        return;
    }
    if(code_word.WORD_OP_CODE < 5 &&code_word.WORD_OP_CODE>=0){
        /* checks if valid compared to the opcode */
        /* check if arugment is valid */

        char  op1[MAX_LINE_LENGTH]; /*hold the first operand. if there is only 1 operand it will be empty*/
        char  op2[MAX_LINE_LENGTH];/*hold the second operand */
        bool isDelimExist;/*will hold the value of wether the ere ' , ' or not */
        isDelimExist= split_line(operands,",",before_and_after_struct);
        if(!isDelimExist){/* if there is no ' , ' there is an error because the command require 2 operands */
            printf("missing operand or operand is invalid on line %d\n", file_info->line_num);
            file_info->status = TRUE;
            return;
        }
        /*if we got here we have 2 operands */
        strcpy(op1,trim(before_and_after_struct->before));
        strcpy(op2,trim(before_and_after_struct->after));

        if(split_line(op2,",",before_and_after_struct)==TRUE){
            printf("missing operand or operand is invalid on line %d\n", file_info->line_num);
            file_info->status = TRUE;
            return;
        }

        if((strcmp(trim(op2),"")==0)||(op2[0]==',')){
            printf("missing operand or operand is invalid on line %d\n", file_info->line_num);
            file_info->status = TRUE;
            return;
        }

        /*getting addressing type and save it in the code_word */
        code_word.src_addressing_type = get_addressing_type(op1,file_info);
        code_word.dest_addressing_type = get_addressing_type(op2,file_info);
        /*check if opcode fit to addressing type */
        if(!operands_addressing_valid(&code_word)) { /*gets 2 operands addressing types and checks if it matchs the command */
            printf("operand doesnt match the command on line %d\n", file_info->line_num);
            file_info->status = TRUE;
            return;
        }


        L = 3;

        /*if source addressing is immediate*/
        if(code_word.src_addressing_type == IMMEDIATE){

            strcpy(op1,trim(op1+1));
            code_word_2.value = atoi(op1);
            strcpy(code_word_2.labelName, "");
            code_word_2.ARE = 'A';
            /*we will fill label name with blank value to avoid garbage values*/
            strcpy(code_word_2.labelName, "");

            if(code_word_2.value>MAXIMAL_INT_VALUE || code_word_2.value < MINIAL_INT_VALUE){
                printf("number out of boundaries in line %d\n",file_info->line_num);
                file_info->status = TRUE;
                return;
            }
            /*if its register */
        } else if(code_word.src_addressing_type == REGISTER){

            code_word_2.value = register_name_to_bin(op1);
            code_word_2.ARE = 'A';
            /*we will fill label name with blank value to avoid garbage values*/
            strcpy(code_word_2.labelName, "");
            /*if source code is direct */
        } else if(code_word.src_addressing_type == DIRECT) {
            strcpy(code_word_2.labelName,op1);
            code_word_2.ARE = 'R';
        } else { /* the addressing type has to be 2 which means Absolute */
            code_word_2.value = 0;
            code_word_2.ARE = 'A';
            strcpy(code_word_3.labelName, "");
        }

        /* Now we will deal will the destination addressing */
        if(code_word.dest_addressing_type == IMMEDIATE){
            strcpy(op2,trim(op2+1));
            code_word_3.value = atoi(op2);
            code_word_3.ARE = 'A';
            strcpy(code_word_3.labelName, "");

            if(code_word_3.value>MAXIMAL_INT_VALUE || code_word_3.value < MINIAL_INT_VALUE){
                printf("number out of boundaries in line %d\n",file_info->line_num);
                file_info->status = TRUE;
                return;
            }

        } else if(code_word.dest_addressing_type == REGISTER){
            code_word_3.ARE = 'A';
            code_word_3.value = register_name_to_bin(op2);
            strcpy(code_word_3.labelName, "");

        } else if(code_word.dest_addressing_type == DIRECT) {
            strcpy(code_word_3.labelName, op2);
            code_word_3.ARE = 'R';
        } else { /* the addressing type has to be 2, which means Absolute */
            code_word_3.ARE = 'A';
            code_word_3.value = 0;
            strcpy(code_word_3.labelName, "");
        }
    }
                     /* one operand - no need of source addressing*/
    else if(code_word.WORD_OP_CODE>=5 && code_word.WORD_OP_CODE<=13){
        L = 2;
        code_word.dest_addressing_type = get_addressing_type(operands, file_info);
        /*if operand type is invalid*/
        if(code_word.dest_addressing_type==NO_ADDRESS){
            printf("missing or invalid operand on line %d\n",file_info->line_num);
            file_info->status = TRUE;
            return;
        }
         /*cheking if operand is missing or dosent match the addressing type*/
        if((strcmp(trim(operands),"")==0)||(code_word.WORD_OP_CODE!=PRN_OP_CODE&&operands[0]=='#')){
            printf("missing or invalid operand on line %d\n",file_info->line_num);
            file_info->status = TRUE;
            return;
        }
        /*blank value to source addressing*/
        code_word.src_addressing_type = 0;
        if(code_word.dest_addressing_type == IMMEDIATE) {
            strcpy(operands,operands+1);

            code_word_3.value = atoi(operands);
            code_word_3.ARE = 'A';
            strcpy(code_word_3.labelName, "");

            if(code_word_3.value>MAXIMAL_INT_VALUE || code_word_3.value < MINIAL_INT_VALUE){
                printf("number out of boundaries in line %d\n",file_info->line_num);
                file_info->status = TRUE;
                return;

            }

        } else if(code_word.dest_addressing_type == REGISTER){
            code_word_3.value = register_name_to_bin(operands);
            code_word_3.ARE = 'A';
            strcpy(code_word_3.labelName, "");

        } else if(code_word.dest_addressing_type == DIRECT) {
            strcpy(code_word_3.labelName, operands);
            code_word_3.ARE = 'R';
        }
        if(code_word.dest_addressing_type==RELATIVE) { /*destination addressing is realtive */
            code_word_3.value = 0;
            code_word_3.ARE = 'A';
            strcpy(code_word_3.labelName, "");
        }

        /*if operations dosent require operands */
    } else if(code_word.WORD_OP_CODE >= 14){
        code_word.src_addressing_type=0;
        code_word.dest_addressing_type=0;
       if(strlen(trim(operands)) != 0 ){
           printf("error there is text after opertaion on line %d\n", file_info->line_num);
           file_info->status = TRUE;
           return;
       }
       L = 1;
    }

    code_word.L = L;
    code_word.ARE = 'A';

                /* Adding the words to the list */
    if(L==3){
       add_to_list(file_info,&file_info->code_head,code_word,file_info->IC++,FIRST_WORD);

        add_to_list(file_info,&file_info->code_head,code_word_2,file_info->IC++,SECONDARY_WORD);

        add_to_list(file_info,&file_info->code_head,code_word_3,file_info->IC++,SECONDARY_WORD);

   }

   if(L==2){
       add_to_list(file_info,&file_info->code_head,code_word,file_info->IC++,FIRST_WORD);


       add_to_list(file_info,&file_info->code_head,code_word_3,file_info->IC++,SECONDARY_WORD);

   }
   if(L==1){
       add_to_list(file_info,&file_info->code_head,code_word,file_info->IC++,FIRST_WORD);

   }

}



/* used to translate string command to numbers */
static  look_up_element lookup_table[] = {
        {"mov", MOV_OP_CODE, MOV_FUNCT},
        {"cmp",CMP_OP_CODE, CMP_FUNCT},
        {"add",ADD_OP_CODE, ADD_FUNCT},
        {"sub",SUB_OP_CODE, SUB_FUNCT},
        {"lea",LEA_OP_CODE, LEA_FUNCT},
        {"clr",CLR_OP_CODE, CLR_FUNCT},
        {"not",NOT_OP_CODE, NOT_FUNCT},
        {"inc",INC_OP_CODE, INC_FUNCT},
        {"dec",DEC_OP_CODE, DEC_FUNCT},
        {"jmp",JMP_OP_CODE, JMP_FUNCT},
        {"bne",BNE_OP_CODE, BNE_FUNCT},
        {"jsr",JSR_OP_CODE, JSR_FUNCT},
        {"red",RED_OP_CODE, RED_FUNCT},
        {"prn",PRN_OP_CODE, PRN_FUNCT},
        {"rts",RTS_OP_CODE, RTS_FUNCT},
        {"stop",STOP_OP_CODE, STOP_FUNCT},
        {NULL, 0, 0},
};
/**
 * getting a string with a command and a word struct and set the opcode and funct in the struct according to the string
 * @param command string with a command
 * @param word1 struct that will conatain the values
 */
void get_opcode_func(char *command, word *word1) {
    look_up_element *e; /*setting up an element to run trouhg the table */
    word1->WORD_OP_CODE = INVALID_OP_CODE;/*setting both for no values to avoid garbage values */
    word1->WORD_FUNCT= INVALID_OP_CODE;
    /* iterate through the lookup table, if commands are same return the opcode of found. */
    for (e = lookup_table; e->cmd != NULL; e++) {
        if (strcmp(e->cmd, command) == 0) {
            word1->WORD_OP_CODE = e->op;
            word1->WORD_FUNCT = e->fun;
            return;
        }
    }
}

/**
 * getting string with operand and returns what addressing typ the operand is
 * @param operand string include the operand to analyze
 * @param fileInformation1 struct that holds the input file information
 * @return enum that represent what addressing typ the command is
 */
addressing_type get_addressing_type(char *operand, fileInformation *fileInformation1) {
    /* if nothing, just return none */
    if (operand[0] == '\0') return NO_ADDRESS;
    /* if first char is 'r', second is number in range 0-7 and third is end of string, it's a register */
    if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' && operand[2] == '\0') return REGISTER;
        /* if operand starts with # and a number right after that, it's immediately addressed */
    else if (operand[0] == '#' && isNumber(operand + 1,fileInformation1)) return IMMEDIATE;
        /* if operand starts with % and has label afterwards, it;s realtively addressed */
    else if (operand[0] && operand[0] == '%' && is_label_valid(operand + 1)) return RELATIVE;
        /* if operand is a valid label name, it's directly addressed */
    else if (is_label_valid(operand)) return DIRECT;
    else return NO_ADDRESS;
}

/**
 * checks if the label name is valid
 * @param str  the label name
 * @return if label is valid or not
 */
bool is_label_valid(char * str){
    if(str[0] && strlen(str) <= 31 && isalpha(str[0])&&!is_reserved_word(str)){
        if(strlen(str)>1){
            return is_alpha_or_number(str+ 1);
        }
        return TRUE;
    }
      return FALSE;
}

bool is_alpha_or_number(char * str){
    int i;
    for (i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && !isdigit(str[i]))
            return FALSE;
    }
    return TRUE;

}

bool is_reserved_word(char * str){
    word *word1;
    bool b = FALSE;
    word1 = (word*) malloc(sizeof(word));
    /* check if register or command */
    get_opcode_func(str,word1);
    if (word1->WORD_OP_CODE != INVALID_OP_CODE || register_name_to_bin(str) != NONE_REG /*|| find_instruction_by_name(str) != NONE_INST*/)
        b = TRUE;
    free(word1);
    return b;
}

int register_name_to_bin(char * str){
    if (str[0] == 'r' && isdigit(str[1]) && str[2] == '\0'){
        if(str[1] == '0')
            return 1;

        if(str[1] == '1')
            return 2;

        if(str[1] == '2')
            return 4;

        if(str[1] == '3')
            return 8;

        if(str[1] == '4')
            return 16;

        if(str[1] == '5')
            return 32;

        if(str[1] == '6')
            return 64;

        if(str[1] == '7')
            return 128;
    }
return -1;
}
/**
 * gets a word struct and checks if the operands addressing type are valid to the opcode and the funct
 * @param code_word the struct to be checked
 * @return TRUE if operands addressing types are valid and FALSE if not
 */
bool operands_addressing_valid(word *code_word){
    /*if command require 2 operands but one of them is missing or not valid*/
    if((code_word->WORD_OP_CODE>-1 && code_word->WORD_OP_CODE<5)){
        if(code_word->src_addressing_type==NO_ADDRESS||code_word->dest_addressing_type==NO_ADDRESS)
            return FALSE;
    }

    /* if command is add */
    if(code_word->WORD_OP_CODE == ADD_OP_CODE && code_word->WORD_FUNCT == ADD_FUNCT){
        if(code_word->src_addressing_type == RELATIVE ||( code_word->dest_addressing_type ==IMMEDIATE || code_word->dest_addressing_type == RELATIVE))
            return FALSE;
    }
    /* if command is cmp */
    if(code_word->WORD_OP_CODE == CMP_OP_CODE && code_word->WORD_FUNCT == CMP_FUNCT) {
        if (code_word->src_addressing_type == RELATIVE || code_word->dest_addressing_type == RELATIVE)
            return FALSE;

    }

    /*if command is mov */
    if(code_word->WORD_OP_CODE == MOV_OP_CODE && code_word->WORD_FUNCT == MOV_FUNCT){
        if(code_word->src_addressing_type == RELATIVE || (code_word->dest_addressing_type ==IMMEDIATE || code_word->dest_addressing_type == RELATIVE))
            return FALSE;
    }

    /* if command is sub */
    if(code_word->WORD_OP_CODE == SUB_OP_CODE && code_word->WORD_FUNCT == SUB_FUNCT){
        if(code_word->src_addressing_type == RELATIVE || (code_word->dest_addressing_type ==IMMEDIATE || code_word->dest_addressing_type == RELATIVE))
            return FALSE;
    }

    /* if command is lea */
    if(code_word->WORD_OP_CODE == LEA_OP_CODE && code_word->WORD_FUNCT == LEA_FUNCT){
        if(code_word->src_addressing_type != DIRECT ||( code_word->dest_addressing_type ==IMMEDIATE || code_word->dest_addressing_type == RELATIVE))
            return FALSE;
    }

    /* if command is clr ,not,inf,dec ,red */
    if(code_word->WORD_OP_CODE == 5 ||( code_word->WORD_OP_CODE == RED_OP_CODE&& code_word->WORD_FUNCT == RED_FUNCT)){
        if(code_word->src_addressing_type != NO_ADDRESS  ||( code_word->dest_addressing_type !=DIRECT || code_word->dest_addressing_type != REGISTER))
            return FALSE;
    }



    /* if command is jmp,bne,jsr */
    if(code_word->WORD_OP_CODE == JMP_OP_CODE ){
        if(code_word->src_addressing_type != NO_ADDRESS || (code_word->dest_addressing_type ==0 || code_word->dest_addressing_type == 3))
            return FALSE;
    }

/* if command is prn */
    if(code_word->WORD_OP_CODE == PRN_OP_CODE && code_word->WORD_FUNCT == PRN_FUNCT){
        if(code_word->src_addressing_type != NO_ADDRESS || code_word->dest_addressing_type ==2 )
            return FALSE;
    }

    /* if command is rts */
    if(code_word->WORD_OP_CODE == RTS_OP_CODE && code_word->WORD_FUNCT == RTS_FUNCT){
        if(code_word->src_addressing_type != NO_ADDRESS || code_word->dest_addressing_type != NO_ADDRESS )
            return FALSE;
    }

    /* if command is stop */
    if(code_word->WORD_OP_CODE == STOP_OP_CODE && code_word->WORD_FUNCT == STOP_FUNCT){
        if(code_word->src_addressing_type != NO_ADDRESS || code_word->dest_addressing_type != NO_ADDRESS)
            return FALSE;
    }

    return TRUE;
}

