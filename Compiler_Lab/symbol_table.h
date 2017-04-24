#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
#include "type_define.h"

Identifier * enter(Identifier** table, char * name, int offset, int type);

Identifier * insert(Identifier** table,char * name);

Identifier * lookup(Identifier** table, char * name);

int hashpjw(char * s);

Identifier** mktable();

#endif