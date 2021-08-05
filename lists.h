
#ifndef _LISTS_H
#define _LISTS_H
#include "base.h"
void update_extern_name(DataNodePointer head,int address,char *s);
void add_to_list(fileInformation * fileInformation1, DataNodePointer *head, word word_to_add,int address,FirstOrSecondary status);
void update_are(DataNodePointer head, int address,char ARE);
void freeList(DataNodePointer head);

#endif
