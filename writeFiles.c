
#include "writeFiles.h"

void writeFiles(fileInformation *fileInformation1){
    char obFileName[MAX_FILE_NAME];
     int val;
    FILE * obfile;
    DataNodePointer codeTemp;

    strcpy(obFileName, strallocat(fileInformation1->file_name,".ob"));

    obfile = fopen(obFileName,"w");

    printf("\nprinting to ob file\n");

    /* print the code image */

    fprintf(obfile,"    %d %d\n",fileInformation1->IC-IC_INIT_VALUE,fileInformation1->DC);
    codeTemp = fileInformation1->code_head;
    while(codeTemp != NULL) {
        /* print the words */

        /*if the word is a first word in sentence (contain opcode,funct and addressing methods) */
        if(codeTemp->status == FIRST_WORD) {

            val = (codeTemp->word_in_node.WORD_OP_CODE << 8) |
                  (codeTemp->word_in_node.WORD_FUNCT << 4) |
                  (codeTemp->word_in_node.src_addressing_type << 2) |
                  (codeTemp->word_in_node.dest_addressing_type);

        }

        /* if the word is second or third to the first word which means contain a value */
        else{
            val = codeTemp->word_in_node.value;
            if(val < 0) {
                val &= TWELVE_BITS_ON;
            }
        }

        /* printing the IC and the value */
        fprintf(obfile, "%04d ", codeTemp->memory_address);
        fprintf(obfile,"%03X ",val);
        fprintf(obfile,"%c\n",codeTemp->word_in_node.ARE);

         codeTemp = codeTemp->next;

    }

    /* print the data image */
    while(fileInformation1->data_head != NULL) {
        /* print the words */
        val = fileInformation1->data_head->word_in_node.value;
        if(val < 0) {
            val &= TWELVE_BITS_ON;
        }
        fprintf(obfile,"%04d ",fileInformation1->data_head->memory_address);
        fprintf(obfile,"%03X ",val);
        fprintf(obfile,"%c\n",fileInformation1->data_head->word_in_node.ARE);

        fileInformation1->data_head= fileInformation1->data_head->next;

    }

    if(fileInformation1->hasEnt == TRUE) {
        FILE * entFile;
        char entFileName[MAX_FILE_NAME];
        SymbolNodePointer  temp;
        temp = fileInformation1->symbol_head;
        strcpy(entFileName, strallocat(fileInformation1->file_name,".ent"));

        entFile = fopen(entFileName,"w");

        if(entFile == NULL) {
            printf("Couldn't Create Entry File!\n");
            return;
        }

        printf("\nprinting to ent file\n");


        while(temp != NULL){
            if(temp->type == ENTRY_TYPE){
            fprintf(entFile,"%s %04d\n",temp->name,temp->memory_address);

            }
            temp = temp->next;

        }
        fclose(entFile);
        /* manage file.ent */
    }

    if(fileInformation1->hasExt == TRUE) {
        /* manage file.ext */
        FILE *extFile;
        char extFileName[MAX_FILE_NAME];
        strcpy(extFileName, strallocat(fileInformation1->file_name, ".ext"));
        codeTemp = fileInformation1->code_head;

        extFile = fopen(extFileName, "w");

        while(codeTemp != NULL){
            if(codeTemp->word_in_node.ARE == 'E'){
                fprintf(extFile,"%s %04d\n",codeTemp->word_in_node.labelName,codeTemp->memory_address);
            }
            codeTemp = codeTemp->next;
        }

        printf("\nprinting to ext file\n");


        fclose(extFile);
    }
    fclose(obfile);

}
