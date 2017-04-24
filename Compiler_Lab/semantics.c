#include "semantics.h"

Identifier * ID_SymbolTable[BUCKETS] = { 0 };
Identifier * Constant_SymbolTable[BUCKETS] = { 0 };
int constant_offset = 0;
int offset = 0;
int temp_offset = 0;

TAC* three_address_code[1000] = { NULL };
int tac_offset = 0;

int(*semantic_func[])(State* new_state, Stack* parameter_stack) = \
{
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	PASS, PASS, F32, F33, F34,	 PASS, PASS, PASS, PASS, PASS, \
	F40, PASS, F42, PASS, F44,	 PASS, F46, PASS, PASS, F49, \
	PASS, PASS, PASS, PASS, F54,	 F55, PASS, F57, PASS, PASS, \
	PASS, F61, F62, PASS, PASS,	 F65, PASS, PASS, F68, F69, \
	PASS, F71, PASS, F73, PASS,		PASS, PASS, PASS, F78, PASS, \
	PASS, PASS, PASS, PASS, F84,	 PASS, F86, PASS, F88, PASS, \
	F90, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
};
/*
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
*/

Tuple_2* pass_param(State * new_state, Stack * parameter_stack)
{
	State* tstate = pop(parameter_stack);
	new_state->value = tstate->value;
	Tuple_2* p = NULL;
	for (; tstate; tstate = pop(parameter_stack))
	{
		if (p)
		{
			p->next = tstate->value;
		}
		p = tstate->value;
		for (; p->next ; p = p->next);
	}
	return p;
}


int pass(State * new_state, Stack * parameter_stack)
{
	return 0;
}

int gencode(int op, Identifier * arg1, Identifier * arg2, Identifier * result)
{
	printf("%d\n", tac_offset);
	three_address_code[tac_offset] = (TAC*)malloc(sizeof(TAC));
	TAC* tac = three_address_code[tac_offset];
	tac->arg1 = arg1;
	tac->arg2 = arg2;
	tac->op = op;
	tac->result = result;
	return tac_offset++;
}

Identifier * newtemp(int type)
{
	char temp[10];
	itoa(temp_offset++, temp, 8);
	Identifier* result = enter(ID_SymbolTable, temp, 4, type);
	return result;
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
				offset += (declarator->next->value->const_value->num*specifiers->value->next->key);
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
	//new_state->value = (Tuple_2*)malloc(sizeof(Tuple_2));
	//memcpy(new_state->value, parameter_stack->data[4]->value, sizeof(Tuple_2));
	//new_state->value->next = (Tuple_2*)malloc(sizeof(Tuple_2));
	//memcpy(new_state->value->next, parameter_stack->data[2]->value, sizeof(Tuple_2));
	new_state->value = parameter_stack->data[4]->value;
	new_state->value->next = parameter_stack->data[2]->value;
	return 0;
}

int F32(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F33(State * new_state, Stack * parameter_stack)
{
	Identifier* arg1 = parameter_stack->data[1]->value->value;
	Identifier* result = parameter_stack->data[3]->value->value;
	int op = parameter_stack->data[2]->value->key;
	//Identifier* result = newtemp(INT);
	gencode(op,arg1,NULL,result);
	return 0;
}

int F34(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F40(State * new_state, Stack * parameter_stack)
{
	//pass_param(new_state, parameter_stack);
	Tuple_2* p = (Tuple_2*)malloc(sizeof(Tuple_2));
	State* tstate = parameter_stack->data[1];
	Tuple_2* tvalue = tstate->value;
	Identifier* tid = lookup(ID_SymbolTable, tvalue->value->name);
	if (tid)
	{
		p->key = 0;
		p->value = tid;
		p->next = NULL;
		new_state->value = p;
	}
	else
	{
		printf("error!\n");
		pass_param(new_state, parameter_stack);
	}
	return 0;
}

int F42(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F44(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F46(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F49(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F54(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F55(State * new_state, Stack * parameter_stack)
{
	Identifier* arg1 = parameter_stack->data[3]->value->value;
	Identifier* arg2 = parameter_stack->data[1]->value->value;
	int op = parameter_stack->data[2]->value->key;
	Identifier* result = newtemp(INT);
	gencode(op, arg1, arg2, result);

	Tuple_2* p = (Tuple_2*)malloc(sizeof(Tuple_2));
	p->key = 0;
	p->value = result;
	p->next = NULL;
	new_state->value = p;

	return 0;
}

int F57(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F61(State * new_state, Stack * parameter_stack)
{
	//pass_param(new_state, parameter_stack);
	Tuple_2* p = (Tuple_2*)malloc(sizeof(Tuple_2));
	State* tstate = parameter_stack->data[1];
	Tuple_2* tvalue = tstate->value;
	Identifier* tid = lookup(ID_SymbolTable, tvalue->value->name);
	if (tid)
	{
		p->key = 0;
		p->value = tid;
		p->next = NULL;
		new_state->value = p;
	}
	else
	{
		printf("error!\n");
		pass_param(new_state, parameter_stack);
	}
	return 0;
}

int F62(State * new_state, Stack * parameter_stack)
{
	pass_param(new_state, parameter_stack);
	return 0;
}

int F65(State * new_state, Stack * parameter_stack)
{
	Constant_SymbolTable[constant_offset++] = parameter_stack->data[1]->value->value;
	pass_param(new_state, parameter_stack);
	return 0;
}
