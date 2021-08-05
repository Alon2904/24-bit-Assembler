
#include "assembler.h"


int main(int argc, char *argv[]) {


int i;
    for(i = 1; i < argc; ++i) {
        char filename[MAX_FILE_NAME]; /** decleration of a pointer to a file */

        fileInformation *variablesPtr; /* pointer that will hold all of the necesery iformation to handle the data */

        variablesPtr = malloc(sizeof(fileInformation));
        variablesPtr->hasEnt = FALSE;
        variablesPtr->hasExt = FALSE;
        variablesPtr->status = FALSE;
        variablesPtr->data_head = NULL;
        variablesPtr->code_head = NULL;
        variablesPtr->file = NULL;
        variablesPtr->line_num = 0;

        freeList(variablesPtr->data_head);/*cleaning lists rom garbage values */
        freeList(variablesPtr->code_head);

        strcpy(variablesPtr->file_name,argv[i]);
        strcpy(filename,strallocat(variablesPtr->file_name, ".as")); /** restring .as to the input argument */

        variablesPtr->file = fopen(filename,"r");

        if(variablesPtr->file == NULL) {
            printf("Cannot Open File!\n");
            continue;
        }

        printf("before first pass\n");

        /**start to run first pass */
        first_pass(variablesPtr);

        rewind(variablesPtr->file); /** return the pointer to the begining of the file */


        printf("after first pass\n");

        if(variablesPtr->status == TRUE) { /**if we found some errors in first pass */
            fclose(variablesPtr->file);
            free(variablesPtr);
            continue;
        }

        printf("before second pass\n");

        second_pass(variablesPtr);
        printf("after second pass\n");

        if(variablesPtr->status == TRUE) { /**if we found some errors in second pass */
            fclose(variablesPtr->file);
            free(variablesPtr);
            continue;
        }
        writeFiles(variablesPtr);
        /* TODO edit this function writeFiles(variablesPtr); */

    }
    return 0;
}


/**
 * restring 2 strings one after the other
 * @param s0 first string
 * @param s1 second string
 * @return pointer to the new string
 */
char *strallocat(char *s0, char* s1) {
    char *str = (char *)malloc(strlen(s0) + strlen(s1) + 1);
    strcpy(str, s0);
    strcat(str, s1);
    return str;
}