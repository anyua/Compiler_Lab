#include "semantics.h"

Identifier * ID_SymbolTable[BUCKETS] = { 0 };
Identifier * Constant_SymbolTable[BUCKETS] = { 0 };
int offset = 0;
int(*semantic_func[])(State* new_state, Stack* parameter_stack) = \
{
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, F68, F69, \
	PASS, F71, PASS, F73, PASS,		PASS, PASS, PASS, F78, PASS, \
	PASS, PASS, PASS, PASS, F84,	 PASS, F86, PASS, F88, PASS, \
	F90, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
};

Tuple_2* pass_param(State * new_state, Stack * parameter_stack)
{
	Tuple_2* p = (Tuple_2*)malloc(sizeof(Tuple_2));
	State* tstate = pop(parameter_stack);
	Tuple_2* tvalue = tstate->value;
	memcpy(p, tvalue, sizeof(Tuple_2));
	new_state->value = p;

	for (; tvalue->next;)
	{
		tvalue = tvalue->next;
		p->next = (Tuple_2*)malloc(sizeof(Tuple_2));
		p = p->next;
		memcpy(p, tvalue, sizeof(Tuple_2));
	}
	free(tstate);

	for (; parameter_stack->size > 0;)
	{
		tstate = pop(parameter_stack);
		tvalue = tstate->value;
		p->next = (Tuple_2*)malloc(sizeof(Tuple_2));
		p = p->next;
		memcpy(p, tvalue, sizeof(Tuple_2));
		for (; tvalue->next;)
		{
			tvalue = tvalue->next;
			p->next = (Tuple_2*)malloc(sizeof(Tuple_2));
			p = p->next;
			memcpy(p, tvalue, sizeof(Tuple_2));
		}
		free(tstate);
	}
	p->next = NULL;
	return p;
}

int pass(State * new_state, Stack * parameter_stack)
{
	return 0;
}

int F68(State * new_state, Stack * parameter_stack)
{
	printf("%d", parameter_stack->size);
	State* specifiers = parameter_stack->data[3];
	State* declarator_list = parameter_stack->data[2];
	Tuple_2 * declarator = declarator_list->value;
	for (; declarator; declarator = declarator->next)
	{
		if (declarator->key == ID)
		{
			if (declarator->next&&declarator->next->key == NUM)
				offset += (declarator->next->num*specifiers->value->next->key);
			else
				offset += specifiers->value->next->key;
			enter(ID_SymbolTable, declarator->value->name, \
				offset, specifiers->value->key);
		}
	}
	return 0;
}

int F69(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F71(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F72(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F73(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F78(State * new_state, Stack * parameter_stack)
{
	Tuple_2* p = pass_param(new_state, parameter_stack);
	p->next = (Tuple_2*)malloc(sizeof(Tuple_2));
	p->next->key = 4;
	p->next->next = NULL;
	return 0;
}

int F84(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F86(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F88(State * new_state, Stack * parameter_stack)
{
	new_state->value = (Tuple_2*)malloc(sizeof(Tuple_2));
	new_state->value->key = -1;
	new_state->value->next = NULL;
	return 0;
}

int F90(State * new_state, Stack * parameter_stack)
{
	new_state->value = (Tuple_2*)malloc(sizeof(Tuple_2));
	memcpy(new_state->value, parameter_stack->data[4]->value, sizeof(Tuple_2));
	new_state->value->next = (Tuple_2*)malloc(sizeof(Tuple_2));
	memcpy(new_state->value->next, parameter_stack->data[2]->value, sizeof(Tuple_2));
	return 0;
}

