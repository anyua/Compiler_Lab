#ifndef LR_PARSER
#define LR_PARSER
#include "type_define.h"
#include "symbol_table.h"
#include "lexical_analyzer.h"
#include "semantics.h"

Table* load_table(char* filename);
Grammer* load_grammer(char* filename);
void push(Stack* stack, State* state);
State* pop(Stack* stack);
State* top(Stack* stack);
char* get_a_symbol(FILE* fp);
int index_of_V(char ** list, int n, char * target, Table* action_table);
int index_of_T(Tuple_2* target, Table* action_table);
int beta_len(char* rule);
char* get_A(char* rule);

void lr_parser(Grammer* grammer, Table* action_table, Table* goto_table, char* program_filename);

#endif // !LR_PARSER