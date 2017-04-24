#ifndef SEMANTICS
#define SEMANTICS
#include "type_define.h"
#include "symbol_table.h"

#define call_semantic_define(rule_index,new_state,parameter_stack) \
	semantic_func[rule_index](new_state, parameter_stack); 
#define PASS pass

Tuple_2* pass_param(State* new_state, Stack* parameter_stack);

int pass(State * new_state, Stack * parameter_stack);

int gencode(int op,Identifier* arg1,Identifier* arg2,Identifier* result);
Identifier* newtemp(int type);

//expression
int F32(State * new_state, Stack * parameter_stack);

int F33(State * new_state, Stack * parameter_stack);

int F34(State * new_state, Stack * parameter_stack);
int F40(State * new_state, Stack * parameter_stack);
int F42(State * new_state, Stack * parameter_stack);
int F44(State * new_state, Stack * parameter_stack);
int F46(State * new_state, Stack * parameter_stack);
int F49(State * new_state, Stack * parameter_stack);
int F54(State * new_state, Stack * parameter_stack);
int F55(State * new_state, Stack * parameter_stack);
int F57(State * new_state, Stack * parameter_stack);
int F61(State * new_state, Stack * parameter_stack);
int F62(State * new_state, Stack * parameter_stack);

int F65(State * new_state, Stack * parameter_stack);

//Declaration
int F68(State * new_state, Stack * parameter_stack);
int F69(State * new_state, Stack * parameter_stack);
int F71(State * new_state, Stack * parameter_stack);
int F73(State * new_state, Stack * parameter_stack);
int F78(State * new_state, Stack * parameter_stack);
int F84(State * new_state, Stack * parameter_stack);
int F86(State * new_state, Stack * parameter_stack);
int F88(State * new_state, Stack * parameter_stack);
int F90(State * new_state, Stack * parameter_stack);

extern int(*semantic_func[])(State* new_state, Stack* parameter_stack);

#endif // !SEMANTICS