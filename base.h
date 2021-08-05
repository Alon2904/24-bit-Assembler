
#ifndef ASSEMBLERPROJECT_BASES_H
#define ASSEMBLERPROJECT_BASES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIAL_INT_VALUE -2048
#define MAXIMAL_INT_VALUE 2047
#define FILE_NAME_LENGTH 100
#define LINE_LENGTH 82
#define MAXIMAL_LABEL_SIZE 31


/** The maximum length of input */

/** maximal length of a line */
#define MAX_LINE_LENGTH 80

#define IC_INIT_VALUE 100

#define TWELVE_BITS_ON 0x00000FFF


/** Operand addressing type */
typedef enum {
    IMMEDIATE = 0,
    DIRECT ,
    RELATIVE,
    REGISTER,
    NO_ADDRESS = -1

} addressing_type ;

/** Instruction type */
typedef  enum {

    DOT_DATA = 0,
    DOT_STRING,
    DOT_EXTERN,
    DOT_ENTRY,
    NONE = -1,
    ERROR_DIRECTIV = -2


}data_or_string_or_ext_or_ent;

/** Symbol type */
typedef enum {
    EXTERN_TYPE = 0,
    ENTRY_TYPE,
    DATA_TYPE,
    CODE_TYPE

}SYMBOL_TYPE;

/** operation code */
typedef enum  {
    MOV_OP_CODE = 0,
    CMP_OP_CODE = 1,
    ADD_OP_CODE = 2,
    SUB_OP_CODE = 2,
    LEA_OP_CODE = 4,
    CLR_OP_CODE = 5,
    NOT_OP_CODE = 5,
    INC_OP_CODE = 5,
    DEC_OP_CODE = 5,
    JMP_OP_CODE = 9,
    BNE_OP_CODE = 9,
    JSR_OP_CODE = 9,
    RED_OP_CODE = 12,
    PRN_OP_CODE = 13,
    RTS_OP_CODE = 14,
    STOP_OP_CODE = 15,
    INVALID_OP_CODE = -1

} OP_CODE;
/** funct code */
typedef enum {
    MOV_FUNCT = 0 ,
    CMP_FUNCT = 0,
    ADD_FUNCT = 10,
    SUB_FUNCT = 11,
    LEA_FUNCT = 0,
    CLR_FUNCT = 10,
    NOT_FUNCT = 11,
    INC_FUNCT = 12,
    DEC_FUNCT = 13,
    JMP_FUNCT = 10,
    BNE_FUNCT = 11,
    JSR_FUNCT = 12,
    RED_FUNCT = 0,
    PRN_FUNCT = 0,
    RTS_FUNCT = 0,
    STOP_FUNCT =0

}FUNCT;

/** Represents the registers */
typedef enum  {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    NONE_REG = -1
} reg;

/** Represent if a word is code word or data word */
typedef enum {
    CODE_WORD = 0,
    DATA_WORD = 1

}WORD_TYPE;

/** struct used to search in a table */
typedef struct lookup_element {
    char *cmd;
    OP_CODE op;
    FUNCT fun;
}look_up_element;


/**
 * Includes all the needed attributes to handle a word that will have to be printed
 * */
typedef struct {

    WORD_TYPE WORD_T; /** code = 0 , data =1 */

/**
 * if code word
 * */
    OP_CODE WORD_OP_CODE ;
    FUNCT WORD_FUNCT;
    addressing_type src_addressing_type;
    addressing_type  dest_addressing_type;
    int L; /** value is the number of the extra words we need to make 0/1/2 */

    /** if data */
    int value;

    char ARE;
    char labelName[MAXIMAL_LABEL_SIZE];
}word;

/**
 * a struct that is need to the function splitLine
 * */
typedef struct {
    char before[LINE_LENGTH];
    char after[LINE_LENGTH];


}before_and_after;

typedef enum  {
    FALSE = 0, TRUE = 1
} bool;




typedef struct SymbolNode* SymbolNodePointer;
/**
 * Node to hold symbols in the the symbols table
 * */
typedef struct SymbolNode {
    char name[MAXIMAL_LABEL_SIZE];
    int memory_address;
    int dc;
    SYMBOL_TYPE type;
    SymbolNodePointer next;

} SymbolNode;
/**
 * tells us if the word is the first word in a statement or secondary
 */
typedef enum FtOrSry  {
    FIRST_WORD,
    SECONDARY_WORD
}FirstOrSecondary;

 typedef struct DataNode* DataNodePointer;

/**
 * Node to hold data in the data table
 *
 */
typedef struct DataNode {
    word word_in_node;
    int memory_address;
    DataNodePointer next;
    int line;
    FirstOrSecondary status;
    char externName[MAXIMAL_LABEL_SIZE];
} DataNode;

/** handel all of the necesery data for the specific file */
typedef struct fileInformation {
    FILE *file;
    int IC;
    int DC;
    char file_name[FILE_NAME_LENGTH];
    char line_content[LINE_LENGTH];
    int line_num;
    bool status; /*true if error */
    SymbolNodePointer symbol_head;
    DataNodePointer data_head;
    DataNodePointer code_head;
    bool hasExt; /*if there is need for externals file*/
    bool hasEnt;/* if there is need for entry file */



}fileInformation;




#endif
