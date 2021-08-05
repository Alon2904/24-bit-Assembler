

#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H
#include "base.h"
#include "lists.h"
void update_relative_value(fileInformation *fileInformation1, int tempIC, int wordPosition);
void update_value(fileInformation *fileInformation1, int newAddress, int memoryInCode);
int  return_address_by_label(fileInformation *fileInformation1, char * op1);
bool update_entry(SymbolNodePointer head,char * str,fileInformation *fileInformation1);
void add_to_Symbol_list(SymbolNodePointer *head, int address, SYMBOL_TYPE type, char *labelName);


#endif
